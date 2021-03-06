#include "SkyboxRenderer.hpp"

namespace OGL::E1 {

    SkyboxRenderer::SkyboxRenderer( 
        Shader &&shader
    ) : m_shader{ std::move(shader) } {

    }

    void SkyboxRenderer::render(
        Scene &scene,
        BasicCamera const *camera
    ) {
        maybeSkybox &skybox = scene.getSkybox();
        if (!skybox) {
            return;
        }

        glm::mat4 viewMatr = camera->getViewMatrix();
        glm::mat4 viewMatrNoTranslation = glm::mat4(glm::mat3(viewMatr));

        m_shader.use();
        m_shader.setUniformMatrix4("view", viewMatrNoTranslation);
        m_shader.setUniformMatrix4("projection", camera->getProjectionMatrix());

        m_shader.setUniformInt("skyboxSampler", skybox->unit() - GL_TEXTURE0);

        skybox->bind();
        glDepthFunc(GL_LEQUAL);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
        glDepthFunc(GL_LESS);
        Skybox::unbind();
    }

    void SkyboxRenderer::setShader(
        Shader &&shader
    ) {
        m_shader = std::move(shader);
    }

    Shader &SkyboxRenderer::getShader(
    ) {
        return m_shader;
    }

} // OGL::E1