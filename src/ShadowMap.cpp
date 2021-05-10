#include "ShadowMap.hpp"

namespace OGL {

    ShadowMap::ShadowMap(
        GLenum mapTextureUnit, 
        int mapSize,
        glm::mat4 lightProjViewMatrix
    ) : m_mapTextureUnit{ mapTextureUnit },
        m_mapSize{ mapSize },
        m_lightProjView{ lightProjViewMatrix } {
        ColorBufferObject map;
        map.allocateStorage(m_mapSize, m_mapSize, GL_TEXTURE_2D, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
        bindFramebuffer();
        m_fbo.attachColorBuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, std::move(map));
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        ShadowMap::unbindFramebuffer();

        bindTexture();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        ShadowMap::unbindTexture();
    }

    void ShadowMap::bindFramebuffer(
    ) const {
        m_fbo.bind(GL_FRAMEBUFFER);
    }

    void ShadowMap::unbindFramebuffer() {
        FrameBufferObject::unbind(GL_FRAMEBUFFER);
    }

    void ShadowMap::bindTexture() {
        glActiveTexture(m_mapTextureUnit);
        glBindTexture(GL_TEXTURE_2D, value());
    }

    void ShadowMap::unbindTexture() {
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE0);
    }

    unsigned int ShadowMap::value(
    ) {
        return m_fbo.getColorBuffers()[GL_DEPTH_ATTACHMENT].value();
    }

    GLenum ShadowMap::textureUnit(
    ) const {
        return m_mapTextureUnit;
    }

    int ShadowMap::mapSize(
    ) const {
        return m_mapSize;
    }

    glm::mat4 ShadowMap::lightProjView(
    ) const {
        return m_lightProjView;
    }

} // OGL