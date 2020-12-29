#include "FrameBufferObject.hpp"

namespace OGL {

    FrameBufferObject::FrameBufferObject
    (GLenum bufferType
    , float const *frameBufferQuadData
    , size_t frameBufferQuadDataLength
    , GLuint frameQuadVerticesVertexAttribIndex
    , GLuint frameQuadTexCoordVertexAttribIndex
    ) : m_bufferType{bufferType} {
        glGenFramebuffers(1, &m_descriptor);
        glBindVertexArray(m_frameQuadVAO.value());
        glBindBuffer(GL_ARRAY_BUFFER, m_frameQuadVBO.value());
        glBufferData(GL_ARRAY_BUFFER, frameBufferQuadDataLength, frameBufferQuadData, GL_STATIC_DRAW);
        glEnableVertexAttribArray(frameQuadVerticesVertexAttribIndex);
        glVertexAttribPointer(frameQuadVerticesVertexAttribIndex, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(frameQuadTexCoordVertexAttribIndex);
        glVertexAttribPointer(frameQuadTexCoordVertexAttribIndex, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glBindVertexArray(0);
    }

    FrameBufferObject::~FrameBufferObject
    (
    ) {
        glDeleteFramebuffers(1, &m_descriptor);
    }

    void FrameBufferObject::attach
    ( GLenum colorAttachment
    , GLenum targetTexture
    , ColorBufferObject const &obj
    ) {
        glBindFramebuffer(m_bufferType, m_descriptor);
        glFramebufferTexture2D(m_bufferType, colorAttachment, targetTexture, obj.value(), 0);
        glBindFramebuffer(m_bufferType, 0);
        m_colorAttachmentsTable[colorAttachment] = obj.value();
    }

    void FrameBufferObject::attach
    ( GLenum attachment
    , RenderBufferObject const &obj
    ) {
        glBindFramebuffer(m_bufferType, m_descriptor);
        glFramebufferRenderbuffer(m_bufferType, attachment, GL_RENDERBUFFER, obj.value());
        glBindFramebuffer(m_bufferType, 0);
    }

    GLenum FrameBufferObject::checkStatus
    (
    ) const {
        return glCheckFramebufferStatus(m_bufferType);
    }

    bool FrameBufferObject::isComplete
    (
    ) const {
        return checkStatus() == GL_FRAMEBUFFER_COMPLETE;
    }

    void FrameBufferObject::drawQuad
    ( GLenum colorAttachment
    ) {
        glBindVertexArray(m_frameQuadVAO.value());
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, m_colorAttachmentsTable[colorAttachment]);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    float const FrameBufferObject::frameQuadData[] = {
        // position      // texture
        -1.0f, +1.0f,    0.0f, 1.0f,
        -1.0f, -1.0f,    0.0f, 0.0f,
        +1.0f, -1.0f,    1.0f, 0.0f,
        
        -1.0f, +1.0f,    0.0f, 1.0f,
        +1.0f, -1.0f,    1.0f, 0.0f,
        +1.0f, +1.0f,    1.0f, 1.0f,
    };

} // OGL