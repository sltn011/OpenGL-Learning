#ifndef OGL_SKYBOX_HPP
#define OGL_SKYBOX_HPP

#include "Cubemap.hpp"
#include "VertexArrayObject.hpp"
#include "VertexBufferObject.hpp"
#include "ElementBufferObject.hpp"

namespace OGL {

    class Skybox : public Cubemap {
    public:
        Skybox
        ( std::string const folderPath
        , GLenum cubemapTextureUnit
        );

        void bind
        (
        ) const;

        static void unbind
        (
        );

    protected:
        VertexArrayObject m_vao;
        VertexBufferObject m_vbo;
        ElementBufferObject m_ebo;
    };

} // OGL


#endif // OGL_SCENE_HPP
