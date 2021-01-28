#include "RenderBufferObject.hpp"

namespace OGL {

    RenderBufferObject::RenderBufferObject(
    ) {
        glGenRenderbuffers(1, &m_descriptor);
    }

    RenderBufferObject::~RenderBufferObject(
    ) {
        glDeleteRenderbuffers(1, &m_descriptor);
    }

    void RenderBufferObject::allocateStorage( 
        int width, 
        int height, 
        GLenum innertype
    ) {
        glBindRenderbuffer(GL_RENDERBUFFER, m_descriptor);
        glRenderbufferStorage(GL_RENDERBUFFER, innertype, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    void RenderBufferObject::allocateStorageMultisample(
        int width,
        int height, 
        int numSamples,
        GLenum innertype
    ) {
        glBindRenderbuffer(GL_RENDERBUFFER, m_descriptor);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, numSamples, innertype, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

} // OGL