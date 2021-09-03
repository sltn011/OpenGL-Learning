#include "GUI/MainWindow.hpp"

namespace OGL::E1::GUI {

    MainWindow::MainWindow(
        bool isEnabled
    ) : BasicWindow{ isEnabled }, 
        m_hadUserActivity{ false },
        m_changedWindow{ WindowsTypes::NONE },
        m_windowsDropDownIndex{ 0 } {

    }

    void MainWindow::render(
    ) {
        m_hadUserActivity = false;

        ImGui::Begin("Engine1 Main Window");

        static constexpr char* windowsList[] = {
            "Object Transform",
            "Environment Control"
        };

        ImGui::Text("Window Selector");
        if (ImGui::BeginCombo("##ShowHideWindow", windowsList[m_windowsDropDownIndex])) {
            for (int i = 0; i < IM_ARRAYSIZE(windowsList); ++i) {
                bool isSelected = (windowsList[i] == windowsList[m_windowsDropDownIndex]);
                if (ImGui::Selectable(windowsList[i], isSelected)) {
                    m_windowsDropDownIndex = i;
                }
            }
            ImGui::EndCombo();
        }
        ImGui::SameLine();
        if (ImGui::Button("Show/Hide")) {
            switch (m_windowsDropDownIndex) {
            case 0:
                m_changedWindow = WindowsTypes::ObjectTransform;
                m_hadUserActivity = true;
                break;

            case 1:
                m_changedWindow = WindowsTypes::EnvironmentControl;
                m_hadUserActivity = true;
                break;

            default:
                break;
            }
        }
        
        ImGui::End();


        if (!m_hadUserActivity) {
            m_changedWindow = WindowsTypes::NONE;
        }
    }

    bool MainWindow::hadUserActivity(
    ) const {
        return m_hadUserActivity;
    }

    WindowsTypes MainWindow::getChangedWindow(
    ) const {
        return m_changedWindow;
    }

} // OGL::E1::GUI