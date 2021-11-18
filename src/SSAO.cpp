#include "SSAO.hpp"

namespace OGL {

    SSAO::SSAO(
        int width,
        int height
    ) {
        m_fbo.bind(GL_FRAMEBUFFER);

        ColorBufferObject texBuffer;
        texBuffer.allocateStorage(width, height, GL_TEXTURE_2D, GL_R32F, GL_RED, GL_FLOAT);
        m_fbo.attachColorBuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, std::move(texBuffer));

        if (!m_fbo.isComplete(GL_FRAMEBUFFER)) {
            FrameBufferObject::unbind(GL_FRAMEBUFFER);
            throw Exception("Error creating SSAO framebuffer!");
        }

        FrameBufferObject::unbind(GL_FRAMEBUFFER);
    }

    void SSAO::bindAsRenderTarget(
    ) {
        m_fbo.bind(GL_FRAMEBUFFER);
    }

    void SSAO::bindAsShaderInput(
        Shader &shader,
        GLenum textureID
    ) {
        loadToShader(shader, textureID);
        
        glActiveTexture(textureID);
        m_fbo.getColorBuffers().at(GL_COLOR_ATTACHMENT0).bindAsTexture(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
    }

    void SSAO::loadToShader(
        Shader &shader,
        GLenum textureID
    ) {
        shader.setUniformInt("gSSAO", textureID - GL_TEXTURE0);
    }

} // OGL