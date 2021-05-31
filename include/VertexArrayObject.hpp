/**
* @file VertexArrayObject.hpp
*/

#ifndef OGL_VAO_H
#define OGL_VAO_H

#include "glad/glad.h"
#include "Descriptor.hpp"

namespace OGL {

    /**
     * @brief Class to work with OpenGL's VertexArrayObjects that describes how the vertex attributes are stored in VertexBufferObject and ElementBufferObject
    */
    class VertexArrayObject : public Descriptor {
     public:
        /**
         * @brief VertexArrayObject constructor
        */
        VertexArrayObject(
        );

        /**
         * @brief VertexArrayObject destructor
        */
        ~VertexArrayObject(
        );

        VertexArrayObject( 
            VertexArrayObject const &rhs
        ) = delete;

        VertexArrayObject( 
            VertexArrayObject &&rhs
        ) = default;

        VertexArrayObject &operator=( 
            VertexArrayObject const &rhs
        ) = delete;

        VertexArrayObject &operator=( 
            VertexArrayObject &&rhs
        ) = default;

        /**
         * @brief Binds VertexArrayObject
        */
        void bind(
        ) const;

        /**
         * @brief Unbinds VertexArrayObject
        */
        static void unbind(
        );
    };

} // OGL

#endif // OGL_VAO_H
