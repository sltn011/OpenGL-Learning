#ifndef OGL_EBO_H
#define OGL_EBO_H

#include "glad/glad.h"
#include "Descriptor.hpp"

namespace OGL {

    class ElementBufferObject : public Descriptor {
     public:
        ElementBufferObject(
        );

        ~ElementBufferObject(
        );

        ElementBufferObject(
            ElementBufferObject const &rhs
        ) = delete;

        ElementBufferObject(
            ElementBufferObject &&rhs
        ) = default;

        ElementBufferObject &operator=(
            ElementBufferObject const &rhs
        ) = delete;

        ElementBufferObject &operator=(
            ElementBufferObject &&rhs
        ) = default;

        void bind(
        ) const;

        static void unbind(
        );
    };

} // OGL

#endif // OGL_EBO_H
