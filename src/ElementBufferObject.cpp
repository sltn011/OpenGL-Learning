#include "ElementBufferObject.hpp"

namespace OGL {

    ElementBufferObject::ElementBufferObject
    (
    ) {
        glGenBuffers(1, &m_descriptor);
    }

    ElementBufferObject::~ElementBufferObject
    (
    ) {
        glDeleteBuffers(1, &m_descriptor);
    }

    void ElementBufferObject::bind
    (
    ) const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_descriptor);
    }

    void ElementBufferObject::unbind
    (
    ) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

} // OGL