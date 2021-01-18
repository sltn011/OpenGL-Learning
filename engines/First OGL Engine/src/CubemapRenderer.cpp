#include "CubemapRenderer.hpp"

namespace OGL::E1 {

    CubemapRenderer::CubemapRenderer
    (
    ) {

    }

    Cubemap CubemapRenderer::render
    ( Scene &scene
    , int cubemapSize
    , GLenum cubemapTextureUnit
    , glm::vec3 mirrorPos
    , NormalRenderer *normalRenderer
    , SkyboxRenderer *skyboxRenderer
    , TransparentRenderer *transparentRenderer
    ) {
        smartCamPtr cameraCubemap = std::make_unique<CameraCubemap>(mirrorPos, 0.1f, 100.0f);
        Cubemap cubemap(cubemapSize, cubemapTextureUnit);
        
        FrameBufferObject framebuffer(GL_FRAMEBUFFER);
        framebuffer.bind();
        glDrawBuffer(GL_COLOR_ATTACHMENT0);

        RenderBufferObject renderbuffer;
        renderbuffer.allocateStorage(cubemapSize, cubemapSize, GL_DEPTH_COMPONENT24);
        framebuffer.attach(GL_DEPTH_ATTACHMENT, std::move(renderbuffer));

        for (int i = 0; i < 6; ++i) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap.value(), 0);
            dynamic_cast<CameraCubemap*>(&*cameraCubemap)->setSide(i);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderSide(scene, cameraCubemap, cubemap, normalRenderer, skyboxRenderer, transparentRenderer);
        }

        FrameBufferObject::unbind(GL_FRAMEBUFFER);

        cubemap.bind();
        OGL::Texture::generateMipmap(GL_TEXTURE_CUBE_MAP);
        Cubemap::unbind();

        return cubemap;
    }

    void CubemapRenderer::renderSide
    ( Scene &scene
    , smartCamPtr const &cameraCubemap
    , Cubemap &cubemap
    , NormalRenderer *normalRenderer
    , SkyboxRenderer *skyboxRenderer
    , TransparentRenderer *transparentRenderer
    ) {
        if (skyboxRenderer) {
            skyboxRenderer->render(scene, cameraCubemap);
        }
        if (normalRenderer) {
            normalRenderer->render(scene, cameraCubemap);
        }
        if (transparentRenderer) {
            transparentRenderer->render(scene, cameraCubemap);
        }
    }

} // OGL::E1