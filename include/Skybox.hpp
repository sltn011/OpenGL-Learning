/**
* @file Skybox.hpp
*/

#ifndef OGL_SKYBOX_H
#define OGL_SKYBOX_H

#include "Cubemap.hpp"
#include "VertexArrayObject.hpp"
#include "VertexBufferObject.hpp"
#include "ElementBufferObject.hpp"

namespace OGL {

    /**
     * @brief Class that handles Skybox Cubemap functionality
    */
    class Skybox : public Cubemap {
     protected:
        /// VertexArrayObject of Skybox
        VertexArrayObject m_vao;

        /// VertexBufferObject of Skybox
        VertexBufferObject m_vbo;

        /// ElementBufferObject of Skybox
        ElementBufferObject m_ebo;

     public:
        /**
         * @brief Skybox constructor
         * @param folderPath Path to folder containing 6 JPG files with names "top.jpg", "left.jpg", "front.jpg", etc
         * @param cubemapTextureUnit Unit value assigned to Skybox = GL_TEXTURE{N}
        */
        Skybox( 
            std::string const &folderPath,
            GLenum cubemapTextureUnit
        );

        Skybox(
            Skybox const &rhs
        ) = delete;

        Skybox(
            Skybox &&rhs
        ) = default;

        Skybox &operator=(
            Skybox const &rhs
        ) = delete;

        Skybox &operator=(
            Skybox &&rhs
        ) = default;

        /**
         * @brief Binds Skybox to it's texture unit
        */
        void bind(
        ) const override;

        /**
         * @brief Unbinds Skybox
        */
        static void unbind(
        );
        
    };

} // OGL


#endif // OGL_SKYBOX_H
