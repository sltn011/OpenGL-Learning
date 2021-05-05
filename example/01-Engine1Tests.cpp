#include "OGL_E1.hpp"

const int shadowMapDirLightFirstTextureID = 10;
const int shadowCubemapFirstTextureID = shadowMapDirLightFirstTextureID + 4;
const int shadowMapSpotLightFirstTextureID = shadowCubemapFirstTextureID + 4;
const int skyboxTextureID = 22;
const int cubemapTextureID = 23;


const int cubemapSize = 512;
const int shadowMapSize = 1024 * 2;
const int shadowCubemapSize = 1024;


class Test : public OGL::E1::Engine1Base {
public:
    Test(
        int width,
        int height
    ) : Engine1Base{
        width,
        height
    } {
    }

    bool userCreate(
    ) override {

        int screenWidth, screenHeight;
        glfwGetFramebufferSize(m_window, &screenWidth, &screenHeight);

        OGL::Shader normalShader(
            "shaders/01-playgroundObj.vert",
            "shaders/01-playgroundObj.frag"
        );

        OGL::Shader transpShader(
            "shaders/01-playgroundObj.vert",
            "shaders/01-playgroundObj.frag"
        );

        OGL::Shader skyboxShader(
            "shaders/01-playgroundSkybox.vert",
            "shaders/01-playgroundSkybox.frag"
        );

        OGL::Shader mirrorShader(
            "shaders/01-playgroundMirror.vert",
            "shaders/01-playgroundMirror.frag"
        );

        OGL::Shader instancesShader(
            "shaders/01-playgroundInstances.vert",
            "shaders/01-playgroundInstances.frag"
        );

        OGL::Shader shadowMapRender(
            "shaders/01-playgroundShadowMapRenderer.vert",
            "shaders/01-playgroundShadowMapRenderer.frag"
        );

        OGL::Shader shadowCubemapRenderer(
            "shaders/01-playgroundShadowCubemapRenderer.vert",
            "shaders/01-playgroundShadowCubemapRenderer.geom",
            "shaders/01-playgroundShadowCubemapRenderer.frag"
        );

        OGL::Shader lightSourcesRenderer(
            "shaders/01-playgroundLightSourcesRenderer.vert",
            "shaders/01-playgroundLightSourcesRenderer.frag"
        );

        OGL::Shader coloredShapesShader(
            "shaders/01-coloredShapes.vert",
            "shaders/01-coloredShapes.frag"
        );

        m_normalRenderer           .emplace(std::move(normalShader));
        m_transpRenderer           .emplace(std::move(transpShader));
        m_skyboxRenderer           .emplace(std::move(skyboxShader));
        m_mirrorRenderer           .emplace(std::move(mirrorShader));
        m_cubemapRenderer          .emplace();
        m_instancesRenderer        .emplace(std::move(instancesShader));
        m_shadowMapRenderer        .emplace(std::move(shadowMapRender));
        m_shadowCubemapRenderer    .emplace(std::move(shadowCubemapRenderer));
        m_lightSourcesDebugRenderer.emplace(std::move(lightSourcesRenderer), 0.005f);
        m_coloredShapesRenderer    .emplace(std::move(coloredShapesShader));
        m_guiRenderer              .emplace(m_window, "#version 330" );


        loadLevel("levels/01-level.json");


        glViewport(0, 0, shadowMapSize, shadowMapSize);
        for (size_t i = 0; i < m_scene->getDirLights().size(); ++i) {
            auto &[dirLight, shadowMap] = m_scene->getDirLights()[i];
            OGL::CameraShadowMap cam{ dirLight, m_scene->getNormalObjs().front().getPosition(), 2.5f, 0.1f, 6.0f };
            shadowMap = m_shadowMapRenderer->render(
                *m_scene,
                cam,
                GL_TEXTURE0 + shadowMapDirLightFirstTextureID + i,
                shadowMapSize
            );
        }

        glViewport(0, 0, shadowCubemapSize, shadowCubemapSize);
        for (size_t i = 0; i < m_scene->getPointLights().size(); ++i) {
            auto &[pointLight, shadowCubemap] = m_scene->getPointLights()[i];
            OGL::CameraShadowCubemap cam(pointLight, 0.01f, 3.5f);
            shadowCubemap = m_shadowCubemapRenderer->render(
                *m_scene,
                cam,
                GL_TEXTURE0 + shadowCubemapFirstTextureID + i,
                shadowCubemapSize
            );
        }

        glViewport(0, 0, shadowMapSize, shadowMapSize);
        for (size_t i = 0; i < m_scene->getSpotLights().size(); ++i) {
            auto &[spotLight, shadowMap] = m_scene->getSpotLights()[i];
            OGL::CameraShadowMap cam{ spotLight, 0.1f, 6.0f };
            shadowMap = m_shadowMapRenderer->render(
                *m_scene,
                cam,
                GL_TEXTURE0 + shadowMapSpotLightFirstTextureID + i,
                shadowMapSize
            );
        }

        glViewport(0, 0, cubemapSize, cubemapSize);
        for (auto &p : m_scene->getMirrorObjs()) {
            p.second = m_cubemapRenderer->render(
                *m_scene,
                cubemapSize,
                GL_TEXTURE0 + cubemapTextureID,
                p.first.getPosition(),
                m_normalRenderer,
                m_skyboxRenderer,
                m_transpRenderer,
                m_instancesRenderer
            );
        }
        glViewport(0, 0, screenWidth, screenHeight);

        return true;
    }

    bool userFrameUpdate(
        float elapsedTime
    ) override {
        processInputPerFrame();

        m_normalRenderer->render(*m_scene, m_scene->getCamera().get());

        m_mirrorRenderer->render(*m_scene, m_scene->getCamera().get());

        m_instancesRenderer->render(*m_scene, m_scene->getCamera().get());

        m_lightSourcesDebugRenderer->render(*m_scene, m_scene->getCamera().get());

        m_skyboxRenderer->render(*m_scene, m_scene->getCamera().get());

        m_transpRenderer->render(*m_scene, m_scene->getCamera().get());

        return true;
    }

    bool userDestroy(
    ) override {
        return true;
    }
};

int main(
) {
    stbi_set_flip_vertically_on_load(true);

    Test t(1920, 1080);
    t.start();
    return 0;
}