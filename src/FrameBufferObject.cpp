#include "FrameBufferObject.hpp"

namespace OGL {

    FrameBufferObject::FrameBufferObject( 
    ) {
        glGenFramebuffers(1, &m_descriptor);
    }

    FrameBufferObject::FrameBufferObject(
        GLuint frameQuadVerticesVertexAttribIndex,
        GLuint frameQuadTexCoordVertexAttribIndex
    ) {
        glGenFramebuffers(1, &m_descriptor);
        glBindVertexArray(m_frameQuadVAO.value());
        glBindBuffer(GL_ARRAY_BUFFER, m_frameQuadVBO.value());
        glBufferData(GL_ARRAY_BUFFER, 96, frameQuadData, GL_STATIC_DRAW);
        glEnableVertexAttribArray(frameQuadVerticesVertexAttribIndex);
        glVertexAttribPointer(frameQuadVerticesVertexAttribIndex, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(frameQuadTexCoordVertexAttribIndex);
        glVertexAttribPointer(frameQuadTexCoordVertexAttribIndex, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glBindVertexArray(0);
    }

    FrameBufferObject::~FrameBufferObject(
    ) {
        glDeleteFramebuffers(1, &m_descriptor);
    }

    void FrameBufferObject::attachColorBuffer(
        GLenum framebufferType,
        GLenum colorAttachment, 
        GLenum targetTexture, 
        ColorBufferObject &&obj
    ) {
        glFramebufferTexture2D(framebufferType, colorAttachment, targetTexture, obj.value(), 0);
        m_colorAttachmentsTable[colorAttachment] = obj.value();
        m_colorBufferObjs.emplace(colorAttachment, std::move(obj));
    }

    void FrameBufferObject::attachColorBufferMultisample(
        GLenum framebufferType,
        GLenum colorAttachment,
        ColorBufferObject &&obj
    ) {
        attachColorBuffer(framebufferType, colorAttachment, GL_TEXTURE_2D_MULTISAMPLE, std::move(obj));
    }

    void FrameBufferObject::attachRenderBuffer(
        GLenum framebufferType,
        GLenum attachment, 
        RenderBufferObject &&obj
    ) {
        glFramebufferRenderbuffer(framebufferType, attachment, GL_RENDERBUFFER, obj.value());
        m_renderBufferObj = std::move(obj);
    }

    void FrameBufferObject::attachRenderBufferMultisample(
        GLenum framebufferType,
        GLenum attachment,
        RenderBufferObject &&obj
    ) {
        attachRenderBuffer(framebufferType, attachment, std::move(obj));
    }

    void FrameBufferObject::bind(
        GLenum framebufferType
    ) const {
        glBindFramebuffer(framebufferType, m_descriptor);
    }

    void FrameBufferObject::unbind( 
        GLenum framebufferType
    ) {
        glBindFramebuffer(framebufferType, 0);
    }

    GLenum FrameBufferObject::checkStatus(
        GLenum framebufferType
    ) const {
        return glCheckFramebufferStatus(framebufferType);
    }

    bool FrameBufferObject::isComplete(
        GLenum framebufferType
    ) const {
        return checkStatus(framebufferType) == GL_FRAMEBUFFER_COMPLETE;
    }

    void FrameBufferObject::drawQuad( 
        GLenum colorAttachment
    ) {
        glBindVertexArray(m_frameQuadVAO.value());
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