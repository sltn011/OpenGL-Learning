#include "GBuffer.hpp"

namespace OGL {

    GBuffer::GBuffer(
        int width,
        int height
    ) : m_width{ width },
        m_height{ height } {

        m_fbo.bind(GL_FRAMEBUFFER);

        for (int i = 0; i < Buffer::MAX; ++i) {
            ColorBufferObject texBuffer;

            GLint internalFormat;
            GLenum format;
            GLenum type;

            switch (i) {
            case Buffer::WorldPosition:
                internalFormat = GL_RGBA16F;
                format = GL_RGBA;
                type = GL_FLOAT;
                break;

            case Buffer::Depth:
                internalFormat = GL_R32F;
                format = GL_RED;
                type = GL_FLOAT;
                break;

            case Buffer::AlbedoSpecular:
                internalFormat = GL_RGBA;
                format = GL_RGBA;
                type = GL_UNSIGNED_BYTE;
                break;

            case Buffer::Normal:
                internalFormat = GL_RGBA16F;
                format = GL_RGBA;
                type = GL_FLOAT;
                break;

            default:
                continue;
            }

            texBuffer.allocateStorage(m_width, m_height, GL_TEXTURE_2D, internalFormat, format, type);
            m_fbo.attachColorBuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, std::move(texBuffer));
        }

        RenderBufferObject depthBuffer;
        depthBuffer.allocateStorage(m_width, m_height, GL_DEPTH_COMPONENT32F);
        m_fbo.attachRenderBuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, std::move(depthBuffer));

        if (!m_fbo.isComplete(GL_FRAMEBUFFER)) {
            GLenum res = m_fbo.checkStatus(GL_FRAMEBUFFER);

            FrameBufferObject::unbind(GL_FRAMEBUFFER);
            throw Exception("Error creating GBuffer!");
        }

        FrameBufferObject::unbind(GL_FRAMEBUFFER);
    }

    void GBuffer::bindAsRenderTarget(
    ) {
        m_fbo.bind(GL_FRAMEBUFFER);

        GLenum targetBuffers[Buffer::MAX];
        for (int i = 0; i < Buffer::MAX; ++i) {
            targetBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
        }

        glDrawBuffers(Buffer::MAX, targetBuffers);
    }

    void GBuffer::bindAsShaderInput(
        Shader &shader
    ) {
        loadToShader(shader);

        for (int i = 0; i < Buffer::MAX; ++i) {
            glActiveTexture(GL_TEXTURE0 + i);
            m_fbo.getColorBuffers().at(GL_COLOR_ATTACHMENT0 + i).bindAsTexture(GL_TEXTURE_2D);
        }

        glActiveTexture(GL_TEXTURE0);
    }

    void GBuffer::drawQuad(
        Shader &shader
    ) {
        bindAsShaderInput(shader);

        m_fbo.drawQuadRaw();
    }

    void GBuffer::loadToShader(
        Shader &shader
    ) {
        shader.setUniformInt("gWorldPosition", Buffer::WorldPosition);
        shader.setUniformInt("gDepth", Buffer::Depth);
        shader.setUniformInt("gAlbedoSpecular", Buffer::AlbedoSpecular);
        shader.setUniformInt("gNormal", Buffer::Normal);
    }

} // OGL