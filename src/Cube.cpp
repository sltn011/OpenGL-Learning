#include "Cube.hpp"

namespace OGL {

    Cube::Cube(
        float side
    ) : m_sideA{side},
        m_sideB{side},
        m_sideC{side} {

        generateMesh();

    }

    Cube::Cube(
        float a,
        float b,
        float c
    ) : m_sideA{ a },
        m_sideB{ b },
        m_sideC{ c } {

        generateMesh();

    }

    void Cube::generateMesh(
    ) {
        std::vector<Vertex>       vertices(24);
        std::vector<unsigned int> indices(36);
        std::vector<ModelTexture> textures;
        Material                  material{};

        const glm::vec3 vertexCoord[8] = {
             // X             // Y             // Z
            {-0.5f * m_sideA, -0.5f * m_sideB, -0.5f * m_sideC}, // 0
            {-0.5f * m_sideA, -0.5f * m_sideB, +0.5f * m_sideC}, // 1
            {-0.5f * m_sideA, +0.5f * m_sideB, -0.5f * m_sideC}, // 2
            {-0.5f * m_sideA, +0.5f * m_sideB, +0.5f * m_sideC}, // 3
            {+0.5f * m_sideA, -0.5f * m_sideB, -0.5f * m_sideC}, // 4
            {+0.5f * m_sideA, -0.5f * m_sideB, +0.5f * m_sideC}, // 5
            {+0.5f * m_sideA, +0.5f * m_sideB, -0.5f * m_sideC}, // 6
            {+0.5f * m_sideA, +0.5f * m_sideB, +0.5f * m_sideC}, // 7
        };

        constexpr glm::vec2 texCoord[24] = {
            {0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, // 0 1 2 3
            {0.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, // 4 5 6 7
            {0.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f}, // 8 9 10 11
            {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f}, // 12 13 14 15
            {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, // 16 17 18 19
            {1.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}  // 20 21 22 23
        };

        constexpr unsigned int cubeIndices[36] = {
            0, 3, 6,
            3, 9, 6,
            12, 1, 18,
            1, 7, 18,
            15, 13, 19,
            15, 19, 21,
            4, 16, 22,
            4, 22, 10,
            11, 23, 20,
            11, 20, 8,
            2, 14, 5,
            14, 17, 5
        };

        for (size_t i = 0; i < 8; ++i) {
            size_t offset = 3 * i;
            for (size_t j = 0; j < 3; ++j) {
                Vertex &v = vertices[offset + j];
                v.m_pos = vertexCoord[i];
                v.m_tex = texCoord[offset + j];
            }
        }

        for (size_t i = 0; i < 36; ++i) {
            indices[i] = cubeIndices[i];
        }

        for (size_t i = 0; i < 12; ++i) {
            Vertex &v1 = vertices[indices[3 * i + 0]];
            Vertex &v2 = vertices[indices[3 * i + 1]];
            Vertex &v3 = vertices[indices[3 * i + 2]];

            glm::vec3 normal = glm::normalize(glm::cross(v2.m_pos - v1.m_pos, v3.m_pos - v1.m_pos));
            glm::vec3 tangent = CalculateTangent(v1, v2, v3);

            v1.m_norm = normal;
            v1.m_tangent = tangent;
            v2.m_norm = normal;
            v2.m_tangent = tangent;
            v3.m_norm = normal;
            v3.m_tangent = tangent;
        }

        material.m_ambient = glm::vec3(0.1f);
        material.m_diffuse = glm::vec3(0.5f);
        material.m_specular = glm::vec3(0.05f);
        material.m_specularExponent = 10.0f;

        m_meshes.emplace_back(vertices, indices, textures, material);
    }
}