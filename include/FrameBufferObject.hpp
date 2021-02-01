#ifndef OGL_FBO_H
#define OGL_FBO_H

#include "glad/glad.h"
#include "Descriptor.hpp"
#include "ColorBufferObject.hpp"
#include "RenderBufferObject.hpp"
#include "VertexArrayObject.hpp"
#include "VertexBufferObject.hpp"
#include "Shader.hpp"
#include <map>

namespace OGL {

    class FrameBufferObject : public Descriptor {
     protected:

        VertexArrayObject m_frameQuadVAO;
        VertexBufferObject m_frameQuadVBO;

        std::map<GLenum, ColorBufferObject> m_colorBufferObjs;
        RenderBufferObject m_renderBufferObj;

     protected:
        static float const frameQuadData[];

     public:
        FrameBufferObject( 
        );

        FrameBufferObject(
            GLuint frameQuadVerticesVertexAttribIndex,
            GLuint frameQuadTexCoordVertexAttribIndex
        );

        ~FrameBufferObject(
        );

        FrameBufferObject(
            FrameBufferObject const &rhs
        ) = delete;

        FrameBufferObject(
            FrameBufferObject &&rhs
        ) = default;

        FrameBufferObject &operator=( 
            FrameBufferObject const &rhs
        ) = delete;

        FrameBufferObject &operator=(
            FrameBufferObject &&rhs
        ) = default;

        void attachColorBuffer( 
            GLenum framebufferType,
            GLenum colorAttachment,
            GLenum targetTexture,
            ColorBufferObject &&obj
        );

        void attachColorBufferMultisample(
            GLenum framebufferType,
            GLenum colorAttachment,
            ColorBufferObject &&obj
        );

        void attachRenderBuffer( 
            GLenum framebufferType,
            GLenum attachment,
            RenderBufferObject &&obj
        );

        void attachRenderBufferMultisample(
            GLenum framebufferType,
            GLenum attachment,
            RenderBufferObject &&obj
        );

        void bind(
            GLenum framebufferType
        ) const;

        static void unbind( 
            GLenum framebufferType
        );

        static GLenum checkStatus(
            GLenum framebufferType
        );

        static bool isComplete(
            GLenum framebufferType
        );

        void drawQuad(
            GLenum colorAttachment
        );

        std::map<GLenum, ColorBufferObject> &getColorBuffers(
        );

        RenderBufferObject &getRenderBuffer(
        );
    };

} // OGL

#endif // OGL_FBO_H
