/**
* @file Cubemap.hpp
*/

#ifndef OGL_CUBEMAP_H
#define OGL_CUBEMAP_H

#include "glad/glad.h"
#include "Texture.hpp"

#include <string>

namespace OGL {

    /**
     * @brief Cubemap class to work with OpenGL cubemaps
    */
    class Cubemap {
     protected:
        /// Cubemap Texture
        Texture     m_texture;

        /// Cubemap's texture unit GL_TEXTURE{N}
        GLenum      m_textureUnit;

        /// Cubemap texture folder path - set if Cubemap was loaded from folder
        std::string m_folderPath;

     public:

        /**
         * @brief Constructor for empty Cubemap
         * @param size Size of Cubemap side
         * @param cubemapTextureUnit Unit value assigned to Cubemap = GL_TEXTURE{N}
        */
        Cubemap( 
            int size,
            GLenum cubemapTextureUnit
        );

        /**
         * @brief Constructor for Cubemap loaded from folder
         * @param folderPath Path to folder containing 6 JPG files with names "top.jpg", "left.jpg", "front.jpg", etc
         * @param cubemapTextureUnit Unit value assigned to Cubemap = GL_TEXTURE{N}
        */
        Cubemap( 
            std::string const &folderPath, 
            GLenum cubemapTextureUnit
        );

        Cubemap( 
            Cubemap const &rhs
        ) = delete;

        Cubemap( 
            Cubemap &&rhs
        ) = default;

        Cubemap &operator=( 
            Cubemap const &rhs
        ) = delete;

        Cubemap &operator=( 
            Cubemap &&rhs
        ) = default;

        /**
         * @brief Binds Cubemap to it's texture unit
        */
        virtual void bind(
        ) const;

        /**
         * @brief Unbinds Cubemap
        */
        static void unbind(
        );

        /**
         * @brief Getter for Cubemap texture value
         * @return Cubemap texture value
        */
        unsigned int value(
        ) const;

        /**
         * @brief Getter for Cubemap texture unit
         * @return Cubemap texture unit
        */
        GLenum unit(
        ) const;

        /**
         * @brief Getter for Cubemap texture folder path - only viable if Cubemap was loaded from folder
         * @return Cubemap texture folder path
        */
        std::string folderPath(
        ) const;

    };

} // OGL

#endif // OGL_CUBEMAP_H