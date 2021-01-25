#include "NormalRenderer.hpp"

namespace OGL::E1 {

    NormalRenderer::NormalRenderer( 
        Shader &&shader
    ) : m_shader{ std::move(shader) } {

    }

    void NormalRenderer::render( 
        Scene &scene, 
        smartCamPtr const &camera
    ) {
        m_shader.use();
        m_shader.setUniformMatrix4("view", camera->getViewMatrix());
        m_shader.setUniformMatrix4("projection", camera->getProjectionMatrix());
        m_shader.setUniformVec3("viewerPos", camera->getPos());

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

        for (auto const &obj : scene.getNormalObjs()) {
            obj->draw(m_shader);
        }
    }

    void NormalRenderer::setShader( 
        Shader &&shader
    ) {
        m_shader = std::move(shader);
    }

} // OGL::E1