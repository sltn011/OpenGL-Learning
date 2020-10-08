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
            ss >> t.v[0] >> t.v[1] >> t.v[2];
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

    void TexturedMesh::writeTexturedTriangleData
    ( uint8_t *dest
    , TexturedTriangle const src
    ) {
        writeData(dest, (uint8_t*)&src, sizeof(src));
    }

    bool TexturedMesh::readFromFile
    ( std::string const &filePath
    ) {
        std::ifstream f(filePath);
        if (!f.is_open()) {
            return false;
        }
        
        std::string buf;
        
        std::vector<glm::vec3> verts;
        std::vector<glm::vec2> texts;

        while (std::getline(f, buf)) {

            std::stringstream ss(std::move(buf));

            char lineType;
            ss >> lineType;

            switch (lineType) {
            case 'v': {
                char next;
                ss.read(&next, 1);
                if (next == 't') {
                    glm::vec2 text;
                    ss >> text.x >> text.y;
                    texts.push_back(text);
                }
                else {
                    glm::vec3 vert;
                    ss >> vert.x >> vert.y >> vert.z;
                    verts.push_back(vert);
                }
                break;
            }

            case 'f': {
                std::string tokens[6];
                int tokensCount = -1;
                char c;
                while(ss.read(&c, 1)) {
                    if (c == ' ' || c == '/') {
                        ++tokensCount;
                    }
                    else {
                        tokens[tokensCount].append(1, c);
                    }
                }
                TexturedTriangle tt;
                for (int i = 0; i < 3; ++i) {
                    tt.v[i].vert = verts[std::stoi(tokens[2 * i]) - 1];
                    tt.v[i].text = texts[std::stoi(tokens[2 * i + 1]) - 1];
                }
                m_texVertex.push_back(tt);
                break;
            }

            default: {
                break;
            }
            }
        }
        return true;
    }

    std::unique_ptr<uint8_t[]> TexturedMesh::getVBOData
    (
    ) const {
        size_t dataSize = sizeVBO();
        size_t blockSize = sizeof(*m_texVertex.data());
        size_t numBlocks = dataSize / blockSize;
        std::unique_ptr<uint8_t[]> data = std::make_unique<uint8_t[]>(dataSize);
        for (size_t i = 0; i < numBlocks; ++i) {
            writeTexturedTriangleData(data.get() + i * blockSize, m_texVertex[i]);
        }
        return data;
    }

    size_t TexturedMesh::sizeVBO
    (
    ) const {
        return m_texVertex.size() * sizeof(*m_texVertex.data());
    }

    std::unique_ptr<uint8_t[]> TexturedMesh::getEBOData
    (
    ) const {
        return nullptr;
    }

    size_t TexturedMesh::sizeEBO
    (
    ) const {
        return 0;
    }

    size_t TexturedMesh::numElements
    (
    ) const {
        return 0;
    }

    size_t TexturedMesh::numVertices
    (
    ) const {
        return 3 * m_texVertex.size();
    }

} // OGL