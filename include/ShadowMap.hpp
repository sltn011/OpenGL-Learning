/**
* @file ShadowMap.hpp
*/

#ifndef OGL_SHADOWMAP_H
#define OGL_SHADOWMAP_H

#include "FrameBufferObject.hpp"

namespace OGL {

    /**
     * @brief Class that stores shadows created by DirectionalLight or SpotLight
    */
    class ShadowMap {
        /// FrameBufferObject that stores depth values
        FrameBufferObject m_fbo;

        /// Texture unit assigned to ShadowMap texture GL_TEXTURE{N}
        GLenum m_mapTextureUnit;

        /// Size of ShadowMap
        int m_mapSize;

        /// Projection*View matrix of CameraShadowMap used to render ShadowMap
        glm::mat4 m_lightProjView;

     public:
         /**
          * @brief ShadowMap constructor
          * @param mapTextureUnit Texture unit assigned to ShadowCubemap texture GL_TEXTURE{N}
          * @param mapSize Size of ShadowMap
          * @param lightProjViewMatrix Projection*View matrix of CameraShadowMap used to render ShadowMap
         */
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

         /**
          * @brief Binds ShadowMap as FrameBufferObject GL_FRAMEBUFFER
         */
         void bindFramebuffer(
         ) const;

         /**
          * @brief Unbinds ShadowMap FrameBufferObject and sets FrameBufferObject GL_FRAMEBUFFER to default
         */
         static void unbindFramebuffer(
         );

         /**
          * @brief Binds ShadowMap as texture to its texture unit
         */
         void bindTexture(
         );

         /**
          * @brief Unbinds ShadowMap as texture
         */
         static void unbindTexture(
         );

         /**
          * @brief Get ShadowMap ID
          * @return ShadowMap ID
         */
         unsigned int value(
         );

         /**
          * @brief Get ShadowMap texture unit
          * @return ShadowMap texture unit
         */
         GLenum textureUnit(
         ) const;

         /**
          * @brief Get size of ShadowMap
          * @return Size of ShadowMap
         */
         int mapSize(
         ) const;

         /**
          * @brief Get Projection*View matrix of CameraShadowMap used to render ShadowMap
          * @return Projection*View matrix of CameraShadowMap used to render ShadowMap
         */
         glm::mat4 lightProjView(
         ) const;
    };

}

#endif // OGL_SHADOWMAP_H
