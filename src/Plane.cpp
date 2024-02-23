#include "Plane.hpp"
#include "Plane.hpp"

namespace OGL {

    Plane::Plane(
        float side
    ) : m_sideA{ side },
        m_sideB{ side },
        m_res{ 1 } {

        generateMesh();

    }

    Plane::Plane(
        float a,
        float b
    ) : m_sideA{ a },
        m_sideB{ b },
        m_res{ 1 } {

        generateMesh();

    }

    Plane::Plane(
        float a,
        float b,
        int res
    ) : m_sideA{ a },
        m_sideB{ b },
        m_res{ res } {
        generateMesh();
    }

    void Plane::generateMesh(
    ) {
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;

        std::vector<ModelTexture> textures;
        Material                  material{};

        float a_r = m_sideA / m_res;
        float b_r = m_sideB / m_res;

        float o_r = 1.0f / m_res;

        float ha_r = a_r / 2;
        float hb_r = b_r / 2;

        for (int i = 0; i < m_res; ++i) // a offset
        {
            for (int j = 0; j < m_res; ++j) // b offset
            {
                vertices.push_back({});
                vertices.push_back({});
                vertices.push_back({});
                vertices.push_back({});

                Vertex &v0 = vertices[vertices.size() - 4]; // 0     1-----3
                Vertex &v1 = vertices[vertices.size() - 3]; // 1     | \   |
                Vertex &v2 = vertices[vertices.size() - 2]; // 2     |   \ |
                Vertex &v3 = vertices[vertices.size() - 1]; // 3     0-----2

                // Pos
                {
                    glm::vec3 bl_c = { -0.5f * m_sideA, 0.0f, -0.5f * m_sideB };
                    bl_c.x += a_r * i;
                    bl_c.z += b_r * j;

                    v0.m_pos = bl_c + glm::vec3{ 0 * a_r, 0, 0 * b_r };
                    v1.m_pos = bl_c + glm::vec3{ 0 * a_r, 0, 1 * b_r };
                    v2.m_pos = bl_c + glm::vec3{ 1 * a_r, 0, 0 * b_r };
                    v3.m_pos = bl_c + glm::vec3{ 1 * a_r, 0, 1 * b_r };
                }

                // UV
                {
                    glm::vec2 bl_c = { 0.0f, 0.0f };
                    bl_c.x += o_r * i;
                    bl_c.y += o_r * j;

                    v0.m_tex = bl_c + glm::vec2{ 0 * o_r, 0 * o_r };
                    v1.m_tex = bl_c + glm::vec2{ 0 * o_r, 1 * o_r };
                    v2.m_tex = bl_c + glm::vec2{ 1 * o_r, 0 * o_r };
                    v3.m_tex = bl_c + glm::vec2{ 1 * o_r, 1 * o_r };
                }
            }
        }

        int offset = 0;
        for (int i = 0; i < m_res; ++i) // a offset
        {
            for (int j = 0; j < m_res; ++j) // b offset
            {
                // Triangle 1
                {
                    // 0     1-----3
                    // 1     | \   |
                    // 2     |   \ |
                    // 3     0-----2
                    //
                    // 0, 2, 1,

                    indices.push_back({});
                    indices.push_back({});
                    indices.push_back({});

                    unsigned &i0 = indices[indices.size() - 3];
                    unsigned &i1 = indices[indices.size() - 2];
                    unsigned &i2 = indices[indices.size() - 1];

                    i0 = offset + 0;
                    i1 = offset + 2;
                    i2 = offset + 1;
                }

                // Triangle 2
                {
                    // 0     1-----3
                    // 1     | \   |
                    // 2     |   \ |
                    // 3     0-----2
                    //
                    // 1, 2, 3,

                    indices.push_back({});
                    indices.push_back({});
                    indices.push_back({});

                    unsigned &i0 = indices[indices.size() - 3];
                    unsigned &i1 = indices[indices.size() - 2];
                    unsigned &i2 = indices[indices.size() - 1];

                    i0 = offset + 1;
                    i1 = offset + 2;
                    i2 = offset + 3;
                }

                offset += 4;
            }
        }

        for (size_t i = 0; i < 2; ++i) {
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