#include "CubemapRenderer.hpp"

namespace OGL::E1 {

    CubemapRenderer::CubemapRenderer(
    ) {

    }

    Cubemap CubemapRenderer::render( 
        Scene &scene, 
        int cubemapSize, 
        GLenum cubemapTextureUnit,
        glm::vec3 cubemapPos, 
        maybeNormalRenderer &normalRenderer,
        maybeSkyboxRenderer &skyboxRenderer,
        maybeTransparentRenderer &transparentRenderer,
        maybeInstancesRenderer &instancesRenderer
    ) {
        CameraCubemap cameraCubemap(cubemapPos, 0.1f, 100.0f);
        Cubemap cubemap(cubemapSize, cubemapTextureUnit);
        
        FrameBufferObject framebuffer;
        framebuffer.bind(GL_FRAMEBUFFER);
        glDrawBuffer(GL_COLOR_ATTACHMENT0);

        RenderBufferObject renderbuffer;
        renderbuffer.allocateStorage(cubemapSize, cubemapSize, GL_DEPTH_COMPONENT24);
        framebuffer.attachRenderBuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, std::move(renderbuffer));

        for (int i = 0; i < 6; ++i) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap.value(), 0);
            cameraCubemap.setSide(i);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderSide(scene, &cameraCubemap, normalRenderer, skyboxRenderer, transparentRenderer, instancesRenderer);
        }

        framebuffer.unbind(GL_FRAMEBUFFER);

        cubemap.bind();
        OGL::Texture::generateMipmap(GL_TEXTURE_CUBE_MAP);
        Cubemap::unbind();

        return cubemap;
    }

    void CubemapRenderer::renderSide(
        Scene &scene,
        CameraCubemap const *cubemapCamera,
        maybeNormalRenderer &normalRenderer,
        maybeSkyboxRenderer &skyboxRenderer,
        maybeTransparentRenderer &transparentRenderer,
        maybeInstancesRenderer &instancesRenderer
    ) {
        if (normalRenderer) {
            normalRenderer->render(scene, cubemapCamera);
        }
        if (instancesRenderer) {
            instancesRenderer->render(scene, cubemapCamera);
        }
        if (skyboxRenderer) {
            skyboxRenderer->render(scene, cubemapCamera);
        }
        if (transparentRenderer) {
            transparentRenderer->render(scene, cubemapCamera);
        }
    }

} // OGL::E1