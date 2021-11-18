#include "OGL_E1.hpp"

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

        stbi_set_flip_vertically_on_load(false);

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

        OGL::Shader postprocessingShader(
            "shaders/01-postprocessing.vert",
            "shaders/01-postprocessing.frag"
        );

        m_normalRenderer            = OGL::E1::NormalRenderer{ std::move(normalShader) };
        m_transpRenderer            = OGL::E1::TransparentRenderer{ std::move(transpShader) };
        m_skyboxRenderer            = OGL::E1::SkyboxRenderer{std::move(skyboxShader)};
        m_mirrorRenderer            = OGL::E1::MirrorRenderer{ std::move(mirrorShader) };
        m_cubemapRenderer           = OGL::E1::CubemapRenderer{};
        m_instancesRenderer         = OGL::E1::InstancesRenderer{ std::move(instancesShader) };
        m_shadowMapRenderer         = OGL::E1::ShadowMapRenderer{ std::move(shadowMapRender) };
        m_shadowCubemapRenderer     = OGL::E1::ShadowCubemapRenderer{ std::move(shadowCubemapRenderer) };
        m_lightSourcesDebugRenderer = OGL::E1::LightSourcesDebugRenderer{ std::move(lightSourcesRenderer), 0.05f };
        m_coloredShapesRenderer     = OGL::E1::ColoredShapesRenderer{ std::move(coloredShapesShader) };
        m_guiRenderer               = OGL::E1::GUI::GUIRenderer{ m_window, "#version 330" };


        OGL::Shader bloomDownsamplingShader(
            "shaders/bloomDownsampling.vert",
            "shaders/bloomDownsampling.frag"
        );

        OGL::Shader blurHorizontalShader(
            "shaders/blurHorizontal.vert",
            "shaders/blurHorizontal.frag"
        );

        OGL::Shader blurVerticalShader(
            "shaders/blurVertical.vert",
            "shaders/blurVertical.frag"
        );

        OGL::Shader bloomCombineShader(
            "shaders/bloomCombine.vert",
            "shaders/bloomCombine.frag"
        );

        m_bloom = OGL::E1::Bloom{
            std::move(bloomDownsamplingShader),
            std::move(blurHorizontalShader),
            std::move(blurVerticalShader),
            std::move(bloomCombineShader),
            glm::vec3(1.0f)
        };
        m_bloom->initFrameBuffers(screenWidth, screenHeight);

        initPostprocessing(std::move(postprocessingShader));
        togglePostprocessing(true);


        loadLevel("levels/03-level.json");

        rebuildShadows();

        rebuildReflections();

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
    Test t(1920, 1080);
    t.start();
    return 0;
}