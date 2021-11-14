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


        loadLevel("levels/05-gBuffer.json");

        rebuildShadows();

        rebuildReflections();

        OGL::Shader HDRShader("shaders/27-postprocessing.vert", "shaders/27-postprocessing.frag");
        initPostprocessing(std::move(HDRShader));
        togglePostprocessing(true);

        OGL::Shader MipmapShader(
            "shaders/27-bloomMipmap.vert",
            "shaders/27-bloomMipmap.frag"
        );

        OGL::Shader CombineShader(
            "shaders/27-bloomCombine.vert",
            "shaders/27-bloomCombine.frag"
        );

        m_bloom = OGL::E1::Bloom{ std::move(MipmapShader), std::move(CombineShader), glm::vec3(1.0f) };
        m_bloom->initFrameBuffers(screenWidth, screenHeight);

        m_gBuffer = OGL::GBuffer(screenWidth, screenHeight);

        m_gBufferWriteShader = OGL::Shader(
            "shaders/28-gBufferWrite.vert",
            "shaders/28-gBufferWrite.frag"
        );

        m_gBufferReadShader = OGL::Shader(
            "shaders/28-gBufferRead.vert",
            "shaders/28-gBufferRead.frag"
        );

        return true;
    }

    bool userFrameUpdate(
        float elapsedTime
    ) override {
        processInputPerFrame();

        for (OGL::Object &obj : m_scene->getNormalObjs()) {
            obj.draw(*m_gBufferWriteShader);
        }

        //for (OGL::Object &obj : m_scene->getTransparentObjs()) {
        //    obj.draw(*m_gBufferWriteShader);
        //}

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