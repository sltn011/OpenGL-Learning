#include "OGL_E1.hpp"

class HDRTest : public OGL::E1::Engine1Base {
public:
    HDRTest(
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

        m_normalRenderer = OGL::E1::NormalRenderer{ std::move(normalShader) };
        m_transpRenderer = OGL::E1::TransparentRenderer{ std::move(transpShader) };
        m_mirrorRenderer = OGL::E1::MirrorRenderer{ std::move(mirrorShader) };
        m_cubemapRenderer = OGL::E1::CubemapRenderer{};
        m_instancesRenderer = OGL::E1::InstancesRenderer{ std::move(instancesShader) };
        m_shadowMapRenderer = OGL::E1::ShadowMapRenderer{ std::move(shadowMapRender) };
        m_shadowCubemapRenderer = OGL::E1::ShadowCubemapRenderer{ std::move(shadowCubemapRenderer) };
        m_lightSourcesDebugRenderer = OGL::E1::LightSourcesDebugRenderer{ std::move(lightSourcesRenderer), 0.005f };
        m_coloredShapesRenderer = OGL::E1::ColoredShapesRenderer{ std::move(coloredShapesShader) };
        m_guiRenderer = OGL::E1::GUI::GUIRenderer{ m_window, "#version 330" };


        loadLevel("levels/04-HDR.json");

        rebuildShadows();

        rebuildReflections();

        OGL::Shader HDRShader("shaders/26-HDR.vert", "shaders/26-HDR.frag");
        initPostprocessing(std::move(HDRShader));
        togglePostprocessing(true);

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
    HDRTest app(1920, 1080);
    app.start();
    return 0;
}