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

        VertexArrayObject
        ( VertexArrayObject const &rhs
        ) = delete;

        VertexArrayObject
        ( VertexArrayObject &&rhs
        ) = default;

        VertexArrayObject &operator=
        ( VertexArrayObject const &rhs
        ) = delete;

        VertexArrayObject &operator=
        ( VertexArrayObject &&rhs
        ) = default;

        void bind
        (
        ) const;

        static void unbind
        (
        );
    };

} // OGL

#endif // OGL_VAO_H
