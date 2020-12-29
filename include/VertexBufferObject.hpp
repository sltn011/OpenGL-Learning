#ifndef OGL_VBO_H
#define OGL_VBO_H

#include "glad/glad.h"
#include "Descriptor.hpp"

namespace OGL {

    class VertexBufferObject : public Descriptor {
    public:
        VertexBufferObject
        (
        );

        ~VertexBufferObject
        (
        );
    };

} // OGL

#endif // OGL_VBO_H
