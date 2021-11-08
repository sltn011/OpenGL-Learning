#include "Bloom.hpp"

namespace OGL::E1 {

    Bloom::Bloom(
        Shader &&mipmapShader,
        Shader &&combineShader,
        glm::vec3 thresHold
    ) : m_mipmapShader{ std::move(mipmapShader) },
        m_combineShader{ std::move(combineShader) },
        m_thresHold{ thresHold } {

    }

    void Bloom::initFrameBuffers(
        int windowWidth,
        int windowHeight
    ) {
        for (int i = 0; i < s_numMipmaps; ++i) {
            int mipmapWidth = static_cast<int>(windowWidth / std::pow(2, i));
            int mipmapHeight = static_cast<int>(windowHeight / std::pow(2, i));

            m_mipmaps[i] = FrameBufferObject{};
            m_mipmaps[i].bind(GL_FRAMEBUFFER);

            ColorBufferObject mipmapColorBuffer;

            mipmapColorBuffer.allocateStorage(mipmapWidth, mipmapHeight, GL_TEXTURE_2D, GL_RGBA16F, GL_RGBA, GL_FLOAT);
            m_mipmaps[i].attachColorBuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, std::move(mipmapColorBuffer));

            if (!m_mipmaps[i].isComplete(GL_FRAMEBUFFER)) {
                FrameBufferObject::unbind(GL_FRAMEBUFFER);
                throw Exception{ "Error creating bloom framebuffer!" };
            }
        }



        m_intermediateCombineBuffer = FrameBufferObject{};
        m_intermediateCombineBuffer.bind(GL_FRAMEBUFFER);

        ColorBufferObject intermediateColorBuffer;

        intermediateColorBuffer.allocateStorage(windowWidth, windowHeight, GL_TEXTURE_2D, GL_RGBA16F, GL_RGBA, GL_FLOAT);
        m_intermediateCombineBuffer.attachColorBuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, std::move(intermediateColorBuffer));

        if (!m_intermediateCombineBuffer.isComplete(GL_FRAMEBUFFER)) {
            FrameBufferObject::unbind(GL_FRAMEBUFFER);
            throw Exception{ "Error creating bloom framebuffer!" };
        }



        m_result = FrameBufferObject{};
        m_result.bind(GL_FRAMEBUFFER);

        ColorBufferObject renderColorBuffer;
        RenderBufferObject renderDepthBuffer;

        renderColorBuffer.allocateStorage(windowWidth, windowHeight, GL_TEXTURE_2D, GL_RGBA16F, GL_RGBA, GL_FLOAT);
        m_result.attachColorBuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, std::move(renderColorBuffer));

        if (!m_result.isComplete(GL_FRAMEBUFFER)) {
            FrameBufferObject::unbind(GL_FRAMEBUFFER);
            throw Exception{ "Error creating bloom framebuffer!" };
        }

        FrameBufferObject::unbind(GL_FRAMEBUFFER);

        m_resultWidth = windowWidth;
        m_resultHeight = windowHeight;
    }

    void Bloom::drawToResultFrameBuffer(
        FrameBufferObject &sceneFrameBuffer
    ) {
        glDisable(GL_DEPTH_TEST);

        sceneFrameBuffer.bind(GL_READ_FRAMEBUFFER);
        m_result.bind(GL_DRAW_FRAMEBUFFER);
        glBlitFramebuffer(0, 0, m_resultWidth, m_resultHeight, 0, 0, m_resultWidth, m_resultHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        if (s_numMipmaps < 2) {
            return;
        }

        m_mipmapShader.use();
        m_mipmapShader.setUniformInt("fboTexture", 0);
        m_mipmapShader.setUniformVec3("thresHold", m_thresHold);

        m_mipmaps[0].bind(GL_FRAMEBUFFER);
        m_result.drawQuad(GL_COLOR_ATTACHMENT0);
        for (int i = 1; i < s_numMipmaps; ++i) {
            glViewport(0, 0, static_cast<int>(m_resultWidth / std::pow(2, i)), static_cast<int>(m_resultHeight / std::pow(2, i)));
            m_mipmaps[i].bind(GL_FRAMEBUFFER);
            m_mipmaps[i - 1].drawQuad(GL_COLOR_ATTACHMENT0);
        }

        glViewport(0, 0, m_resultWidth, m_resultHeight);

        m_combineShader.use();
        m_combineShader.setUniformInt("fboTexture", 0);
        m_combineShader.setUniformInt("resultTexture", 1);
        glActiveTexture(GL_TEXTURE1);
        m_result.getColorBuffers().at(GL_COLOR_ATTACHMENT0).bindAsTexture(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        for (int i = 0; i < s_numMipmaps; ++i) {
            m_mipmaps[i].bind(GL_READ_FRAMEBUFFER);
            m_intermediateCombineBuffer.bind(GL_DRAW_FRAMEBUFFER);
            glBlitFramebuffer(
                0, 0, static_cast<int>(m_resultWidth / std::pow(2, i)), static_cast<int>(m_resultHeight / std::pow(2, i)),
                0, 0, m_resultWidth, m_resultHeight, 
                GL_COLOR_BUFFER_BIT,
                GL_LINEAR
            );
            m_result.bind(GL_FRAMEBUFFER);
            m_intermediateCombineBuffer.drawQuad(GL_COLOR_ATTACHMENT0);
        }

        FrameBufferObject::unbind(GL_FRAMEBUFFER);

        glEnable(GL_DEPTH_TEST);
    }

    void Bloom::bindResultFrameBuffer(
        GLenum frameBufferType
    ) {
        m_result.bind(frameBufferType);
    }

} // OGL::E1