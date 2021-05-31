/**
* @file UniformBufferObject.hpp
*/

#ifndef OGL_UBO_H
#define OGL_UBO_H

#include "glad/glad.h"
#include "Descriptor.hpp"

namespace OGL {

    /**
     * @brief Class to manipulate uniform data buffers in Shader programms
    */
    class UniformBufferObject : public Descriptor {
     public:
        /**
         * @brief UniformBufferObject constructor
        */
        UniformBufferObject(
        );

        /**
         * @brief UniformBufferObject destructor
        */
        ~UniformBufferObject(
        );

        UniformBufferObject( 
            UniformBufferObject const &rhs
        ) = delete;

        UniformBufferObject(
            UniformBufferObject &&rhs
        ) = default;

        UniformBufferObject &operator=(
            UniformBufferObject const &rhs
        ) = delete;

        UniformBufferObject &operator=( 
            UniformBufferObject &&rhs
        ) = default;

        /**
         * @brief Binds UniformBufferObject
        */
        void bind(
        ) const;

        /**
         * @brief Unbinds UniformBufferObject
        */
        static void unbind(
        );

        /**
         * @brief Allocates buffer storage in UniformBufferObject
         * @param size Size of allocated space
         * @param usage Expected usage of the data - GL_STATIC_DRAW, GL_DYNAMIC_READ, GL_STREAM_DRAW, etc
        */
        void allocateBufferData(
            size_t size, 
            GLenum usage
        );

        /**
         * @brief Writes data in a UniformBufferObject
         * @param offset Buffer starting offset
         * @param length Length of data
         * @param data Pointer to data
        */
        void setBufferData( 
            size_t offset, 
            size_t length, 
            void const *data
        );

        /**
         * @brief Binds UniformBufferObject to binding point
         * @param pointIndex Binding point index
        */
        void setBindingPoint( 
            GLuint pointIndex
        );

        /**
         * @brief Binds range within UniformBufferObject to binding point
         * @param pointIndex Binding point index
         * @param offset Buffer starting offset
         * @param size Size of range
        */
        void setBindingPoint( 
            GLuint pointIndex,
            size_t offset, 
            size_t size
        );
    };

} // OGL

#endif // OGL_UBO_H
