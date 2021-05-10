#include "UniformBufferObject.hpp"

namespace OGL {

    UniformBufferObject::UniformBufferObject(
    ) {
        glGenBuffers(1, &m_descriptor);
    }

    UniformBufferObject::~UniformBufferObject(
    ) {
        glDeleteBuffers(1, &m_descriptor);
    }

    void UniformBufferObject::bind(
    ) const {
        glBindBuffer(GL_UNIFORM_BUFFER, m_descriptor);
    }

    void UniformBufferObject::unbind(
    ) {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void UniformBufferObject::allocateBufferData(
        size_t size, 
        GLenum usage
    ) {
        glBufferData(GL_UNIFORM_BUFFER, size, nullptr, usage);
    }

    void UniformBufferObject::setBufferData( 
        size_t offset, 
        size_t length,
        void const *data
    ) {
        glBufferSubData(GL_UNIFORM_BUFFER, offset, length, data);
    }

    void UniformBufferObject::setBindingPoint(
        GLuint pointIndex
    ) {
        glBindBufferBase(GL_UNIFORM_BUFFER, pointIndex, m_descriptor);
    }

    void UniformBufferObject::setBindingPoint( 
        GLuint pointIndex,
        size_t offset, 
        size_t size
    ) {
        glBindBufferRange(GL_UNIFORM_BUFFER, pointIndex, m_descriptor, offset, size);
    }

} // OGL