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

        void bind
        (
        ) const;

        static void unbind
        (
        );
    };

} // OGL

#endif // OGL_VBO_H
