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
    , GLenum textureType
    , GLint colorComponents
    , GLenum format
    ) {
        glBindTexture(textureType, m_descriptor);
        glTexImage2D(textureType, 0, colorComponents, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(textureType, 0);
    }

} // OGL