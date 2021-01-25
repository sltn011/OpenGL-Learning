#ifndef OGL_SKYBOX_H
#define OGL_SKYBOX_H

#include "Cubemap.hpp"
#include "VertexArrayObject.hpp"
#include "VertexBufferObject.hpp"
#include "ElementBufferObject.hpp"

namespace OGL {

    class Skybox : public Cubemap {
     public:
        Skybox( 
            std::string const folderPath,
            GLenum cubemapTextureUnit
        );

        void bind(
        ) const override;

        static void unbind(
        );

     protected:
        VertexArrayObject m_vao;
        VertexBufferObject m_vbo;
        ElementBufferObject m_ebo;
    };

} // OGL


#endif // OGL_SKYBOX_H
