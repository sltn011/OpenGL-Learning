#include "GUI/EnvironmentControlWindow.hpp"

#include "OGL_E1.hpp"

namespace OGL::E1::GUI {

    EnvironmentControlWindow::EnvironmentControlWindow(
        bool isEnabled
    ) : BasicWindow{ isEnabled } {
        Engine1Base *Engine = UtilsLibrary::getEngineInstance();
        m_OnRebuildShadows.bind(Engine, &Engine1Base::rebuildShadows);
        m_OnRebuildReflections.bind(Engine, &Engine1Base::rebuildReflections);
    }

    void EnvironmentControlWindow::render(
    ) {

        ImGui::Begin("Environment Control");
        
        if (ImGui::Button("Rebuild Shadows")) {
            m_OnRebuildShadows();
        }

        ImGui::SameLine();

        if (ImGui::Button("Rebuild Reflections")) {
            m_OnRebuildReflections();
        }

        ImGui::End();
    }

} // OGL::E1::GUI