#include "VertexArrayObject.hpp"

namespace OGL {

    VertexArrayObject::VertexArrayObject(
    ) {
        glGenVertexArrays(1, &m_descriptor);
    }

    VertexArrayObject::~VertexArrayObject(
    ) {
        glDeleteVertexArrays(1, &m_descriptor);
    }

    void VertexArrayObject::bind(
    ) const {
        glBindVertexArray(m_descriptor);
    }

    void VertexArrayObject::unbind(
    ) {
        glBindVertexArray(0);
    }

} // OGL