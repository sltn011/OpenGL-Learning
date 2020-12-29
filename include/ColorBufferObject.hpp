#ifndef OGL_CBO_H
#define OGL_CBO_H

#include "glad/glad.h"
#include "Descriptor.hpp"

namespace OGL {

    class ColorBufferObject : public Descriptor {
    public:
        ColorBufferObject
        (
        );

        ~ColorBufferObject
        (
        );

        void allocateStorage
        ( int width
        , int height
        , GLint colorComponents
        , GLenum format
        );
    };

} // OGL

#endif // OGL_CBO_H
