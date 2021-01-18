#include "Skybox.hpp"

namespace OGL {
    
    Skybox::Skybox
    ( std::string const folderPath
    , GLenum cubemapTextureUnit
    ) : Cubemap{folderPath, cubemapTextureUnit} {
        float constexpr skyboxVertices[] = {
            -1.0f, -1.0f, -1.0f, // 1
            -1.0f, -1.0f, +1.0f, // 2
            +1.0f, -1.0f, +1.0f, // 3
            +1.0f, -1.0f, -1.0f, // 4
            -1.0f, +1.0f, -1.0f, // 5
            -1.0f, +1.0f, +1.0f, // 6
            +1.0f, +1.0f, +1.0f, // 7
            +1.0f, +1.0f, -1.0f, // 8
        };

        int constexpr skyboxIndices[] = {
            4, 0, 3,
            3, 7, 4,

            7, 3, 2,
            2, 6, 7,

            6, 2, 1,
            1, 5, 6,

            5, 1, 0,
            0, 4, 5,

            5, 4, 7,
            7, 6, 5,

            0, 1, 2,
            2, 3, 0,
        };

        m_vao.bind();
        m_vbo.bind();
        m_ebo.bind();
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        VertexArrayObject::unbind();
    }

    void Skybox::bind
    (
    ) const {
        m_vao.bind();
        Texture::setActive(m_textureUnit);
        m_texture.bind(GL_TEXTURE_CUBE_MAP);
    }

    void Skybox::unbind
    (
    ) {
        Texture::unbind(GL_TEXTURE_CUBE_MAP);
        Texture::setActive(GL_TEXTURE0);
        VertexArrayObject::unbind();
    }

} // OGL