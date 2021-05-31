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

         loadLevel("levels/02-level.json");

         OGL::Shader normalShader(
             "shaders/02-imguiTest.vert",
             "shaders/02-imguiTest.frag"
         );
         m_normalRenderer.emplace(std::move(normalShader));

         OGL::Shader coloredShapesShader(
             "shaders/01-coloredShapes.vert",
             "shaders/01-coloredShapes.frag"
         );
         m_coloredShapesRenderer.emplace(std::move(coloredShapesShader));

         glClearColor(0.6f, 0.6f, 0.7f, 1.0f);

         m_normalRenderer->getShader().use();
         m_normalRenderer->getShader().setUniformVec3("objectColor", { 0.5f, 0.8f, 0.2f });

         m_guiRenderer.emplace(m_window, "#version 330");

         saveLevel("levels/02-level.json");

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
