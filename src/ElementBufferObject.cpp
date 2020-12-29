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

} // OGL