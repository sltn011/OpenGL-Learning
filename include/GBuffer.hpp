/**
* @file GBuffer.hpp
*/

#ifndef OGL_GBUFFER_H
#define OGL_GBUFFER_H

#include "FrameBufferObject.hpp"
#include "Shader.hpp"

namespace OGL {

    class GBuffer {
    public:

        GBuffer(
            int width,
            int height
        );

        void bindAsRenderTarget(
        );

        void drawQuad(
            Shader &shader
        );

    private:

        void loadToShader(
            Shader &shader
        );

        FrameBufferObject m_fbo;

        int m_width;
        int m_height;

    public:

        enum Buffer {
            WorldPosition,
            Depth,
            AlbedoSpecular,
            Normal,
            MAX
        };

    };

} // OGL

#endif // OGL_GBUFFER_H
