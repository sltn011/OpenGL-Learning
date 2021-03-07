#include "NormalRenderer.hpp"

namespace OGL::E1 {

    NormalRenderer::NormalRenderer( 
        Shader &&shader
    ) : m_shader{ std::move(shader) } {

    }

    void NormalRenderer::render( 
        Scene &scene, 
        BasicCamera const *camera
    ) {
        m_shader.use();
        m_shader.setUniformMatrix4("view", camera->getViewMatrix());
        m_shader.setUniformMatrix4("projection", camera->getProjectionMatrix());
        m_shader.setUniformVec3("viewerPos", camera->getPos());

        dirLights &sceneDirLights = scene.getDirLights();
        m_shader.setUniformInt("numDirLights", sceneDirLights.size());
        for (size_t i = 0; i < sceneDirLights.size(); ++i) {
            auto &[light, pShadowMap] = sceneDirLights[i];
            light.loadInShader(m_shader, i);
            if (!pShadowMap) {
                continue;
            }
            pShadowMap->bindTexture();
            m_shader.setUniformMatrix4("dirLightProjView[" + std::to_string(i) + "]", pShadowMap->lightProjView());
            m_shader.setUniformInt("dirLightShadowMap[" + std::to_string(i) + "]", pShadowMap->textureUnit() - GL_TEXTURE0);
        }

        pointLights &scenePointLights = scene.getPointLights();
        m_shader.setUniformInt("numPointLights", scenePointLights.size());
        for (size_t i = 0; i < scenePointLights.size(); ++i) {
            auto &[light, pShadowCubemap] = scenePointLights[i];
            light.loadInShader(m_shader, i);
            if (!pShadowCubemap) {
                continue;
            }
            pShadowCubemap->bindTexture();
            m_shader.setUniformInt("pointLightShadowMap[" + std::to_string(i) + "]", pShadowCubemap->textureUnit() - GL_TEXTURE0);
            m_shader.setUniformFloat("pointLightShadowMapFarPlane[" + std::to_string(i) + "]", pShadowCubemap->farPlane());
        }

        spotLights &sceneSpotLights = scene.getSpotLights();
        m_shader.setUniformInt("numSpotLights", sceneSpotLights.size());
        for (size_t i = 0; i < sceneSpotLights.size(); ++i) {
            auto &[light, pShadowMap] = sceneSpotLights[i];
            light.loadInShader(m_shader, i);
            if (!pShadowMap) {
                continue;
            }
            pShadowMap->bindTexture();
            m_shader.setUniformMatrix4("spotLightProjView[" + std::to_string(i) + "]", pShadowMap->lightProjView());
            m_shader.setUniformInt("spotLightShadowMap[" + std::to_string(i) + "]", pShadowMap->textureUnit() - GL_TEXTURE0);
        }

        for (auto const &obj : scene.getNormalObjs()) {
            obj.draw(m_shader);
        }
    }

    void NormalRenderer::setShader( 
        Shader &&shader
    ) {
        m_shader = std::move(shader);
    }

    Shader &NormalRenderer::getShader(
    ) {
        return m_shader;
    }

} // OGL::E1