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

        void bind(
        ) const;

        static void unbind(
        );
    };

} // OGL

#endif // OGL_EBO_H
