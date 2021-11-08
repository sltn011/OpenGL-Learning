#include "GUI/PostprocessingWindow.hpp"

#include "OGL_E1.hpp"

namespace OGL::E1::GUI {

    PostprocessingWindow::PostprocessingWindow(
        bool isEnabled
    ) : BasicWindow{ isEnabled } {
        Engine1Base *engineInstance = UtilsLibrary::getEngineInstance();
        m_onTogglePostprocessing.bind(engineInstance, &Engine1Base::togglePostprocessing);
        m_onToggleHDR.bind(engineInstance, &Engine1Base::toggleHDR);
        m_onSetHDRExposure.bind(engineInstance, &Engine1Base::setHDRExposure);
        m_onToggleBloom.bind(engineInstance, &Engine1Base::toggleBloom);
    }

    void PostprocessingWindow::render(
    ) {
        Engine1Base *engineInstance = UtilsLibrary::getEngineInstance();
        if (!engineInstance) {
            return;
        }

        PostprocessingData data = engineInstance->getPostprocessingData();

        ImGui::Begin("Postprocessing");
        
        if (ImGui::Checkbox("Toggle Postprocessing", &data.bEnablePostprocessing)) {
            m_onTogglePostprocessing(data.bEnablePostprocessing);
        }
        if (ImGui::Checkbox("Toggle HDR", &data.bEnableHDR)) {
            m_onToggleHDR(data.bEnableHDR);
        }
        if (ImGui::InputFloat("HDR Exposure", &data.HDRExposure, 0.000005f, 0.000005f, "%.6f")) {
            m_onSetHDRExposure(data.HDRExposure);
        }
        if (ImGui::Checkbox("Toggle Bloom", &data.bEnableBloom)) {
            m_onToggleBloom(data.bEnableBloom);
        }

        ImGui::End();
    }

} // OGL::E1::GUI