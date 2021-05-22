/**
* @file FrameBufferObject.hpp
*/

#ifndef OGL_FBO_H
#define OGL_FBO_H

#include "glad/glad.h"
#include "Descriptor.hpp"
#include "ColorBufferObject.hpp"
#include "RenderBufferObject.hpp"
#include "VertexArrayObject.hpp"
#include "VertexBufferObject.hpp"
#include "Shader.hpp"
#include <unordered_map>

namespace OGL {

    /**
     * @brief Class for OpenGL's FrameBufferObject that can be used to store render data
    */
    class FrameBufferObject : public Descriptor {
     protected:
        /// Quad's VertexArrayObject - created when FrameBufferObject set to be drawn with drawQuad method
        VertexArrayObject m_frameQuadVAO;

        /// Quad's VertexBufferObject - created when FrameBufferObject set to be drawn with drawQuad method
        VertexBufferObject m_frameQuadVBO;

        /// Hashmap of color attachment -> ColorBufferObject which are attached to FrameBufferObject
        std::unordered_map<GLenum, ColorBufferObject> m_colorBufferObjs;

        /// RenderBufferObject attached to FrameBufferObject
        RenderBufferObject m_renderBufferObj;

     protected:
        static float const s_frameQuadData[];

     public:

        /**
         * @brief Empty FrameBufferObject constructor 
        */
        FrameBufferObject( 
        );

        /**
         * @brief Empty FrameBufferObject constructor which can be used in rendering as a texture of FrameBufferObject data
         * @param frameQuadVerticesVertexAttribIndex Vertices attribute index in vertex shader used to render FrameBufferObject data as texture
         * @param frameQuadTexCoordVertexAttribIndex Texels attribute index in vertex shader used to render FrameBufferObject data as texture
        */
        FrameBufferObject(
            GLuint frameQuadVerticesVertexAttribIndex,
            GLuint frameQuadTexCoordVertexAttribIndex
        );

        /**
         * @brief FrameBufferObject destructor
        */
        ~FrameBufferObject(
        );

        FrameBufferObject(
            FrameBufferObject const &rhs
        ) = delete;

        FrameBufferObject(
            FrameBufferObject &&rhs
        ) noexcept;

        FrameBufferObject &operator=( 
            FrameBufferObject const &rhs
        ) = delete;

        FrameBufferObject &operator=(
            FrameBufferObject &&rhs
        ) = default;

        /**
         * @brief Attaches ColorBufferObject to FrameBufferObject
         * @param framebufferType Type of FrameBufferObject - GL_FRAMEBUFFER, GL_DRAW_FRAMEBUFFER, GL_READ_FRAMEBUFFER
         * @param colorAttachment Attachment for ColorBufferObject - GL_DEPTH_ATTACHMENT(for rendering shadows)
         * @param obj ColorBufferObject
        */
        void attachColorBuffer(
            GLenum framebufferType,
            GLenum colorAttachment,
            ColorBufferObject &&obj
        );

        /**
         * @brief Attaches ColorBufferObject to FrameBufferObject 
         * @param framebufferType Type of FrameBufferObject - GL_FRAMEBUFFER, GL_DRAW_FRAMEBUFFER, GL_READ_FRAMEBUFFER
         * @param colorAttachment Attachment for ColorBufferObject - GL_COLOR_ATTACHMENT{N}
         * @param targetTexture Type of texture represented by ColorBufferObject - GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X and other
         * @param obj ColorBufferObject
        */
        void attachColorBuffer( 
            GLenum framebufferType,
            GLenum colorAttachment,
            GLenum targetTexture,
            ColorBufferObject &&obj
        );

        /**
         * @brief Attaches multisampled ColorBufferObject to FrameBufferObject
         * @param framebufferType Type of FrameBufferObject - GL_FRAMEBUFFER, GL_DRAW_FRAMEBUFFER, GL_READ_FRAMEBUFFER
         * @param colorAttachment Attachment for ColorBufferObject - GL_COLOR_ATTACHMENT{N}
         * @param obj ColorBufferObject
        */
        void attachColorBufferMultisample(
            GLenum framebufferType,
            GLenum colorAttachment,
            ColorBufferObject &&obj
        );

        /**
         * @brief Attaches RenderBufferObject to FrameBufferObject 
         * @param framebufferType Type of FrameBufferObject - GL_FRAMEBUFFER, GL_DRAW_FRAMEBUFFER, GL_READ_FRAMEBUFFER
         * @param attachment Attachment for RenderBufferObject - GL_DEPTH_ATTACHMENT, GL_STENCIL_ATTACHMENT, GL_DEPTH_STENCIL_ATTACHMENT
         * @param obj RenderBufferObject
        */
        void attachRenderBuffer( 
            GLenum framebufferType,
            GLenum attachment,
            RenderBufferObject &&obj
        );

        /**
         * @brief Attaches multisampled RenderBufferObject to FrameBufferObject 
         * @param framebufferType Type of FrameBufferObject - GL_FRAMEBUFFER, GL_DRAW_FRAMEBUFFER, GL_READ_FRAMEBUFFER
         * @param attachment Attachment for RenderBufferObject - GL_DEPTH_ATTACHMENT, GL_STENCIL_ATTACHMENT, GL_DEPTH_STENCIL_ATTACHMENT
         * @param obj RenderBufferObject
        */
        void attachRenderBufferMultisample(
            GLenum framebufferType,
            GLenum attachment,
            RenderBufferObject &&obj
        );

        /**
         * @brief Binds FrameBufferObject
         * @param framebufferType Type of FrameBufferObject - GL_FRAMEBUFFER, GL_DRAW_FRAMEBUFFER, GL_READ_FRAMEBUFFER
        */
        void bind(
            GLenum framebufferType
        ) const;

        /**
         * @brief Unbinds specified FrameBufferObject and sets this framebufferType to default FrameBufferObject
         * @param framebufferType Type of FrameBufferObject - GL_FRAMEBUFFER, GL_DRAW_FRAMEBUFFER, GL_READ_FRAMEBUFFER
        */
        static void unbind( 
            GLenum framebufferType
        );

        /**
         * @brief Checks status of FrameBufferObject
         * @param framebufferType Type of FrameBufferObject - GL_FRAMEBUFFER, GL_DRAW_FRAMEBUFFER, GL_READ_FRAMEBUFFER
         * @return Status of FrameBufferObject
        */
        static GLenum checkStatus(
            GLenum framebufferType
        );

        /**
         * @brief Checks if FrameBufferObject is complete and can be used
         * @param framebufferType Type of FrameBufferObject - GL_FRAMEBUFFER, GL_DRAW_FRAMEBUFFER, GL_READ_FRAMEBUFFER
         * @return True if FrameBufferObject is completed, False otherwise
        */
        static bool isComplete(
            GLenum framebufferType
        );

        /**
         * @brief Renders FrameBufferObject data as a texture using currently active Shader
         * @param colorAttachment Color attachment used as texture of quad
        */
        void drawQuad(
            GLenum colorAttachment
        );

        /**
         * @brief Getter of color attachments map
         * @return Reference to hashmap Color attachment -> ColorBufferObject
        */
        std::unordered_map<GLenum, ColorBufferObject> &getColorBuffers(
        );

        /**
         * @brief Getter of attached RenderBufferObject
         * @return Reference to RenderBufferObject - only valid if attached
        */
        RenderBufferObject &getRenderBuffer(
        );
    };

} // OGL

#endif // OGL_FBO_H
