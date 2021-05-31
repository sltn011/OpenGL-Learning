/**
* @file ElementBufferObject.hpp
*/

#ifndef OGL_EBO_H
#define OGL_EBO_H

#include "glad/glad.h"
#include "Descriptor.hpp"

namespace OGL {

    /**
     * @brief Class for OpenGL's ElementBufferObject to store indices data
    */
    class ElementBufferObject : public Descriptor {
     public:

        /**
         * @brief ElementBufferObject constructor
        */
        ElementBufferObject(
        );

        /**
         * @brief ElementBufferObject destructor
        */
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

        /**
         * @brief Binds ElementBufferObject
        */
        void bind(
        ) const;

        /**
         * @brief Unbinds ElementBufferObject
        */
        static void unbind(
        );
    };

} // OGL

#endif // OGL_EBO_H
