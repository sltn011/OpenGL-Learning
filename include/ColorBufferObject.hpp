/**
* @file ColorBufferObject.hpp
*/

#ifndef OGL_CBO_H
#define OGL_CBO_H

#include "glad/glad.h"
#include "Descriptor.hpp"

namespace OGL {

    /**
     * @brief ColorBufferObject class that can be used as FrameBufferObject buffer for storing color data
    */
    class ColorBufferObject : public Descriptor {
     public:

        /**
         * @brief ColorBufferObject constructor
        */
        ColorBufferObject();

        /**
         * @brief ColorBufferObject destructor
        */
        ~ColorBufferObject();

        ColorBufferObject(
            ColorBufferObject const &rhs
        ) = delete;

        ColorBufferObject(
            ColorBufferObject &&rhs
        ) = default;

        ColorBufferObject &operator=(
            ColorBufferObject const &rhs
        ) = delete;

        ColorBufferObject &operator=( 
            ColorBufferObject &&rhs
        ) = default;

        /**
         * @brief Allocates storage in ColorBufferObject to store data
         * @param width Buffer width
         * @param height Buffer height
         * @param textureType Type of texture used for buffer - most commonly GL_TEXTURE_2D
         * @param colorComponents Format of pixel data and number of channels in GPU - GL_RED, GL_RG, GL_RGB and others
         * @param format Format of pixel data - GL_RED, GL_RG, GL_RGB and others
         * @param type Data type of pixel data - GL_UNSIGNED_INT, GL_BYTE, GL_FLOAT and others
        */
        void allocateStorage( 
            int width, 
            int height, 
            GLenum textureType, 
            GLint colorComponents, 
            GLenum format,
            GLenum type
        );

        /**
         * @brief Allocates multisampled storage in ColorBufferObject
         * @param width Buffer width
         * @param height Buffer height
         * @param numSamples Number of samples in buffer
         * @param format Format of pixel data - GL_RED, GL_RG, GL_RGB and others
        */
        void allocateStorageMultisample(
            int width,
            int height,
            int numSamples,
            GLenum format
        );

        /**
         * @brief Binds ColorBufferObject as Texture
         * @param textureType Texture type
        */
        void bindAsTexture(
            GLenum textureType
        ) const;
    };

} // OGL

#endif // OGL_CBO_H
