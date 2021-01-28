#include "TransparentRenderer.hpp"

namespace OGL::E1 {

    TransparentRenderer::TransparentRenderer( 
        Shader &&shader
    ) : m_shader{ std::move(shader) } {

    }

    void TransparentRenderer::render( 
        Scene &scene, 
        BasicCamera const *camera
    ) {
        m_shader.use();
        m_shader.setUniformMatrix4("view", camera->getViewMatrix());
        m_shader.setUniformMatrix4("projection", camera->getProjectionMatrix());
        glm::vec3 camPos = camera->getPos();
        m_shader.setUniformVec3("viewerPos", camPos);

        dirLights &sceneDirLights = scene.getDirLights();
        m_shader.setUniformInt("numDirLights", sceneDirLights.size());
        for (size_t i = 0; i < sceneDirLights.size(); ++i) {
            sceneDirLights[i].loadInShader(m_shader, i);
        }

        pointLights &scenePointLights = scene.getPointLights();
        m_shader.setUniformInt("numPointLights", scenePointLights.size());
        for (size_t i = 0; i < scenePointLights.size(); ++i) {
            scenePointLights[i].loadInShader(m_shader, i);
        }

        spotLights &sceneSpotLights = scene.getSpotLights();
        m_shader.setUniformInt("numSpotLights", sceneSpotLights.size());
        for (size_t i = 0; i < sceneSpotLights.size(); ++i) {
            sceneSpotLights[i].loadInShader(m_shader, i);
        }

        std::map<float, Object const&> objectsMap;
        for (auto const &obj : scene.getTransparentObjs()) {
            float dist = glm::distance(camPos, obj.m_postiton);
            objectsMap.emplace(dist, obj);
        }

        for (auto it = objectsMap.rbegin(); it != objectsMap.rend(); ++it) {
            it->second.draw(m_shader);
        }
    }

    void TransparentRenderer::setShader( 
        Shader &&shader
    ) {
        m_shader = std::move(shader);
    }

} // OGL::E1