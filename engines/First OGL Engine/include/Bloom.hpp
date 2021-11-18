/**
* @file Bloom.hpp
*/

#ifndef OGL_E1_BLOOM_H
#define OGL_E1_BLOOM_H

#include "FrameBufferObject.hpp"
#include "Shader.hpp"

namespace OGL::E1 {

    class Bloom {
    public:

        Bloom(
            Shader &&downsamplingShader,
            Shader &&horizontalBlurShader,
            Shader &&verticalBlurShader,
            Shader &&combineShader,
            glm::vec3 thresHold
        );

        Bloom(
            Bloom const &rhs
        ) = delete;

        Bloom(
            Bloom &&rhs
        ) = default;

        Bloom &operator=(
            Bloom const &rhs
        ) = delete;

        Bloom &operator=(
            Bloom &&rhs
        ) = default;

        void initFrameBuffers(
            int windowWidth,
            int windowHeight
        );

        void drawToResultFrameBuffer(
            FrameBufferObject &sceneFrameBuffer
        );

        void bindResultFrameBuffer(
            GLenum frameBufferType
        );

    private:

        void initFramebuffer(
            FrameBufferObject &fbo,
            int width,
            int height
        );

        void blit(
            FrameBufferObject &from,
            FrameBufferObject &to,
            int width,
            int height
        );

        void resizeImage(
            FrameBufferObject &from,
            FrameBufferObject &to,
            int newWidth,
            int newHeight
        );

        void blurImage(
            int imageIndex,
            int imageWidth,
            int imageHeight
        );

        void combineImages(
            FrameBufferObject &dst,
            FrameBufferObject &fbo1,
            FrameBufferObject &fbo2,
            int dstWidth,
            int dstHeight
        );

        void setupDownsampleShader(
            bool bDoCutoff
        );

        void setupHorizontalBlurShader(
            int imageWidth
        );

        void setupVerticalBlurShader(
            int imageHeight
        );

        void setupCombineShader(
        );

        static constexpr int s_numMipmaps = 7;
        static constexpr int s_downscale = 2;

        int m_resultWidth;
        int m_resultHeight;

        Shader m_downsamplingShader;
        Shader m_horizontalBlurShader;
        Shader m_verticalBlurShader;
        Shader m_combineShader;

        FrameBufferObject m_downsamples[s_numMipmaps];
        FrameBufferObject m_intermediate[s_numMipmaps];
        FrameBufferObject m_temp;
        FrameBufferObject m_result;

        glm::vec3 m_thresHold;
    };

} // OGL::E1

#endif // OGL_E1_BLOOM_H
