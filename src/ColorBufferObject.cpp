#include "ColorBufferObject.hpp"

namespace OGL {

    ColorBufferObject::ColorBufferObject
    (
    ) {
        glGenTextures(1, &m_descriptor);
    }

    ColorBufferObject::~ColorBufferObject
    (
    ) {
        glDeleteTextures(1, &m_descriptor);
    }

    void ColorBufferObject::allocateStorage
    ( int width
    , int height
    , GLint colorComponents
    , GLenum format
    ) {
        glBindTexture(GL_TEXTURE_2D, m_descriptor);
        glTexImage2D(GL_TEXTURE_2D, 0, colorComponents, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

} // OGL