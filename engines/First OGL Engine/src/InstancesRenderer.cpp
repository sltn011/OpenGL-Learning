#include "InstancesRenderer.hpp"

namespace OGL::E1 {

    InstancesRenderer::InstancesRenderer(
        Shader &&shader
    ) : m_shader{ std::move(shader) } {

    }

    void InstancesRenderer::render(
        Scene &scene,
        BasicCamera const *camera
    ) {
        m_shader.use();
        m_shader.setUniformMatrix4("view", camera->getViewMatrix());
        m_shader.setUniformMatrix4("projection", camera->getProjectionMatrix());
        m_shader.setUniformVec3("viewerPos", camera->getPos());

        dirLights &sceneDirLights = scene.getDirLights();
        m_shader.setUniformInt("numDirLights", static_cast<int>(sceneDirLights.size()));
        for (size_t i = 0; i < sceneDirLights.size(); ++i) {
            sceneDirLights[i].first.loadInShader(m_shader, static_cast<int>(i));
        }

        pointLights &scenePointLights = scene.getPointLights();
        m_shader.setUniformInt("numPointLights", static_cast<int>(scenePointLights.size()));
        for (size_t i = 0; i < scenePointLights.size(); ++i) {
            scenePointLights[i].first.loadInShader(m_shader, static_cast<int>(i));
        }

        spotLights &sceneSpotLights = scene.getSpotLights();
        m_shader.setUniformInt("numSpotLights", static_cast<int>(sceneSpotLights.size()));
        for (size_t i = 0; i < sceneSpotLights.size(); ++i) {
            sceneSpotLights[i].first.loadInShader(m_shader, static_cast<int>(i));
        }

        for (auto const &[obj, numInstances] : scene.getInstancedObjs()) {
            obj.drawInstanced(m_shader, numInstances);
        }
    }

    void InstancesRenderer::setShader(
        Shader &&shader
    ) {
        m_shader = std::move(shader);
    }

    Shader &InstancesRenderer::getShader(
    ) {
        return m_shader;
    }

} // OGL::E1