#include "Mesh.hpp"

namespace OGL {

    Mesh::Mesh( 
        std::vector<Vertex> vertices,
        std::vector<unsigned int> indices,
        std::vector<ModelTexture> textures,
        Material material
    ) : m_vertices{ std::move(vertices) }, 
        m_indices { std::move(indices)  }, 
        m_textures{ std::move(textures) }, 
        m_material{ material } {
        setup();
    }

    Mesh::~Mesh(
    ) {
        for (size_t i = 0; i < m_textures.size(); ++i) {
            glDeleteTextures(1, &(m_textures[i].m_id));
        }
    }

    Mesh::Mesh(
        Mesh &&rhs
    ) noexcept :
        m_VAO{ std::move(rhs.m_VAO) },
        m_VBO{ std::move(rhs.m_VBO) },
        m_EBO{ std::move(rhs.m_EBO) },
        m_vertices{ std::move(rhs.m_vertices) },
        m_indices{ std::move(rhs.m_indices) },
        m_textures{ rhs.m_textures.size() },
        m_material{ std::move(rhs.m_material) } {

        for (size_t i = 0; i < m_textures.size(); ++i) {
            m_textures[i].m_id = std::exchange(rhs.m_textures[i].m_id, 0);
            m_textures[i].m_type = rhs.m_textures[i].m_type;
            m_textures[i].m_path = std::move(rhs.m_textures[i].m_path);
        }

    }

    Mesh &Mesh::operator=(
        Mesh &&rhs
    ) noexcept {
        m_VAO = std::move(rhs.m_VAO);
        m_VBO = std::move(rhs.m_VBO);
        m_EBO = std::move(rhs.m_EBO);
        m_vertices   = std::move(rhs.m_vertices);
        m_indices    = std::move(rhs.m_indices);
        m_material   = std::move(rhs.m_material);

        for (size_t i = 0; i < m_textures.size(); ++i) {
            glDeleteTextures(1, &(m_textures[i].m_id));
        }

        m_textures.resize(rhs.m_textures.size());

        for (size_t i = 0; i < m_textures.size(); ++i) {
            m_textures[i].m_id = std::exchange(rhs.m_textures[i].m_id, 0);
            m_textures[i].m_type = rhs.m_textures[i].m_type;
            m_textures[i].m_path = std::move(rhs.m_textures[i].m_path);
        }

        return *this;
    }

    void Mesh::setup(
    ) {
        m_VAO.bind();

        m_VBO.bind();
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(*m_vertices.data()), m_vertices.data(), GL_STATIC_DRAW);

        m_EBO.bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(*m_indices.data()), m_indices.data(), GL_STATIC_DRAW);

        // Vertex pos
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);

        // Vertex norm
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_norm));
        glEnableVertexAttribArray(1);

        // Vertex texture
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_tex));
        glEnableVertexAttribArray(2);

        VertexArrayObject::unbind();
    }

    void Mesh::loadToShader(
        OGL::Shader &shader
    ) const {
        unsigned int diffuseTexCnt = 0;
        unsigned int specularTexCnt = 0;
        unsigned int normalTexCnt = 0;
        unsigned int heightTexCnt = 0;

        for (size_t i = 0; i < m_textures.size(); ++i) {
            glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(i));

            std::string name;

            switch (m_textures[i].m_type) {
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

            shader.setUniformInt("material." + std::move(name), static_cast<int>(i));
            glBindTexture(GL_TEXTURE_2D, m_textures[i].m_id);
        }
        glActiveTexture(GL_TEXTURE0);

        shader.setUniformInt("material.numDiffuseTextures", diffuseTexCnt);
        shader.setUniformInt("material.numSpecularTextures", specularTexCnt);
        shader.setUniformInt("material.numNormalTextures", normalTexCnt);
        shader.setUniformInt("material.numHeightTextures", heightTexCnt);
        shader.setUniformVec3("material.colorAmbient", m_material.m_ambient);
        shader.setUniformVec3("material.colorDiffuse", m_material.m_diffuse);
        shader.setUniformVec3("material.colorSpecular", m_material.m_specular);
        shader.setUniformFloat("material.specularExponent", m_material.m_specularExponent);
    }

    void Mesh::draw(
        OGL::Shader &shader
    ) const {
        loadToShader(shader);
        m_VAO.bind();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, 0);
        VertexArrayObject::unbind();
    }

    void Mesh::drawInstanced( 
        OGL::Shader &shader, 
        uint32_t amount
    ) const {
        loadToShader(shader);
        m_VAO.bind();
        glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, 0, static_cast<GLsizei>(amount));
        VertexArrayObject::unbind();
    }

    void Mesh::drawShape(
        Shader &shader
    ) const {
        m_VAO.bind();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, 0);
        VertexArrayObject::unbind();
    }

    void Mesh::drawShapeInstanced(
        Shader &shader, 
        uint32_t amount
    ) const {
        m_VAO.bind();
        glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, 0, static_cast<GLsizei>(amount));
        VertexArrayObject::unbind();
    }

    void Mesh::setVertexAttribInstancedModelMat4( 
        int attribLocation
    ) {
        m_VAO.bind();
        for (int i = 0; i < 4; ++i) {
            glEnableVertexAttribArray(attribLocation + i);
            glVertexAttribPointer(attribLocation + i, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(i * sizeof(glm::vec4)));
            glVertexAttribDivisor(attribLocation + i, 1);
        }
        VertexArrayObject::unbind();
    }

} // OGL