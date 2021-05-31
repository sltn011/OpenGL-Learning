#include "ShadowCubemap.hpp"

namespace OGL {

    ShadowCubemap::ShadowCubemap(
        int size,
        GLenum cubemapTextureUnit,
        float farCameraPlane,
        std::array<glm::mat4, 6> const &lightProjViewsMatrices
    ) : m_textureUnit{ cubemapTextureUnit },
        m_mapSize{ size },
        m_farPlane{ farCameraPlane },
        m_lightProjViews{ lightProjViewsMatrices } {

        ColorBufferObject cubemap;
        Texture::setActive(m_textureUnit);
        cubemap.bindAsTexture(GL_TEXTURE_CUBE_MAP);

        for (int i = 0; i < 6; ++i) {
            cubemap.allocateStorage(m_mapSize, m_mapSize, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
        }

        Texture::setParameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        Texture::setParameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        Texture::setParameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        Texture::setParameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        Texture::setParameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        Texture::unbind(GL_TEXTURE_CUBE_MAP);
        Texture::setActive(GL_TEXTURE0);

        m_fbo.bind(GL_FRAMEBUFFER);
        m_fbo.attachColorBuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, std::move(cubemap));
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        FrameBufferObject::unbind(GL_FRAMEBUFFER);
    }

    void ShadowCubemap::bindFramebuffer(
    ) const {
        m_fbo.bind(GL_FRAMEBUFFER);
    }

    void ShadowCubemap::unbindFramebuffer(
    ) {
        FrameBufferObject::unbind(GL_FRAMEBUFFER);
    }

    void ShadowCubemap::bindTexture(
    ) {
        Texture::setActive(m_textureUnit);
        Texture::bind(GL_TEXTURE_CUBE_MAP, value());
    }

    void ShadowCubemap::unbindTexture(
    ) {
        Texture::unbind(GL_TEXTURE_CUBE_MAP);
        Texture::setActive(GL_TEXTURE0);
    }

    unsigned int ShadowCubemap::value(
    ) {
        return m_fbo.getColorBuffers()[GL_DEPTH_ATTACHMENT].value();
    }

    GLenum ShadowCubemap::textureUnit(
    ) const {
        return m_textureUnit;
    }

    int ShadowCubemap::mapSize(
    ) const {
        return m_mapSize;
    }

    float ShadowCubemap::farPlane(
    ) const {
        return m_farPlane;
    }

    glm::mat4 ShadowCubemap::lightProjView(
        int sideIndex
    ) const {
        return m_lightProjViews.at(sideIndex);
    }

}