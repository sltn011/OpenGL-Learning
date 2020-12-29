#ifndef OGL_VAO_H
#define OGL_VAO_H

#include "glad/glad.h"
#include "Descriptor.hpp"

namespace OGL {

    class VertexArrayObject : public Descriptor {
    public:
        VertexArrayObject
        (
        );

        ~VertexArrayObject
        (
        );
    };

} // OGL

#endif // OGL_VAO_H
