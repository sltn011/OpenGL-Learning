#include "First OGL Engine/OGL_E1.hpp"
#include "CameraStatic.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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


         IMGUI_CHECKVERSION();
         ImGui::CreateContext();
         ImGuiIO &io = ImGui::GetIO();
         ImGui_ImplGlfw_InitForOpenGL(m_window, true);
         ImGui_ImplOpenGL3_Init("#version 330");
         ImGui::StyleColorsDark();


         OGL::E1::smartCamPtr gameCamera = OGL::E1::factory<OGL::CameraStatic>(
             glm::vec3{ 0.0f, 3.15f, 5.0f },
             glm::vec3{ 0.0f, -3.15f, -5.0f },
             glm::vec3{ 0.0f, 1.0f, 0.0f },
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

         // Models
         addModel("models/Axises/axises.obj", 0);

         glm::vec3 axisesPos{ 0.0f, 0.0f, 0.0f };
         addNormalObject(0, axisesPos);

         addDirLight({ -0.3f, -0.5f, -0.2f });

         glClearColor(0.6f, 0.6f, 0.7f, 1.0f);

         glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

         m_normalRenderer->getShader().use();
         m_normalRenderer->getShader().setUniformVec3("objectColor", { 0.5f, 0.8f, 0.2f });

         return true;
     }

     bool userFrameUpdate(
         float elapsedTime
     ) override {
         processInputPerFrame();

         ImGui_ImplOpenGL3_NewFrame();
         ImGui_ImplGlfw_NewFrame();
         ImGui::NewFrame();

         OGL::Object &obj = m_scene->getNormalObjs()[0];
         glm::vec3 objPos = obj.getPosition();
         float     objScale = obj.getScale();
         glm::vec3 objEulerAngles = glm::degrees(glm::eulerAngles(obj.getRotation()));

         ImGui::Begin("GUI Window");
         if (ImGui::InputFloat3("Position", &(objPos.x))) {
             obj.setPosition(objPos);
         }
         if (ImGui::InputFloat("Scale", &(objScale))) {
             obj.setScale(objScale);
         }
         if (ImGui::InputFloat3("Rotation", &(objEulerAngles.x))) {
             obj.setRotation(objEulerAngles);
         }
         ImGui::End();

         ImGui::Render();
         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

         m_normalRenderer->render(*m_scene, m_scene->getCamera().get());

         return true;
     }

     bool userDestroy(
     ) override {
         ImGui_ImplOpenGL3_Shutdown();
         ImGui_ImplGlfw_Shutdown();
         ImGui::DestroyContext();
         return true;
     }
};

int main() {
    stbi_set_flip_vertically_on_load(true);

    EngineImGuiTest engine(1920, 1080, "ImGui Tests");
    engine.start();

    return 0;
}
