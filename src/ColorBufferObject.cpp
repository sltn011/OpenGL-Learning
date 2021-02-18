#include "ColorBufferObject.hpp"

namespace OGL {

    ColorBufferObject::ColorBufferObject(
    ) {
        glGenTextures(1, &m_descriptor);
    }

    ColorBufferObject::~ColorBufferObject(
    ) {
        glDeleteTextures(1, &m_descriptor);
    }

    void ColorBufferObject::allocateStorage( 
        int width, 
        int height, 
        GLenum textureType,
        GLint colorComponents,
        GLenum format,
        GLenum type
    ) {
        glBindTexture(textureType, m_descriptor);
        glTexImage2D(textureType, 0, colorComponents, width, height, 0, format, type, nullptr);
        glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(textureType, 0);
    }

    void ColorBufferObject::allocateStorageMultisample(
        int width,
        int height,
        int numSamples,
        GLenum format
    ) {
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_descriptor);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, numSamples, format, width, height, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    }

    void ColorBufferObject::bindAsTexture(
        GLenum textureType
    ) const {
        glBindTexture(textureType, m_descriptor);
    }

} // OGL