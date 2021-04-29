#ifndef OGL_SKYBOX_H
#define OGL_SKYBOX_H

#include "Cubemap.hpp"
#include "VertexArrayObject.hpp"
#include "VertexBufferObject.hpp"
#include "ElementBufferObject.hpp"

namespace OGL {

    class Skybox : public Cubemap {
     protected:
        VertexArrayObject m_vao;
        VertexBufferObject m_vbo;
        ElementBufferObject m_ebo;

     public:
        Skybox( 
            std::string const &folderPath,
            GLenum cubemapTextureUnit
        );

        Skybox(
            Skybox const &rhs
        ) = delete;

        Skybox(
            Skybox &&rhs
        ) = default;

        Skybox &operator=(
            Skybox const &rhs
        ) = delete;

        Skybox &operator=(
            Skybox &&rhs
        ) = default;

        void bind(
        ) const override;

        static void unbind(
        );
        
    };

} // OGL


#endif // OGL_SKYBOX_H
