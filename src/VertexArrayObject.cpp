#include "VertexArrayObject.hpp"

namespace OGL {

    VertexArrayObject::VertexArrayObject
    (
    ) {
        glGenVertexArrays(1, &m_descriptor);
    }

    VertexArrayObject::~VertexArrayObject
    (
    ) {
        glDeleteVertexArrays(1, &m_descriptor);
    }

} // OGL