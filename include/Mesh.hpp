#ifndef OGL_MESH_H
#define OGL_MESH_H

#include "glm/glm.hpp"

#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <sstream>

namespace OGL {

    struct RGBA {
        float r;
        float g;
        float b;
        float a;
    };

    struct Triangle {
        int a;
        int b;
        int c;
    };

    class BasicMesh {
    protected:
        std::vector<glm::vec3> m_vertices;
        std::vector<Triangle> m_faces;

        static void writeData
        ( uint8_t *dest
        , uint8_t const *src
        , size_t length
        );

        static void writeVertexData
        ( uint8_t *dest
        , glm::vec3 const src
        );

        static void writeFaceData
        ( uint8_t *dest
        , Triangle const src
        );

    public:
        virtual ~BasicMesh
        (
        );

        virtual bool readFromFile
        ( std::string const &filePath
        ) = 0;

        virtual std::unique_ptr<uint8_t[]> getVBOData
        (
        ) const = 0;

        virtual size_t sizeVBO
        (
        ) const = 0;

        virtual std::unique_ptr<uint8_t[]> getEBOData
        (
        ) const = 0;

        virtual size_t sizeEBO
        (
        ) const = 0;

        virtual size_t numElements
        (
        ) const = 0;
    };

    // For now only works with triangulated .ply models!!!
    class ColoredMesh : public BasicMesh {
    protected:
        std::vector<RGBA> m_verticesColor;

        static void writeVertexColorData
        ( uint8_t *dest
        , RGBA const src
        );

    public:
        bool readFromFile
        ( std::string const &filePath
        ) override;

        std::unique_ptr<uint8_t[]> getVBOData
        (
        ) const override;

        size_t sizeVBO
        (
        ) const override;

        std::unique_ptr<uint8_t[]> getEBOData
        (
        ) const override;

        size_t sizeEBO
        (
        ) const override;

        size_t numElements
        (
        ) const override;
    };

}

#endif // OGL_MESH_H