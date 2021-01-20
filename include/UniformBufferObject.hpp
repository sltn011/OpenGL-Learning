#ifndef OGL_UBO_H
#define OGL_UBO_H

#include "glad/glad.h"
#include "Descriptor.hpp"

namespace OGL {

    class UniformBufferObject : public Descriptor {
    public:
        UniformBufferObject
        (
        );

        ~UniformBufferObject
        (
        );

        UniformBufferObject
        ( UniformBufferObject const &rhs
        ) = delete;

        UniformBufferObject
        ( UniformBufferObject &&rhs
        ) = default;

        UniformBufferObject &operator=
        ( UniformBufferObject const &rhs
        ) = delete;

        UniformBufferObject &operator=
        ( UniformBufferObject &&rhs
        ) = default;

        void bind
        (
        ) const;

        static void unbind
        (
        );

        void allocateBufferData
        ( size_t size
        , GLenum usage
        );

        void setBufferData
        ( size_t offset
        , size_t length
        , void const *data
        );

        void setBindingPoint
        ( size_t pointIndex
        );

        void setBindingPoint
        ( size_t pointIndex
        , size_t offset
        , size_t size
        );
    };

} // OGL

#endif // OGL_UBO_H
