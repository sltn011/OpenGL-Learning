#include "ShadowMapRenderer.hpp"

namespace OGL::E1 {

    ShadowMapRenderer::ShadowMapRenderer(
        Shader &&shader
    ) : m_shader{ std::move(shader) } {

    }

    ShadowMap ShadowMapRenderer::render(
        Scene &scene,
        CameraShadowMap camera,
        GLenum mapTextureUnit,
        int mapSize
    ) {
        glm::mat4 proj = camera.getProjectionMatrix();
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 lightProjView = proj * view;

        ShadowMap map(mapTextureUnit, mapSize, lightProjView);
        map.bindFramebuffer();
        glClear(GL_DEPTH_BUFFER_BIT);
        m_shader.use();
        m_shader.setUniformMatrix4("lightProjView", map.lightProjView());

        for (auto &obj : scene.getNormalObjs()) {
            obj.drawShape(m_shader);
        }
        for (auto &obj : scene.getTransparentObjs()) {
            obj.drawShape(m_shader);
        }
        for (auto &pObjCubemap : scene.getMirrorObjs()) {
            pObjCubemap.first.drawShape(m_shader);
        }

        map.unbindFramebuffer();
        return map;
    }

    void ShadowMapRenderer::setShader(
        Shader &&shader
    ) {
        m_shader = std::move(shader);
    }

    Shader &ShadowMapRenderer::getShader(
    ) {
        return m_shader;
    }

} // OGL::E1