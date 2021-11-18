/**
* @file SSAO.hpp
*/

#ifndef OGL_SSAO_H
#define OGL_SSAO_H

#include "GBuffer.hpp"
#include "Shader.hpp"

namespace OGL {

    class SSAO {
    public:

        SSAO(
            int width,
            int height
        );

        void bindAsRenderTarget(
        );

        void bindAsShaderInput(
            Shader &shader,
            GLenum textureID
        );

    private:

        void loadToShader(
            Shader &shader,
            GLenum textureID
        );

        FrameBufferObject m_fbo;

    };

} // OGL

#endif // OGL_SSAO_H
