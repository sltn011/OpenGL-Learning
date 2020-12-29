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

} // OGL