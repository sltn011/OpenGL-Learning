#include "ShadowCubemap.hpp"

namespace OGL {

    ShadowCubemap::ShadowCubemap(
        int size,
        GLenum cubemapTextureUnit
    ) : m_textureUnit{ cubemapTextureUnit } {
        Texture::setActive(cubemapTextureUnit);
        m_texture.bind(GL_TEXTURE_CUBE_MAP);
        for (size_t i = 0; i < 6; ++i) {
            Texture::allocate(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, size, size, GL_DEPTH_COMPONENT, nullptr);
        }

        Texture::setParameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        Texture::setParameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        Texture::setParameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        Texture::setParameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        Texture::setParameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        Texture::unbind(GL_TEXTURE_CUBE_MAP);

        Texture::setActive(GL_TEXTURE0);
    }

}