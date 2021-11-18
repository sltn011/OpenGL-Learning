/**
* @file Texture.hpp
*/

#ifndef OGL_TEXTURE_H
#define OGL_TEXTURE_H

#include "glad/glad.h"
#include "Descriptor.hpp"
#include "Exception.hpp"

#include "stb_image.h"

#include <string>

namespace OGL {

    /**
     * @brief Class for storing image data and assign them to OpenGL textures
    */
    class Texture : public Descriptor {
     public:

        /**
         * @brief Empty Texture constructor
        */
        Texture(
        );

        /**
         * @brief Texture constructor with loading from file
         * @param folderPath Path to folder with texture image
         * @param fileName Image name inside folderPath folder
         * @param textureType Type of OpenGL texture - GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X, etc
        */
        Texture( 
            std::string const &folderPath,
            std::string const &fileName,
            GLenum textureType
        );

        /**
         * @brief Texture destructor
        */
        ~Texture(
        );

        Texture( 
            Texture const &rhs
        ) = delete;

        Texture( 
            Texture &&rhs
        ) = default;

        Texture &operator=( 
            Texture const &rhs
        ) = delete;

        Texture &operator=( 
            Texture &&rhs
        ) = default;

        /**
         * @brief Allocates memory for Texture buffer
         * @param textureType Type of OpenGL texture - GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X, etc
         * @param width Width of buffer
         * @param height Height of buffer
         * @param format Format of pixel data - GL_RED, GL_RG, GL_RGB, etc
         * @param type Data type of pixel data - GL_UNSIGNED_BYTE, GL_INT, GL_FLOAT, etc
         * @param data Pointer to data used to fill allocated memory - nullptr for empty buffer
        */
        static void allocate( 
            GLenum textureType,
            int width,
            int height,
            GLenum format,
            GLenum type,
            void *data
        );

        static void allocate(
            GLenum textureType,
            int width,
            int height,
            GLenum internalFormat,
            GLenum format,
            GLenum type,
            void *data
        );

        /**
         * @brief Loads texture from file
         * @param folderPath Path to folder with texture image
         * @param fileName Image name inside folderPath folder 
         * @param textureType Type of OpenGL texture - GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X, etc
        */
        void load( 
            std::string const &folderPath,
            std::string const &fileName,
            GLenum textureType
        );

        /**
         * @brief Sets supplied texture unit as active
         * @param textureUnit GL_TEXTURE{N} texture unit
        */
        static void setActive(
            GLenum textureUnit
        );

        /**
         * @brief Binds texture to it's texture unit
         * @param textureType Type of OpenGL texture - GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X, etc
        */
        void bind(
            GLenum textureType
        ) const;

        /**
         * @brief Binds texture id to texture type
         * @param textureType Type of OpenGL texture - GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X, etc
         * @param texture ID of texture
        */
        static void bind(
            GLenum textureType,
            int texture
        );

        /**
         * @brief Unbinds texture
         * @param textureType Type of OpenGL texture - GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X, etc
        */
        static void unbind(
            GLenum textureType
        );

        /**
         * @brief Sets OpenGL texture parameter
         * @param textureType Type of OpenGL texture - GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X, etc
         * @param paramName Name of parameter - GL_TEXTURE_MAG_FILTER, GL_TEXTURE_WRAP_S, etc
         * @param paramValue Value of parameter - GL_LINEAR, GL_CLAMP_TO_EDGE, etc
        */
        static void setParameter( 
            GLenum textureType, 
            GLenum paramName, 
            GLint paramValue
        );

        /**
         * @brief Generates mipmaps for specified texture
         * @param textureType Type of OpenGL texture - GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X, etc
        */
        static void generateMipmap( 
            GLenum textureType
        );

    };

} // OGL

#endif // OGL_TEXTURE_H