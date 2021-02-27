#ifndef OGL_CBO_H
#define OGL_CBO_H

#include "glad/glad.h"
#include "Descriptor.hpp"

namespace OGL {

    class ColorBufferObject : public Descriptor {
     public:
        ColorBufferObject();

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

        void allocateStorage( 
            int width, 
            int height, 
            GLenum textureType, 
            GLint colorComponents, 
            GLenum format,
            GLenum type
        );

        void allocateStorageMultisample(
            int width,
            int height,
            int numSamples,
            GLenum format
        );

        void bindAsTexture(
            GLenum textureType
        ) const;
    };

} // OGL

#endif // OGL_CBO_H
