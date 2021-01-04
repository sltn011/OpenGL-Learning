#include "VertexBufferObject.hpp"

namespace OGL {

    VertexBufferObject::VertexBufferObject
    (
    ) {
        glGenBuffers(1, &m_descriptor);
    }

    VertexBufferObject::~VertexBufferObject
    (
    ) {
        glDeleteBuffers(1, &m_descriptor);
    }

    void VertexBufferObject::bind
    (
    ) const {
        glBindBuffer(GL_ARRAY_BUFFER, m_descriptor);
    }

    void VertexBufferObject::unbind
    (
    ) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

} // OGL