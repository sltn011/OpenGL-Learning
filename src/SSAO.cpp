#include "SSAO.hpp"

namespace OGL {

    SSAO::SSAO(
        int width,
        int height
    ) : m_randomFloat(0.0f, 1.0f) {
        m_fbo.bind(GL_FRAMEBUFFER);

        ColorBufferObject texBuffer;
        texBuffer.allocateStorage(width, height, GL_TEXTURE_2D, GL_R16F, GL_RED, GL_FLOAT);
        m_fbo.attachColorBuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, std::move(texBuffer));

        if (!m_fbo.isComplete(GL_FRAMEBUFFER)) {
            FrameBufferObject::unbind(GL_FRAMEBUFFER);
            throw Exception("Error creating SSAO framebuffer!");
        }

        FrameBufferObject::unbind(GL_FRAMEBUFFER);

        createSamples();
        createRandomNoiseRotation();

        m_bufferWidth = width;
        m_bufferHeight = height;
    }

    void SSAO::bindAsShaderOutput(
        Shader &shader,
        GLenum samplesTextureID,
        GLenum noiseTextureID
    ) {
        m_fbo.bind(GL_FRAMEBUFFER);

        glActiveTexture(samplesTextureID);
        m_hemisphereSamples.bind(GL_TEXTURE_1D);
        glActiveTexture(GL_TEXTURE0);
        
        glActiveTexture(noiseTextureID);
        m_randomNoiseRotation.bind(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);

        shader.setUniformInt("imageWidth", m_bufferWidth);
        shader.setUniformInt("imageHeight", m_bufferHeight);
        shader.setUniformInt("samples", samplesTextureID - GL_TEXTURE0);
        shader.setUniformInt("numSamples", s_numSamples);
        shader.setUniformInt("noiseRotation", noiseTextureID - GL_TEXTURE0);
    }

    void SSAO::bindAsShaderInput(
        Shader &shader,
        GLenum textureID
    ) {
        shader.setUniformInt("gSSAO", textureID - GL_TEXTURE0);
        
        glActiveTexture(textureID);
        m_fbo.getColorBuffers().at(GL_COLOR_ATTACHMENT0).bindAsTexture(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
    }

    FrameBufferObject &SSAO::getBuffer(
    ) {
        return m_fbo;
    }

    void SSAO::createSamples(
    ) {
        glm::vec3 samples[s_numSamples];
        for (size_t i = 0; i < s_numSamples; ++i) {
            samples[i].x = m_randomFloat(m_generator) * 2.0f - 1.0f;
            samples[i].y = m_randomFloat(m_generator) * 2.0f - 1.0f;
            samples[i].z = m_randomFloat(m_generator) * 2.0f - 1.0f;
            //samples[i].w = 0.0f;
            
            samples[i] = glm::normalize(samples[i]);
            samples[i] *= m_randomFloat(m_generator);
            float scale = static_cast<float>(i) / s_numSamples;
            scale = lerpSampleScale(0.1f, 1.0f, scale * scale);
            samples[i] *= scale;
        }

        m_hemisphereSamples.bind(GL_TEXTURE_1D);
        m_hemisphereSamples.allocate1D(GL_TEXTURE_1D, s_numSamples, GL_RGBA16F, GL_RGB, GL_FLOAT, samples);
        m_hemisphereSamples.setParameter(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        Texture::unbind(GL_TEXTURE_1D);
    }

    void SSAO::createRandomNoiseRotation(
    ) {
        glm::vec3 noise[16];
        for (size_t i = 0; i < 16; ++i) {
            noise[i].x = m_randomFloat(m_generator) * 2.0f - 1.0f;
            noise[i].y = m_randomFloat(m_generator) * 2.0f - 1.0f;
            noise[i].z = 0.0f;
            //noise[i].w = 0.0f;
        }

        m_randomNoiseRotation.bind(GL_TEXTURE_2D);
        m_randomNoiseRotation.allocate(GL_TEXTURE_2D, 4, 4, GL_RGBA16F, GL_RGB, GL_FLOAT, &noise);
        m_randomNoiseRotation.setParameter(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        m_randomNoiseRotation.setParameter(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        m_randomNoiseRotation.setParameter(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        m_randomNoiseRotation.setParameter(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        Texture::unbind(GL_TEXTURE_2D);
    }

    float SSAO::lerpSampleScale(
        float a,
        float b,
        float scale
    ) {
        return a + scale * (b - a);
    }

} // OGL