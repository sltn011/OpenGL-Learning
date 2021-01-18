#ifndef OGL_CUBEMAP_H
#define OGL_CUBEMAP_H

#include "glad/glad.h"
#include "Texture.hpp"

#include <string>

namespace OGL {

    class Cubemap {
    protected:
        Texture m_texture;
        GLenum  m_textureUnit;

    public:
        Cubemap
        (
        );

        Cubemap
        ( int size
        , GLenum cubemapTextureUnit
        );

        Cubemap
        ( std::string const &folderPath
        , GLenum cubemapTextureUnit
        );

        Cubemap
        ( Cubemap const &rhs
        ) = delete;

        Cubemap
        ( Cubemap &&rhs
        ) = default;

        Cubemap &operator=
        ( Cubemap const &rhs
        ) = delete;

        Cubemap &operator=
        ( Cubemap &&rhs
        ) = default;

        virtual void bind
        (
        ) const;

        static void unbind
        (
        );

        unsigned int value
        (
        ) const;

        GLenum unit
        (
        ) const;

    };

} // OGL

#endif // OGL_CUBEMAP_H