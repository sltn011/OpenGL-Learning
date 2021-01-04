#ifndef OGL_CUBEMAP_H
#define OGL_CUBEMAP_H

#include "glad/glad.h"
#include "Texture.hpp"

#include <string>

namespace OGL {

    class Cubemap {
    protected:
        Texture m_texture;

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

        unsigned int value
        (
        ) const;

    };

} // OGL

#endif // OGL_CUBEMAP_H