#include "Mesh.hpp"

namespace OGL {

    Mesh::Mesh( 
        std::vector<Vertex> vertices,
        std::vector<unsigned int> indices,
        std::vector<ModelTexture> textures,
        Colors colors
    ) : m_vertices{ std::move(vertices) }, 
        m_indices { std::move(indices)  }, 
        m_textures{ std::move(textures) }, 
        m_colors{colors} {
        setup();
    }

    void Mesh::setup(
    ) {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(*m_vertices.data()), m_vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(*m_indices.data()), m_indices.data(), GL_STATIC_DRAW);

        // Vertex pos
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);

        // Vertex norm
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, norm));
        glEnableVertexAttribArray(1);

        // Vertex texture
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }

    void Mesh::draw(
        OGL::Shader &shader
    ) const {
        unsigned int diffuseTexCnt  = 0;
        unsigned int specularTexCnt = 0;
        unsigned int normalTexCnt   = 0;
        unsigned int heightTexCnt   = 0;
        
        for (size_t i = 0; i < m_textures.size(); ++i) {
            glActiveTexture(GL_TEXTURE0 + i);

            std::string name;

            switch (m_textures[i].type) {
            case TextureType::Diffuse:
                name = "textureDiffuse[" + std::to_string(diffuseTexCnt++) + "]";
                break;

            case TextureType::Specular:
                name = "textureSpecular[" + std::to_string(specularTexCnt++) + "]";
                break;

            case TextureType::Normal:
                name = "textureNormal[" + std::to_string(normalTexCnt++) + "]";
                break;

            case TextureType::Height:
                name = "textureHeight[" + std::to_string(heightTexCnt++) + "]";
                break;

            default:
                continue;
                break;
            }

            shader.setUniformInt("material." + std::move(name), i);
            glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
        }
        glActiveTexture(GL_TEXTURE0);

        shader.setUniformInt("material.numDiffuseTextures", diffuseTexCnt);
        shader.setUniformInt("material.numSpecularTextures", specularTexCnt);
        shader.setUniformInt("material.numNormalTextures", normalTexCnt);
        shader.setUniformInt("material.numHeightTextures", heightTexCnt);
        shader.setUniformVec3("material.colorAmbient", m_colors.ambient);
        shader.setUniformVec3("material.colorDiffuse", m_colors.diffuse);
        shader.setUniformVec3("material.colorSpecular", m_colors.specular);
        shader.setUniformFloat("material.specularExponent", m_colors.specularExponent);

        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void Mesh::drawInstanced( 
        OGL::Shader &shader, 
        size_t amount
    ) const {
        unsigned int diffuseTexCnt = 0;
        unsigned int specularTexCnt = 0;
        unsigned int normalTexCnt = 0;
        unsigned int heightTexCnt = 0;

        for (size_t i = 0; i < m_textures.size(); ++i) {
            glActiveTexture(GL_TEXTURE0 + i);

            std::string name;

            switch (m_textures[i].type) {
            case TextureType::Diffuse:
                name = "textureDiffuse[" + std::to_string(diffuseTexCnt++) + "]";
                break;

            case TextureType::Specular:
                name = "textureSpecular[" + std::to_string(specularTexCnt++) + "]";
                break;

            case TextureType::Normal:
                name = "textureNormal[" + std::to_string(normalTexCnt++) + "]";
                break;

            case TextureType::Height:
                name = "textureHeight[" + std::to_string(heightTexCnt++) + "]";
                break;

            default:
                continue;
                break;
            }

            shader.setUniformInt("material." + std::move(name), i);
            glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
        }
        glActiveTexture(GL_TEXTURE0);

        shader.setUniformInt("material.numDiffuseTextures", diffuseTexCnt);
        shader.setUniformInt("material.numSpecularTextures", specularTexCnt);
        shader.setUniformInt("material.numNormalTextures", normalTexCnt);
        shader.setUniformInt("material.numHeightTextures", heightTexCnt);
        shader.setUniformVec3("material.colorAmbient", m_colors.ambient);
        shader.setUniformVec3("material.colorDiffuse", m_colors.diffuse);
        shader.setUniformVec3("material.colorSpecular", m_colors.specular);
        shader.setUniformFloat("material.specularExponent", m_colors.specularExponent);

        glBindVertexArray(m_VAO);
        glDrawElementsInstanced(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0, amount);
        glBindVertexArray(0);
    }

    void Mesh::setVertexAttribInstancedModelMat4( 
        int attribLocation
    ) {
        glBindVertexArray(m_VAO);
        for (int i = 0; i < 4; ++i) {
            glEnableVertexAttribArray(attribLocation + i);
            glVertexAttribPointer(attribLocation + i, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(i * sizeof(glm::vec4)));
            glVertexAttribDivisor(attribLocation + i, 1);
        }
        glBindVertexArray(0);
    }

} // OGL