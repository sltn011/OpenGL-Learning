#include "ShadowCubemapRenderer.hpp"

namespace OGL::E1 {

    ShadowCubemapRenderer::ShadowCubemapRenderer(
        Shader &&shader
    ) : m_shader{ std::move(shader) } {

    }

    ShadowCubemap ShadowCubemapRenderer::render(
        Scene &scene,
        CameraShadowCubemap camera,
        GLenum mapTextureUnit,
        int mapSize
    ) {
        std::array<glm::mat4, 6> lightProjViews;
        for (int i = 0; i < 6; ++i) {
            camera.setSide(i);
            glm::mat4 proj = camera.getProjectionMatrix();
            glm::mat4 view = camera.getViewMatrix();
            lightProjViews[i] = proj * view;
        }

        ShadowCubemap cubemap(mapSize, mapTextureUnit, camera.m_far, lightProjViews);
        cubemap.bindFramebuffer();
        glClear(GL_DEPTH_BUFFER_BIT);
        m_shader.use();
        for (int i = 0; i < 6; ++i) {
            m_shader.setUniformMatrix4("lightProjView[" + std::to_string(i) + "]", cubemap.lightProjView(i));
        }
        m_shader.setUniformVec3("lightPos", camera.getPos());
        m_shader.setUniformFloat("farPlane", camera.m_far);

        for (auto &obj : scene.getNormalObjs()) {
            obj.draw(m_shader);
        }
        for (auto &obj : scene.getTransparentObjs()) {
            obj.draw(m_shader);
        }
        for (auto &pObjCubemap : scene.getMirrorObjs()) {
            pObjCubemap.first.draw(m_shader);
        }

        cubemap.unbindFramebuffer();
        return cubemap;
    }

    void ShadowCubemapRenderer::setShader(
        Shader &&shader
    ) {
        m_shader = std::move(shader);
    }

    Shader &ShadowCubemapRenderer::getShader(
    ) {
        return m_shader;
    }

} // OGL::E1