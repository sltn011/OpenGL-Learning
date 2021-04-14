#include "GUI/GUIRenderer.hpp"

namespace OGL::E1::GUI {

    GUIRenderer::GUIRenderer(
        GLFWwindow *window, 
        std::string glslVersion
    ) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glslVersion.c_str());
        ImGui::StyleColorsDark();
    }

    GUIRenderer::~GUIRenderer(
    ) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    std::unordered_map<WindowsType, std::unique_ptr<BasicWindow>> &GUIRenderer::getWindows(
    ){
        return m_windows;
    }

    void GUIRenderer::addWindow(
        std::unique_ptr<BasicWindow> window,
        WindowsType windowType
    ) {
        m_windows.emplace(windowType, std::move(window));
    }

    void GUIRenderer::render(
    ) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        for (auto &[type, windowPtr] : m_windows) {
            if (windowPtr && windowPtr->m_enabled) {
                windowPtr->render();
            }
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

} //OGL::E1::GUI