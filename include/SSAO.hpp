/**
* @file SSAO.hpp
*/

#ifndef OGL_SSAO_H
#define OGL_SSAO_H

#include "GBuffer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include <random>
#include <vector>

namespace OGL {

    class SSAO {
    public:

        SSAO(
            int width,
            int height
        );

        void bindAsShaderOutput(
            Shader &shader,
            GLenum samplesTextureID,
            GLenum noiseTextureID
        );

        void bindAsShaderInput(
            Shader &shader,
            GLenum textureID
        );

        FrameBufferObject &getBuffer(
        );

        void createSamples(
        );

        void createRandomNoiseRotation(
        );

    private:

        float lerpSampleScale(
            float a,
            float b,
            float scale
        );

        FrameBufferObject m_fbo;

        static constexpr size_t s_numSamples = 64;

        Texture m_hemisphereSamples;
        Texture m_randomNoiseRotation;

        std::normal_distribution<float> m_randomFloat;
        std::default_random_engine m_generator;

        int m_bufferWidth;
        int m_bufferHeight;

    };

} // OGL

#endif // OGL_SSAO_H
