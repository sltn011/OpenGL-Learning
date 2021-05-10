/**
* @file VertexArrayObject.hpp
*/

#ifndef OGL_VBO_H
#define OGL_VBO_H

#include "glad/glad.h"
#include "Descriptor.hpp"

namespace OGL {

    /**
     * @brief Class for OpenGL's VertexBufferObject to store vertices data
    */
    class VertexBufferObject : public Descriptor {
     public:
        /**
        * @brief VertexBufferObject constructor
        */
        VertexBufferObject(
        );

        /**
         * @brief VertexBufferObject constructor
        */
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

        /**
         * @brief Binds VertexBufferObject
        */
        void bind(
        ) const;

        /**
         * @brief Unbinds VertexBufferObject
        */
        static void unbind(
        );
    };

} // OGL

#endif // OGL_VBO_H
