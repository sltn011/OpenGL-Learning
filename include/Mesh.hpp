/**
* @file Mesh.hpp
*/

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

    /**
     * @brief Struct that represents vertex of Model's Mesh
    */
    struct Vertex {
        /// Position of Mesh vertex in model space
        glm::vec3 m_pos;

        /// Normal vector
        glm::vec3 m_norm;

        /// Coordinate of texture
        glm::vec2 m_tex;
    };

    /**
     * @brief Enum that represents different types of textures
    */
    enum class TextureType {
        Diffuse,
        Specular,
        Normal,
        Height
    };

    /**
     * @brief Struct that represents texture used in Model's Mesh
    */
    struct ModelTexture {
        /// Texture descriptor
        unsigned int m_id;

        /// Type of texture
        TextureType  m_type;

        /// Path to texture
        std::string  m_path;
    };

    /**
     * @brief Struct that represents material used in Model's Mesh
    */
    struct Colors {
        glm::vec3 m_ambient;
        glm::vec3 m_diffuse;
        glm::vec3 m_specular;
        float     m_specularExponent;
    };

    /**
     * @brief Class that represents part of Model
    */
    class Mesh {
        /// VertexArrayObject used to render Mesh
        VertexArrayObject   m_VAO;

        /// VertexBufferObject that stores vertices data of Mesh
        VertexBufferObject  m_VBO;

        /// ElementBufferObject that stores indices data of Mesh
        ElementBufferObject m_EBO;

        /// Vertex container of a Mesh
        std::vector<Vertex>       m_vertices;

        /// Indices container of a Mesh
        std::vector<unsigned int> m_indices;

        /// Container of Model textures used in Mesh
        std::vector<ModelTexture> m_textures;

        /// Mesh material
        Colors                    m_colors;
        
        /**
         * @brief Loads Mesh vertices and indices data into m_VAO, m_VBO, m_EBO
        */
        void setup(
        );

        /**
         * @brief Loads Mesh textures and material into uniform material struct in target shader
         * @param shader Currently active target Shader in which data will be loaded
        */
        void loadToShader(
            OGL::Shader &shader
        ) const;

     public:
        
        /**
         * @brief Mesh constructor
         * @param vertices Vertex container of a Mesh
         * @param indices Indices container of a Mesh
         * @param textures Container of Model textures used in Mesh
         * @param color Mesh material
        */
        Mesh( 
            std::vector<Vertex> vertices,
            std::vector<unsigned int> indices,
            std::vector<ModelTexture> textures,
            Colors color
        );

        /**
         * @brief Mesh destructor
        */
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

        /**
         * @brief Fully renders Mesh with supplied Shader
         * @param shader Currently active Shader which is used to render Mesh
        */
        void draw( 
            OGL::Shader &shader
        ) const;

        /**
         * @brief Fully renders amount instances of a Mesh with supplied Shader
         * @param shader Currently active Shader which is used to render Mesh
         * @param amount Number of instances to be rendered
        */
        void drawInstanced( 
            OGL::Shader &shader,
            uint32_t amount
        ) const;

        /**
         * @brief Renders only shape of a Mesh with supplied Shader without loading textures and material into it
         * @param shader Currently active Shader which is used to render Mesh
        */
        void drawShape(
            Shader &shader
        ) const;

        /**
         * @brief Renders only shape of amount instances of a Mesh with supplied Shader without loading textures and material into it
         * @param shader Currently active Shader which is used to render Mesh
         * @param amount Number of instances to be rendered
        */
        void drawShapeInstanced(
            Shader &shader,
            uint32_t amount
        ) const;

        /**
         * @brief Enables vertex attribute for Mesh to pass Mat4x4 data into vertex Shader
         * @param attribLocation First location index used by Mat4x4 data - indexes from attribLocation to attribLocation + 3 will be occupied
        */
        void setVertexAttribInstancedModelMat4( 
            int attribLocation
        );

    };

}

#endif // OGL_MESH_H