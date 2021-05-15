#include "GUI/GUIRenderer.hpp"

namespace OGL::E1::GUI {

    GUIRenderer::GUIRenderer(
        GLFWwindow *window, 
        std::string glslVersion
    ) : m_mainWindow{ true } {
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

    std::unordered_map<WindowsTypes, std::unique_ptr<BasicWindow>> &GUIRenderer::getWindows(
    ){
        return m_windows;
    }

    void GUIRenderer::addWindow(
        WindowsTypes windowType,
        std::unique_ptr<BasicWindow> window
    ) {
        m_windows.emplace(windowType, std::move(window));
    }

    void GUIRenderer::render(
    ) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        renderMainWindow();

        if (m_mainWindow.hadUserActivity()) {
            handleUserActivity();
        }

        renderSubWindows();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void GUIRenderer::renderMainWindow(
    ) {
        m_mainWindow.render();
    }

    void GUIRenderer::renderSubWindows(
    ) {
        for (auto &[winType, winPtr] : m_windows) {
            if (winPtr && winPtr->m_enabled) {
                winPtr->render();
            }
        }
    }

    void GUIRenderer::handleUserActivity(
    ) {
        WindowsTypes winType = m_mainWindow.getChangedWindow();
        if (winType != WindowsTypes::NONE) {
            auto winIt = m_windows.find(winType);
            if (winIt != m_windows.end()) {
                winIt->second->m_enabled ^= true;
            }
            else {
                addWindow(winType, createWindow(winType, true));
            }
        }
    }

    std::unique_ptr<BasicWindow> GUIRenderer::createWindow(
        WindowsTypes windowType,
        bool isEnabled
    ) const {
        switch (windowType) {
        case OGL::E1::GUI::WindowsTypes::ObjectTransform:
            return std::make_unique<ObjectTransformWindow>(isEnabled);

        default:
            return nullptr;
        }
    }

} //OGL::E1::GUI