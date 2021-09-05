#include "Cubemap.hpp"

namespace OGL {

    Cubemap::Cubemap( 
        int size, 
        GLenum cubemapTextureUnit
    ) : m_textureUnit{ cubemapTextureUnit } {
        Texture::setActive(cubemapTextureUnit);
        m_texture.bind(GL_TEXTURE_CUBE_MAP);
        for (GLenum i = 0; i < 6; ++i) {
            Texture::allocate(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, size, size, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        }

        Texture::setParameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        Texture::setParameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        Texture::setParameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        Texture::setParameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        Texture::setParameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        Texture::unbind(GL_TEXTURE_CUBE_MAP);

        Texture::setActive(GL_TEXTURE0);
    }

    Cubemap::Cubemap( 
        std::string const &folderPath, 
        GLenum cubemapTextureUnit
    ) : m_textureUnit{ cubemapTextureUnit },
        m_folderPath{ folderPath } {
        std::string const fileNames[6] = {
            "right.jpg",
            "left.jpg",
            "top.jpg",
            "bottom.jpg",
            "front.jpg",
            "back.jpg",
        };

        Texture::setActive(cubemapTextureUnit);
        m_texture.bind(GL_TEXTURE_CUBE_MAP);

        //stbi_set_flip_vertically_on_load(false);
        for (GLenum i = 0; i < 6; ++i) {
            std::string filePath = folderPath + "/" + fileNames[i];
            int width, height, nrChannels;
            unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
            if (!data) {
                stbi_image_free(data);
                throw OGL::Exception("Error loading cubemap texture " + filePath);
            }
            Texture::allocate(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        //stbi_set_flip_vertically_on_load(true);

        Texture::setParameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        Texture::setParameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        Texture::setParameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        Texture::setParameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        Texture::setParameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        Texture::setActive(GL_TEXTURE0);
    }

    void Cubemap::bind(
    ) const {
        Texture::setActive(m_textureUnit);
        m_texture.bind(GL_TEXTURE_CUBE_MAP);
    }

    void Cubemap::unbind(
    ) {
        Texture::unbind(GL_TEXTURE_CUBE_MAP);
        Texture::setActive(GL_TEXTURE0);
    }

    unsigned int Cubemap::value(
    ) const {
        return m_texture.value();
    }

    GLenum Cubemap::unit(
    ) const {
        return m_textureUnit;
    }

    std::string Cubemap::folderPath(
    ) const {
        return m_folderPath;
    }

} // OGL