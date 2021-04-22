#include "OGL_E1.hpp"

class EngineImGuiTest : public OGL::E1::Engine1Base {
 public:
     EngineImGuiTest(
         int width,
         int height,
         std::string name
     ) : OGL::E1::Engine1Base{
         width,
         height,
         name
     } {
     }

     bool userCreate(
     ) override {

         int screenWidth, screenHeight;
         glfwGetFramebufferSize(m_window, &screenWidth, &screenHeight);

         OGL::E1::smartCamPtr gameCamera = OGL::E1::factory<OGL::CameraFree>(
             glm::vec3{ 0.0f, 0.0f, 5.0f },
             glm::vec3{ 0.0f, 0.0f, -5.0f },
             glm::vec3{ 0.0f, 1.0f, 0.0f },
             1.0f,
             45.0f,
             static_cast<float>(screenWidth) / static_cast<float>(screenHeight),
             0.01f,
             100.0f
         );

         m_scene = OGL::E1::factory<OGL::E1::Scene>(std::move(gameCamera));

         OGL::Shader normalShader(
             "shaders/02-imguiTest.vert",
             "shaders/02-imguiTest.frag"
         );
         m_normalRenderer = OGL::E1::factory<OGL::E1::NormalRenderer>(std::move(normalShader));

         OGL::Shader coloredShapesShader(
             "shaders/01-coloredShapes.vert",
             "shaders/01-coloredShapes.frag"
         );
         m_coloredShapesRenderer = OGL::E1::factory<OGL::E1::ColoredShapesRenderer>(std::move(coloredShapesShader));

         // Models
         addModel("models/Axises/axises.obj", 0);

         addNormalObject(0, glm::vec3{ 0.0f, 0.0f, 0.0f });
         addNormalObject(0, glm::vec3{ 1.0f, 0.0f, 1.0f });
         addNormalObject(0, glm::vec3{ -1.0f, 0.0f, -1.0f });
         addNormalObject(0, glm::vec3{ -1.0f, 1.0f, 1.0f });
         addNormalObject(0, glm::vec3{ -1.0f, -1.0f, 1.0f });
         addNormalObject(0, glm::vec3{ 1.0f, 1.0f, -1.0f });

         addDirLight({ -0.3f, -0.5f, -0.2f });

         glClearColor(0.6f, 0.6f, 0.7f, 1.0f);

         m_normalRenderer->getShader().use();
         m_normalRenderer->getShader().setUniformVec3("objectColor", { 0.5f, 0.8f, 0.2f });

         m_guiRenderer = OGL::E1::factory<OGL::E1::GUI::GUIRenderer>(
             m_window,
             "#version 330"
         );

         return true;
     }

     bool userFrameUpdate(
         float elapsedTime
     ) override {
         processInputPerFrame();

         m_normalRenderer->render(*m_scene, m_scene->getCamera().get());
         
         return true;
     }

     bool userDestroy(
     ) override {
         return true;
     }
};

int main() {
    stbi_set_flip_vertically_on_load(true);
    EngineImGuiTest engine(1920, 1080, "ImGui Tests");
    engine.start();

    return 0;
}
