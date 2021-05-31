/**
* @file ShadowCubemap.hpp
*/

#ifndef OGL_SHADOWCUBEMAP_H
#define OGL_SHADOWCUBEMAP_H

#include "FrameBufferObject.hpp"
#include "Texture.hpp"
#include <array>

namespace OGL {

    /**
     * @brief Class that stores shadows created by PointLight
    */
    class ShadowCubemap {
     protected:
        /// FrameBufferObject that stores depth values
        FrameBufferObject m_fbo;

        /// Texture unit assigned to ShadowCubemap texture GL_TEXTURE{N}
        GLenum m_textureUnit;

        /// Size of ShadowCubemap
        int m_mapSize;

        /// Far clip plane
        float m_farPlane;

        /// 6 Projection*View matrices of CameraShadowCubemap looking at each side of ShadowCubemap
        std::array<glm::mat4, 6> m_lightProjViews;

     public:
         /**
          * @brief ShadowCubemap constructor
          * @param size Size of ShadowCubemap
          * @param textureUnit Texture unit assigned to ShadowCubemap texture GL_TEXTURE{N}
          * @param farCameraPlane Far clip plane of cameras that render ShadowCubemap
          * @param lightProjViewsMatrices 6 Projection*View matrices of CameraShadowCubemap looking at each side of ShadowCubemap
         */
         ShadowCubemap(
             int size,
             GLenum textureUnit,
             float farCameraPlane,
             std::array<glm::mat4, 6> const &lightProjViewsMatrices
         );

         ShadowCubemap(
             ShadowCubemap const &rhs
         ) = delete;

         ShadowCubemap(
             ShadowCubemap &&rhs
         ) = default;

         ShadowCubemap &operator=(
             ShadowCubemap const &rhs
         ) = delete;

         ShadowCubemap &operator=(
             ShadowCubemap &&rhs
         ) = default;

         /**
          * @brief Binds ShadowCubemap as FrameBufferObject GL_FRAMEBUFFER
         */
         void bindFramebuffer(
         ) const;

         /**
          * @brief Unbinds ShadowCubemap FrameBufferObject and sets FrameBufferObject GL_FRAMEBUFFER to default
         */
         static void unbindFramebuffer(
         );

         /**
          * @brief Binds ShadowCubemap as texture to its texture unit
         */
         void bindTexture(
         );

         /**
          * @brief Unbinds ShadowCubemap as texture
         */
         static void unbindTexture(
         );

         /**
          * @brief Get ShadowCubemap ID
          * @return ShadowCubemap ID
         */
         unsigned int value(
         );

         /**
          * @brief Get ShadowCubemap texture unit
          * @return ShadowCubemap texture unit
         */
         GLenum textureUnit(
         ) const;

         /**
          * @brief Get size of ShadowCubemap
          * @return Size of ShadowCubemap
         */
         int mapSize(
         ) const;

         /**
          * @brief Get ShadowCubemap far clip plane
          * @return ShadowCubemap far clip plane
         */
         float farPlane(
         ) const;

         /**
          * @brief Gets Projection*View matrix for specified side of ShadowCubemap
          * @param sideIndex ShadowCubemap side value (0-5) - (+X, -X, +Y, -Y, +Z, -Z) 
          * @return Projection*View matrix for specified side of ShadowCubemap
         */
         glm::mat4 lightProjView(
             int sideIndex
         ) const;
    };

} // OGL

#endif // OGL_SHADOWCUBEMAP_H
