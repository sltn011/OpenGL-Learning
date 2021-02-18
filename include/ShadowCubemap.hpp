#ifndef OGL_SHADOWCUBEMAP_H
#define OGL_SHADOWCUBEMAP_H

#include "FramebufferObject.hpp"
#include "Texture.hpp"
#include <array>

namespace OGL {

    class ShadowCubemap {
     protected:
        FrameBufferObject m_fbo;
        GLenum m_textureUnit;
        int m_mapSize;
        float m_farPlane;
        std::array<glm::mat4, 6> m_lightProjs;

     public:
         ShadowCubemap(
             int size,
             GLenum textureUnit,
             float farCameraPlane,
             std::array<glm::mat4, 6> const &lightProjMatrices
         );

         void bindFramebuffer(
         ) const;

         static void unbindFramebuffer(
         );

         void bindTexture(
         );

         static void unbindTexture(
         );

         unsigned int value(
         );

         GLenum textureUnit(
         ) const;

         float farPlane(
         ) const;

         glm::mat4 lightProjView(
             int sideIndex
         ) const;
    };

} // OGL

#endif // OGL_SHADOWCUBEMAP_H
