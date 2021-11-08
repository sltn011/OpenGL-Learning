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
            Shader &&mipmapShader,
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

        static constexpr int s_numMipmaps = 4;

        int m_resultWidth;
        int m_resultHeight;

        Shader m_mipmapShader;
        Shader m_combineShader;

        FrameBufferObject m_mipmaps[s_numMipmaps];

        FrameBufferObject m_intermediateCombineBuffer;

        FrameBufferObject m_result;

        glm::vec3 m_thresHold;
    };

} // OGL::E1

#endif // OGL_E1_BLOOM_H
