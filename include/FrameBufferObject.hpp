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

        std::map<GLenum, unsigned int> m_colorAttachmentsTable;

    public:
        static float const frameQuadData[];

    public:
        FrameBufferObject
        ( GLenum bufferType
        , float const *frameBufferQuadData
        , size_t frameBufferQuadDataLength
        , GLuint frameQuadVerticesVertexAttribIndex = 0
        , GLuint frameQuadTexCoordVertexAttribIndex = 1
        );

        ~FrameBufferObject
        (
        );

        void attach
        ( GLenum colorAttachment
        , GLenum targetTexture
        , ColorBufferObject const &obj
        );

        void attach
        ( GLenum attachment
        , RenderBufferObject const &obj
        );

        GLenum checkStatus
        (
        ) const;

        bool isComplete
        (
        ) const;

        void drawQuad
        ( GLenum colorAttachment
        );
    };

} // OGL

#endif // OGL_FBO_H
