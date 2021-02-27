#include "MirrorRenderer.hpp"

namespace OGL::E1 {

    MirrorRenderer::MirrorRenderer( 
        Shader &&shader
    ) : m_shader{ std::move(shader) } {

    }

    void MirrorRenderer::render(
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
            pShadowMap->bindTexture();
            m_shader.setUniformMatrix4("dirLightProjView[" + std::to_string(i) + "]", pShadowMap->lightProjView());
            m_shader.setUniformInt("dirLightShadowMap[" + std::to_string(i) + "]", pShadowMap->textureUnit() - GL_TEXTURE0);
        }

        pointLights &scenePointLights = scene.getPointLights();
        m_shader.setUniformInt("numPointLights", scenePointLights.size());
        for (size_t i = 0; i < scenePointLights.size(); ++i) {
            auto &[light, pShadowCubemap] = scenePointLights[i];
            light.loadInShader(m_shader, i);
            pShadowCubemap->bindTexture();
            m_shader.setUniformInt("pointLightShadowMap[" + std::to_string(i) + "]", pShadowCubemap->textureUnit() - GL_TEXTURE0);
            m_shader.setUniformFloat("pointLightShadowMapFarPlane[" + std::to_string(i) + "]", pShadowCubemap->farPlane());
        }

        spotLights &sceneSpotLights = scene.getSpotLights();
        m_shader.setUniformInt("numSpotLights", sceneSpotLights.size());
        for (size_t i = 0; i < sceneSpotLights.size(); ++i) {
            sceneSpotLights[i].loadInShader(m_shader, i);
        }

        for (auto const &pObjCubemap : scene.getMirrorObjs()) {
            if (!pObjCubemap.second) {
                continue;
            }
            m_shader.setUniformInt("cubemapSampler", pObjCubemap.second->unit() - GL_TEXTURE0);
            pObjCubemap.second->bind();
            pObjCubemap.first.draw(m_shader);
            Cubemap::unbind();
        }
    }

    void MirrorRenderer::setShader( 
        Shader &&shader
    ) {
        m_shader = std::move(shader);
    }

    Shader &MirrorRenderer::getShader(
    ) {
        return m_shader;
    }

} // OGL::E1