#ifndef OGL_SHADOWCUBEMAP_H
#define OGL_SHADOWCUBEMAP_H

#include "Texture.hpp"

namespace OGL {

    class ShadowCubemap {
     protected:
        Texture m_texture;
        GLenum  m_textureUnit;

     public:
         ShadowCubemap(
             int size,
             GLenum textureUnit
         );

         void bind(
         ) const;

         static void unbind(
         );

         unsigned int value(
         ) const;

         GLenum unit(
         ) const;
    };

} // OGL

#endif // OGL_SHADOWCUBEMAP_H
