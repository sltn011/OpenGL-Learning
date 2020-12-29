#ifndef OGL_RBO_H
#define OGL_RBO_H

#include "glad/glad.h"
#include "Descriptor.hpp"

namespace OGL {

    class RenderBufferObject : public Descriptor {
    protected:

    public:
        RenderBufferObject
        (
        );

        ~RenderBufferObject
        (
        );

        void allocateStorage
        ( int width
        , int height
        , GLenum innertype
        );
    };

} // OGL

#endif // OGL_RBO_H
