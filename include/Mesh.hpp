#ifndef OGL_MESH_H
#define OGL_MESH_H

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "Shader.hpp"
#include "VertexArrayObject.hpp"
#include "VertexBufferObject.hpp"
#include "ElementBufferObject.hpp"

#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <sstream>

namespace OGL {

    struct Vertex {
        glm::vec3 m_pos;
        glm::vec3 m_norm;
        glm::vec2 m_tex;
    };

    enum class TextureType {
        Diffuse,
        Specular,
        Normal,
        Height
    };

    struct ModelTexture {
        unsigned int m_id;
        TextureType  m_type;
        std::string  m_path;
    };

    struct Colors {
        glm::vec3 m_ambient;
        glm::vec3 m_diffuse;
        glm::vec3 m_specular;
        float     m_specularExponent;
    };

    class Mesh {
        VertexArrayObject   m_VAO;
        VertexBufferObject  m_VBO;
        ElementBufferObject m_EBO;

        std::vector<Vertex>       m_vertices;
        std::vector<unsigned int> m_indices;
        std::vector<ModelTexture> m_textures;
        Colors                    m_colors;
        

        void setup(
        );

        void loadToShader(
            OGL::Shader& shader
        ) const;

     public:
        
        Mesh( 
            std::vector<Vertex> vertices,
            std::vector<unsigned int> indices,
            std::vector<ModelTexture> textures,
            Colors color
        );

        ~Mesh(
        );

        Mesh(
            Mesh const &rhs
        ) = delete;

        Mesh &operator=(
            Mesh const &rhs
        ) = delete;

        /**
         * @brief Mesh move constructor
         * @param rhs Moved value
        */
        Mesh(
            Mesh &&rhs
        ) noexcept;

        /**
         * @brief Mesh move assignment
         * @param rhs Moved value
         * @return Reference to *this
        */
        Mesh &operator=(
            Mesh &&rhs
        ) noexcept;

        void draw( 
            OGL::Shader &shader
        ) const;

        void drawInstanced( 
            OGL::Shader &shader,
            uint32_t amount
        ) const;

        void drawShape(
            Shader &shader
        ) const;

        void drawShapeInstanced(
            Shader &shader,
            uint32_t amount
        ) const;

        void setVertexAttribInstancedModelMat4( 
            int attribLocation
        );

    };

}

#endif // OGL_MESH_H