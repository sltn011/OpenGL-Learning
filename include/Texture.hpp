#ifndef OGL_TEXTURE_H
#define OGL_TEXTURE_H

#include "glad/glad.h"
#include "Descriptor.hpp"
#include "Exception.hpp"

#include "stb_image.h"

#include <string>

namespace OGL {

    class Texture : public Descriptor {
     public:
        Texture(
        );

        Texture( 
            std::string const &folderPath,
            std::string const &fileName,
            GLenum textureType, 
            bool gamma = false
        );

        ~Texture
        (
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

        static void allocate( 
            GLenum textureType,
            int width,
            int height,
            GLenum format,
            void *data = nullptr
        );

        void load( 
            std::string const &folderPath,
            std::string const &fileName,
            GLenum textureType, 
            bool gamma = false
        );

        static void setActive(
            GLenum textureUnit
        );

        void bind(
            GLenum textureType
        ) const;

        static void unbind(
            GLenum textureType
        );

        static void setParameter( 
            GLenum textureType, 
            GLenum paramName, 
            GLint paramValue
        );

        static void generateMipmap( 
            GLenum textureType
        );

    };

} // OGL

#endif // OGL_TEXTURE_H