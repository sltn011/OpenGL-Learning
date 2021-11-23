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

        OGL::Shader shadowMapRender(
            "shaders/01-playgroundShadowMapRenderer.vert",
            "shaders/01-playgroundShadowMapRenderer.frag"
        );

        OGL::Shader shadowCubemapRenderer(
            "shaders/01-playgroundShadowCubemapRenderer.vert",
            "shaders/01-playgroundShadowCubemapRenderer.geom",
            "shaders/01-playgroundShadowCubemapRenderer.frag"
        );

        OGL::Shader coloredShapesShader(
            "shaders/01-coloredShapes.vert",
            "shaders/01-coloredShapes.frag"
        );

        m_normalRenderer = OGL::E1::NormalRenderer{ std::move(normalShader) };
        m_transpRenderer = OGL::E1::TransparentRenderer{ std::move(transpShader) };
        m_shadowMapRenderer = OGL::E1::ShadowMapRenderer{ std::move(shadowMapRender) };
        m_shadowCubemapRenderer = OGL::E1::ShadowCubemapRenderer{ std::move(shadowCubemapRenderer) };
        m_coloredShapesRenderer = OGL::E1::ColoredShapesRenderer{ std::move(coloredShapesShader) };
        m_guiRenderer = OGL::E1::GUI::GUIRenderer{ m_window, "#version 330" };

        OGL::Shader HDRShader("shaders/27-postprocessing.vert", "shaders/27-postprocessing.frag");
        initPostprocessing(std::move(HDRShader));
        togglePostprocessing(true);

        m_gBuffer = OGL::GBuffer(screenWidth, screenHeight);

        m_gBufferWriteShader = OGL::Shader(
            "shaders/29-SSAOgBufferWrite.vert",
            "shaders/29-SSAOgBufferWrite.frag"
        );

        m_gBufferReadShader = OGL::Shader(
            "shaders/29-SSAOgBufferRead.vert",
            "shaders/29-SSAOgBufferRead.frag"
        );

        m_SSAOShader = OGL::Shader(
            "shaders/29-SSAO.vert",
            "shaders/29-SSAO.frag"
        );

        OGL::Shader blurHorizontalShader(
            "shaders/blurHorizontal.vert",
            "shaders/blurHorizontal.frag"
        );

        OGL::Shader blurVerticalShader(
            "shaders/blurVertical.vert",
            "shaders/blurVertical.frag"
        );

        initBlur(std::move(blurHorizontalShader), std::move(blurVerticalShader));

        loadLevel("levels/06-SSAO.json");

        rebuildShadows();

        rebuildReflections();

        return true;
    }

    bool userFrameUpdate(
        float elapsedTime
    ) override {
        processInputPerFrame();

        for (OGL::Object &obj : m_scene->getNormalObjs()) {
            obj.draw(*m_gBufferWriteShader);
        }

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