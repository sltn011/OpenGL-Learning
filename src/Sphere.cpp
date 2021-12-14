#include "Sphere.hpp"

namespace OGL {

    Sphere::Sphere(
        float radius,
        int sectors,
        int stacks
    ) : m_radius{ radius },
        m_sectors{ sectors },
        m_stacks{ stacks } {
        generateMesh();
    }

    void Sphere::generateMesh(
    ) {
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;
        std::vector<ModelTexture> textures;
        Material                  material{};

        float const pi = glm::acos(-1);
        float const lengthInv = 1.0f / m_radius;

        float const sectorStep = 2 * pi / m_sectors;
        float const stackStep = pi / m_stacks;

        for (int i = 0; i <= m_stacks; ++i) {

            float stackAngle = (pi / 2.0f) - (i * stackStep);
            float xy = m_radius * glm::cos(stackAngle);
            float z = m_radius * glm::sin(stackAngle);

            for (int j = 0; j <= m_sectors; ++j) {

                float sectorAngle = j * sectorStep;
                float x = xy * glm::cos(sectorAngle);
                float y = xy * glm::sin(sectorAngle);

                float nx = x * lengthInv;
                float ny = y * lengthInv;
                float nz = z * lengthInv;

                float tx = static_cast<float>(j) / static_cast<float>(m_sectors);
                float ty = static_cast<float>(i) / static_cast<float>(m_stacks);

                Vertex vertex;
                vertex.m_pos = glm::vec3{ x, y, z };
                vertex.m_norm = glm::normalize(glm::vec3{ nx, ny, nz });
                vertex.m_tex = glm::vec2{ tx, ty };

                vertices.push_back(vertex);
            }
        }

        //  i1--i1+1
        //  |  / |
        //  | /  |
        //  i2--i2+1
        int i1, i2;
        for (int i = 0; i < m_stacks; ++i) {

            i1 = i * (m_sectors + 1);
            i2 = i1 + m_sectors + 1;

            for (int j = 0; j < m_sectors; ++j, ++i1, ++i2) {

                if (i != 0) {
                    indices.push_back(i1);
                    indices.push_back(i2);
                    indices.push_back(i1 + 1);
                }

                if (i != (m_stacks - 1)) {
                    indices.push_back(i1 + 1);
                    indices.push_back(i2);
                    indices.push_back(i2 + 1);
                }

            }
        }

        if (indices.size() % 3 == 0) {
            for (size_t i = 0; i < indices.size(); i += 3) {
                Vertex &a = vertices[indices[i]];
                Vertex &b = vertices[indices[i + 1]];
                Vertex &c = vertices[indices[i + 2]];

                glm::vec3 tangent = CalculateTangent(a, b, c);
                a.m_tangent = tangent;
                b.m_tangent = tangent;
                c.m_tangent = tangent;
            }
        }

        material.m_ambient = glm::vec3(0.1f);
        material.m_diffuse = glm::vec3(0.5f);
        material.m_specular = glm::vec3(0.05f);
        material.m_specularExponent = 10.0f;

        m_meshes.emplace_back(vertices, indices, textures, material);
    }
}