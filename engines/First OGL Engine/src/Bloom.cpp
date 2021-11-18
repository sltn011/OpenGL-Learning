#include "Bloom.hpp"

namespace OGL::E1 {

    Bloom::Bloom(
        Shader &&downsamplingShader,
        Shader &&horizontalBlurShader,
        Shader &&verticalBlurShader,
        Shader &&combineShader,
        glm::vec3 thresHold
    ) : m_downsamplingShader{ std::move(downsamplingShader) },
        m_horizontalBlurShader{ std::move(horizontalBlurShader) },
        m_verticalBlurShader{ std::move(verticalBlurShader) },
        m_combineShader{ std::move(combineShader) },
        m_thresHold{ thresHold },
        m_resultWidth{ 0 },
        m_resultHeight{ 0 } {

    }

    void Bloom::initFrameBuffers(
        int windowWidth,
        int windowHeight
    ) {
        initFramebuffer(m_result, windowWidth, windowHeight);
        initFramebuffer(m_temp, windowWidth, windowHeight);

        for (int i = 0; i < s_numMipmaps; ++i) {
            int fboWidth = static_cast<int>(windowWidth / std::pow(s_downscale, i + 1));
            int fboHeight = static_cast<int>(windowHeight / std::pow(s_downscale, i + 1));
            initFramebuffer(m_downsamples[i], fboWidth, fboHeight);
            initFramebuffer(m_intermediate[i], fboWidth, fboHeight);
        }

        m_resultWidth = windowWidth;
        m_resultHeight = windowHeight;
    }

    void Bloom::drawToResultFrameBuffer(
        FrameBufferObject &sceneFrameBuffer
    ) {
        glDisable(GL_DEPTH_TEST);

        blit(sceneFrameBuffer, m_result, m_resultWidth, m_resultHeight);

        setupDownsampleShader(true);
        resizeImage(
            m_result,
            m_downsamples[0],
            static_cast<int>(m_resultWidth / s_downscale),
            static_cast<int>(m_resultHeight / s_downscale)
        );
        blurImage(0, static_cast<int>(m_resultWidth / s_downscale), static_cast<int>(m_resultHeight / s_downscale));
        
        for (int i = 1; i < s_numMipmaps; ++i) {
            setupDownsampleShader(false);
            int sampleWidth = static_cast<int>(m_resultWidth / std::pow(s_downscale, i + 1));
            int sampleHeight = static_cast<int>(m_resultHeight / std::pow(s_downscale, i + 1));
            resizeImage(
                m_downsamples[i - 1],
                m_downsamples[i],
                sampleWidth,
                sampleHeight
            );

            blurImage(i, sampleWidth, sampleHeight);
        }

        setupCombineShader();
        for (int i = s_numMipmaps - 1; i > 0; --i) {
            int sampleWidth = static_cast<int>(m_resultWidth / std::pow(s_downscale, i));
            int sampleHeight = static_cast<int>(m_resultHeight / std::pow(s_downscale, i));
            combineImages(
                m_intermediate[i - 1],
                m_downsamples[i],
                m_downsamples[i - 1],
                sampleWidth,
                sampleHeight
            );
            blit(m_intermediate[i - 1], m_downsamples[i - 1], sampleWidth, sampleHeight);
        }
        combineImages(m_temp, m_downsamples[0], m_result, m_resultWidth, m_resultHeight);
        blit(m_temp, m_result, m_resultWidth, m_resultHeight);

        glEnable(GL_DEPTH_TEST);
    }

    void Bloom::bindResultFrameBuffer(
        GLenum frameBufferType
    ) {
        m_result.bind(frameBufferType);
    }

    void Bloom::initFramebuffer(
        FrameBufferObject &fbo,
        int width,
        int height
    ) {
        fbo.bind(GL_FRAMEBUFFER);
        ColorBufferObject cbo;
        cbo.allocateStorage(width, height, GL_TEXTURE_2D, GL_RGBA16F, GL_RGBA, GL_FLOAT);
        fbo.attachColorBuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, std::move(cbo));

        if (!fbo.isComplete(GL_FRAMEBUFFER)) {
            FrameBufferObject::unbind(GL_FRAMEBUFFER);
            throw Exception("Error creating FBO!");
        }

        FrameBufferObject::unbind(GL_FRAMEBUFFER);
    }

    void Bloom::blit(
        FrameBufferObject &from,
        FrameBufferObject &to,
        int width,
        int height
    ) {
        from.bind(GL_READ_FRAMEBUFFER);
        to.bind(GL_DRAW_FRAMEBUFFER);
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
        FrameBufferObject::unbind(GL_FRAMEBUFFER);
    }

    void Bloom::resizeImage(
        FrameBufferObject &from,
        FrameBufferObject &to,
        int newWidth,
        int newHeight
    ) {
        glViewport(0, 0, newWidth, newHeight);
        to.bind(GL_FRAMEBUFFER);
        from.drawQuad(GL_COLOR_ATTACHMENT0);
    }

    void Bloom::blurImage(
        int imageIndex,
        int imageWidth,
        int imageHeight
    ) {
        setupHorizontalBlurShader(imageWidth);
        m_intermediate[imageIndex].bind(GL_FRAMEBUFFER);
        m_downsamples[imageIndex].drawQuad(GL_COLOR_ATTACHMENT0);
        setupVerticalBlurShader(imageHeight);
        m_downsamples[imageIndex].bind(GL_FRAMEBUFFER);
        m_intermediate[imageIndex].drawQuad(GL_COLOR_ATTACHMENT0);
        FrameBufferObject::unbind(GL_FRAMEBUFFER);
    }

    void Bloom::combineImages(
        FrameBufferObject &dst,
        FrameBufferObject &fbo1,
        FrameBufferObject &fbo2,
        int dstWidth,
        int dstHeight
    ) {
        glViewport(0, 0, dstWidth, dstHeight);
        glActiveTexture(GL_TEXTURE0);
        fbo1.getColorBuffers().at(GL_COLOR_ATTACHMENT0).bindAsTexture(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE1);
        fbo2.getColorBuffers().at(GL_COLOR_ATTACHMENT0).bindAsTexture(GL_TEXTURE_2D);
        dst.bind(GL_FRAMEBUFFER);
        dst.drawQuadRaw();
        FrameBufferObject::unbind(GL_FRAMEBUFFER);
    }

    void Bloom::setupDownsampleShader(
        bool bDoCutoff
    ) {
        m_downsamplingShader.use();
        m_downsamplingShader.setUniformInt("fboTexture", 0);
        m_downsamplingShader.setUniformVec3("bloomThresHold", bDoCutoff ? m_thresHold : glm::vec3(0.0f));
    }

    void Bloom::setupHorizontalBlurShader(
        int imageWidth
    ) {
        m_horizontalBlurShader.use();
        m_horizontalBlurShader.setUniformInt("fboTexture", 0);
        m_horizontalBlurShader.setUniformInt("imageWidth", imageWidth);
    }

    void Bloom::setupVerticalBlurShader(
        int imageHeight
    ) {
        m_verticalBlurShader.use();
        m_verticalBlurShader.setUniformInt("fboTexture", 0);
        m_verticalBlurShader.setUniformInt("imageHeight", imageHeight);
    }

    void Bloom::setupCombineShader(
    ) {
        m_combineShader.use();
        m_combineShader.setUniformInt("fboTexture1", 0);
        m_combineShader.setUniformInt("fboTexture2", 1);
    }

} // OGL::E1