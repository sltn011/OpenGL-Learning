#ifndef OGL_SHADOWMAP_H
#define OGL_SHADOWMAP_H

#include "FrameBufferObject.hpp"

namespace OGL {

    class ShadowMap {
        FrameBufferObject m_fbo;
        GLenum m_mapTextureUnit;
        int m_mapSize;
        glm::mat4 m_lightProjView;

     public:
         ShadowMap(
             GLenum mapTextureUnit,
             int mapSize,
             glm::mat4 lightProjViewMatrix
         );

         ShadowMap(
             ShadowMap const &rhs
         ) = delete;

         ShadowMap(
             ShadowMap &&rhs
         ) = default;

         ShadowMap &operator=(
             ShadowMap const &rhs
         ) = delete;

         ShadowMap &operator=(
             ShadowMap &&rhs
         ) = default;

         void bindFramebuffer(
         ) const;

         static void unbindFramebuffer(
         );

         void bindTexture(
         );

         static void unbindTexture(
         );

         unsigned int textureValue(
         );

         GLenum textureUnit(
         ) const;

         int mapSize(
         ) const;

         glm::mat4 lightProjView(
         ) const;
    };

}

#endif // OGL_SHADOWMAP_H
