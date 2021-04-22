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

    std::unordered_map<WindowsType, std::unique_ptr<BasicWindow>> &GUIRenderer::getWindows(
    ){
        return m_windows;
    }

    void GUIRenderer::addWindow(
        WindowsType windowType,
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
        WindowsType winType = m_mainWindow.getChangedWindow();
        if (winType != WindowsType::NONE) {
            auto winIt = m_windows.find(winType);
            if (winIt != m_windows.end()) {
                winIt->second->m_enabled ^= true;
            }
            else {
                auto newWindow = createWindow(winType, true);
                addWindow(winType, std::move(newWindow));
            }
        }
    }

    std::unique_ptr<BasicWindow> GUIRenderer::createWindow(
        WindowsType windowType,
        bool isEnabled
    ) const {
        switch (windowType) {
        case OGL::E1::GUI::WindowsType::ObjectTransform:
            return std::make_unique<ObjectTransformWindow>(isEnabled);

        default:
            return nullptr;
        }
    }

} //OGL::E1::GUI