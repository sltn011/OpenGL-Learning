/**
* @file RenderBufferObject.hpp
*/

#ifndef OGL_RBO_H
#define OGL_RBO_H

#include "glad/glad.h"
#include "Descriptor.hpp"

namespace OGL {

    /**
     * @brief RenderBufferObject class that can be used FrameBufferObject buffer for storing depth and stencil data
    */
    class RenderBufferObject : public Descriptor {
     protected:

     public:
        /**
         * @brief RenderBufferObject constructor
        */
        RenderBufferObject(
        );

        /**
         * @brief RenderBufferObject destructor
        */
        ~RenderBufferObject(
        );

        RenderBufferObject( 
            RenderBufferObject const &rhs
        ) = delete;

        RenderBufferObject( 
            RenderBufferObject &&rhs
        ) = default;

        RenderBufferObject &operator=( 
            RenderBufferObject const &rhs
        ) = delete;

        RenderBufferObject &operator=( 
            RenderBufferObject &&rhs
        ) = default;

        /**
         * @brief Allocates storage in RenderBufferObject
         * @param width Width of buffer
         * @param height Height of buffer
         * @param innertype Type of data inside RenderBufferObject - GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT32F, GL_DEPTH24_STENCIL8
        */
        void allocateStorage(
            int width, 
            int height, 
            GLenum innertype
        );

        /**
         * @brief Allocates multisampled storage in RenderBufferObject
         * @param width Width of buffer
         * @param height Height of buffer
         * @param numSamples Number of samples
         * @param innertype Type of data inside RenderBufferObject - GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT32F, GL_DEPTH24_STENCIL8
        */
        void allocateStorageMultisample(
            int width,
            int height,
            int numSamples,
            GLenum innertype
        );

        void bindAsTexture(
            GLenum textureType
        ) const;
    };

} // OGL

#endif // OGL_RBO_H
