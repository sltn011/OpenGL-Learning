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
        GLenum m_bufferType;

        VertexArrayObject m_frameQuadVAO;
        VertexBufferObject m_frameQuadVBO;

        std::map<GLenum, ColorBufferObject> m_colorBufferObjs;
        RenderBufferObject m_renderBufferObj;

        std::map<GLenum, unsigned int> m_colorAttachmentsTable;

     public:
        static float const frameQuadData[];

     public:
        FrameBufferObject( 
            GLenum bufferType
        );

        FrameBufferObject( 
            GLenum bufferType, 
            float const *frameBufferQuadData,
            size_t frameBufferQuadDataLength,
            GLuint frameQuadVerticesVertexAttribIndex = 0,
            GLuint frameQuadTexCoordVertexAttribIndex = 1
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

        void attach( 
            GLenum colorAttachment,
            GLenum targetTexture,
            ColorBufferObject &&obj
        );

        void attach( 
            GLenum attachment,
            RenderBufferObject &&obj
        );

        void bind(
        ) const;

        static void unbind( 
            GLenum bufferType
        );

        GLenum checkStatus(
        ) const;

        bool isComplete(
        ) const;

        void drawQuad( 
            GLenum colorAttachment
        );

        GLenum bufferType(
        ) const;
    };

} // OGL

#endif // OGL_FBO_H
