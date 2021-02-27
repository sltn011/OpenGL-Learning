#include "Texture.hpp"


namespace OGL {

    Texture::Texture(
    ) {
        glGenTextures(1, &m_descriptor);
    }

    Texture::Texture( 
        std::string const &folderPath,
        std::string const &fileName, 
        GLenum textureType, 
        bool gamma
    ) {
        glGenTextures(1, &m_descriptor);
        load(folderPath, fileName, textureType, gamma);
    }

    Texture::~Texture(
    ) {
        glDeleteTextures(1, &m_descriptor);
    }

    void Texture::allocate( 
        GLenum textureType, 
        int width, 
        int height, 
        GLenum format,
        GLenum type,
        void *data
    ) {
        glTexImage2D(textureType, 0, format, width, height, 0, format, type, data);
    }

    void Texture::load( 
        std::string const &folderPath,
        std::string const &fileName,
        GLenum textureType, 
        bool gamma
    ) {
        std::string filePath = folderPath + '/' + fileName;
        
        int width, height, nrComponents;
        unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrComponents, 0);
        if (!data) {
            throw Exception("Error loading texture " + filePath);
        }
        GLenum format;
        switch (nrComponents) {
        case 1:
            format = GL_RED;
            break;

        case 3:
            format = GL_RGB;
            break;

        case 4:
            format = GL_RGBA;
            break;

        default:
            format = 0;
            break;
        }

        glBindTexture(textureType, m_descriptor);
        glTexImage2D(textureType, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    void Texture::setActive( 
        GLenum textureUnit
    ) {
        glActiveTexture(textureUnit);
    }

    void Texture::bind( 
        GLenum textureType
    ) const {
        glBindTexture(textureType, m_descriptor);
    }

    void Texture::bind(
        GLenum textureType,
        int texture
    ) {
        glBindTexture(textureType, texture);
    }

    void Texture::unbind( 
        GLenum textureType
    ) {
        glBindTexture(textureType, 0);
    }

    void Texture::setParameter( 
        GLenum textureType,
        GLenum paramName, 
        GLint paramValue
    ) {
        glTexParameteri(textureType, paramName, paramValue);
    }

    void Texture::generateMipmap( 
        GLenum textureType
    ) {
        glGenerateMipmap(textureType);
    }

} // OGL