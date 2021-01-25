#ifndef OGL_VBO_H
#define OGL_VBO_H

#include "glad/glad.h"
#include "Descriptor.hpp"

namespace OGL {

    class VertexBufferObject : public Descriptor {
     public:
        VertexBufferObject(
        );

        ~VertexBufferObject(
        );

        VertexBufferObject( 
            VertexBufferObject const &rhs
        ) = delete;

        VertexBufferObject( 
            VertexBufferObject &&rhs
        ) = default;

        VertexBufferObject &operator=( 
            VertexBufferObject const &rhs
        ) = delete;

        VertexBufferObject &operator=( 
            VertexBufferObject &&rhs
        ) = default;

        void bind(
        ) const;

        static void unbind(
        );
    };

} // OGL

#endif // OGL_VBO_H
