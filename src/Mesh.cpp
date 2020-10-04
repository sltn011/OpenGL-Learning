#include "Mesh.hpp"

namespace OGL {

    BasicMesh::~BasicMesh() {}

    void BasicMesh::writeData
    ( uint8_t *dest
    , uint8_t const *src
    , size_t length
    ) {
        for (size_t i = 0; i < length; ++i) {
            *(dest + i) = *(src + i);
        }
    }

    void BasicMesh::writeVertexData
    ( uint8_t *dest
    , glm::vec3 const src
    ) {
        float coords[3] = {src.x, src.y, src.z};
        writeData(dest, (uint8_t*)coords, sizeof(coords));
    }

    void BasicMesh::writeFaceData
    ( uint8_t *dest
    , Triangle const src
    ) {
        writeData(dest, (uint8_t*)&src, sizeof(src));
    }

    void ColoredMesh::writeVertexColorData
    ( uint8_t *dest
    , RGBA const src
    ) {
        writeData(dest, (uint8_t*)&src, sizeof(src));
    }

    bool ColoredMesh::readFromFile
    ( std::string const &filePath
    ) {
        std::ifstream f(filePath);
        if (!f.is_open()) {
            return false;
        }

        std::string const vertsLine = "element vertex ";
        std::string const facesLine = "element face ";
        std::string const endHeader = "end_header";

        int nVerts, nFaces;
        std::string buf;

        while (std::getline(f, buf)) {
            if (buf.find(vertsLine) != std::string::npos) {
                buf.erase(buf.begin(), buf.begin() + vertsLine.size());
                nVerts = std::stoi(buf);
            }
            else if (buf.find(facesLine) != std::string::npos) {
                buf.erase(buf.begin(), buf.begin() + facesLine.size());
                nFaces = std::stoi(buf);
            }
            else if (buf.find(endHeader) != std::string::npos) {
                break;
            }
        }

        for (int i = 0; i < nVerts; ++i) {
            std::getline(f, buf);
            std::stringstream ss(std::move(buf));
            glm::vec3 v;
            RGBA vc;
            int bufColor[4];
            ss >> v.x >> v.y >> v.z >> bufColor[0] >> bufColor[1] >> bufColor[2] >> bufColor[3];
            vc.r = bufColor[0] / 255.0f;
            vc.g = bufColor[1] / 255.0f;
            vc.b = bufColor[2] / 255.0f;
            vc.a = bufColor[3] / 255.0f;
            m_vertices.push_back(v);
            m_verticesColor.push_back(vc);
        }

        for (int i = 0; i < nFaces; ++i) {
            std::getline(f, buf);
            std::stringstream ss(std::move(buf));
            int vertsInFace;
            ss >> vertsInFace; // Always 3 for now
            Triangle t;
            ss >> t.a >> t.b >> t.c;
            m_faces.push_back(t);
        }
        return true;
    }

    std::unique_ptr<uint8_t[]> ColoredMesh::getVBOData
    (
    ) const {
        size_t dataSize = sizeVBO();
        size_t blockSize = sizeof(*m_vertices.data()) + sizeof(*m_verticesColor.data());
        size_t numBlocks = dataSize / blockSize;
        std::unique_ptr<uint8_t[]> data = std::make_unique<uint8_t[]>(dataSize);
        for (size_t i = 0; i < numBlocks; ++i) {
            size_t offset = blockSize * i;
            writeVertexData(data.get() + offset, m_vertices[i]);
            offset += sizeof(m_vertices[i]);
            writeVertexColorData(data.get() + offset, m_verticesColor[i]);
        }
        return data;
    }

    size_t ColoredMesh::sizeVBO
    (
    ) const {
        return m_vertices.size() * sizeof(*m_vertices.data()) + m_verticesColor.size() * sizeof(*m_verticesColor.data());
    }

    std::unique_ptr<uint8_t[]> ColoredMesh::getEBOData
    (
    ) const {
        size_t dataSize = sizeEBO();
        size_t blockSize = sizeof(*m_faces.data());
        size_t numBlocks = dataSize / blockSize;
        std::unique_ptr<uint8_t[]> data = std::make_unique<uint8_t[]>(dataSize);
        for (size_t i = 0; i < numBlocks; ++i) {
            size_t offset = blockSize * i;
            writeFaceData(data.get() + offset, m_faces[i]);
        }
        return data;
    }

    size_t ColoredMesh::sizeEBO
    (
    ) const {
        return m_faces.size() * sizeof(*m_faces.data());
    }

    size_t ColoredMesh::numElements
    (
    ) const {
        return 3 * m_faces.size();
    }

} // OGL