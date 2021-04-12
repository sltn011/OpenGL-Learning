#include "OGL_E1.hpp"

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

     glm::vec3          m_objPos = glm::vec3{ 0.0f, 0.0f, 0.0f };
     float              m_objScale = 1.0f;
     glm::vec3          m_objEulerAngles = glm::vec3{ 0.0f, 0.0f, 0.0f };

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

         // Models
         addModel("models/Axises/axises.obj", 0);

         glm::vec3 axisesPos{ 0.0f, 0.0f, 0.0f };
         addNormalObject(0, axisesPos, 1.0f);

         addDirLight({ -0.3f, -0.5f, -0.2f });

         glClearColor(0.6f, 0.6f, 0.7f, 1.0f);

         m_normalRenderer->getShader().use();
         m_normalRenderer->getShader().setUniformVec3("objectColor", { 0.5f, 0.8f, 0.2f });

         return true;
     }

     bool userFrameUpdate(
         float elapsedTime
     ) override {
         processInputPerFrame();

         OGL::Object &obj = m_scene->getNormalObjs()[0];
         m_objPos = obj.getPosition();
         m_objScale = obj.getScale();
         m_objEulerAngles = obj.getRotationAngles();

         m_normalRenderer->render(*m_scene, m_scene->getCamera().get());


         if (m_showGUI) {
             ImGui_ImplOpenGL3_NewFrame();
             ImGui_ImplGlfw_NewFrame();
             ImGui::NewFrame();

             ImGui::Begin("GUI Window");
             ImGui::Text(obj.getName().c_str());
             ImGui::NewLine();
             ImGui::Text("Position");
             if (ImGui::InputFloat("X##Post", &(m_objPos.x))) {
                 obj.setPosition(m_objPos);
             }
             if (ImGui::InputFloat("Y##Post", &(m_objPos.y))) {
                 obj.setPosition(m_objPos);
             }
             if (ImGui::InputFloat("Z##Post", &(m_objPos.z))) {
                 obj.setPosition(m_objPos);
             }
             ImGui::NewLine();
             ImGui::Text("Scale");
             if (ImGui::InputFloat("##Scale", &(m_objScale))) {
                 obj.setScale(m_objScale);
             }
             ImGui::NewLine();
             ImGui::Text("Rotation (Order YXZ)");
             if (ImGui::InputFloat("Y##Rotation", &(m_objEulerAngles.y))) {
                 obj.setRotation(m_objEulerAngles);
             }
             if (ImGui::InputFloat("X##Rotation", &(m_objEulerAngles.x))) {
                 obj.setRotation(m_objEulerAngles);
             }
             if (ImGui::InputFloat("Z##Rotation", &(m_objEulerAngles.z))) {
                 obj.setRotation(m_objEulerAngles);
             }
             ImGui::End();



             ImGui::Render();
             ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
         }
         

         return true;
     }

     bool userDestroy(
     ) override {
         ImGui_ImplOpenGL3_Shutdown();
         ImGui_ImplGlfw_Shutdown();
         ImGui::DestroyContext();
         return true;
     }

     void keyCallback(
         int key,
         int scancode,
         int action,
         int mods
     ) {
         if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
             glfwSetWindowShouldClose(m_window, true);
         }
         if (key == GLFW_KEY_RIGHT_CONTROL && action == GLFW_PRESS) {
             m_showGUI = !m_showGUI;
             if (m_showGUI) {
                 glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
             }
             else {
                 glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                 glfwSetCursorPos(m_window, m_system.lastMouseXPos, m_system.lastMouseYPos);
             }
         }
     }
};

int main() {
    stbi_set_flip_vertically_on_load(true);
    
    EngineImGuiTest engine(1920, 1080, "ImGui Tests");
    engine.start();

    return 0;
}
