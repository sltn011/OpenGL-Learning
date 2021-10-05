/**
* @file PostprocessingWindow.hpp
*/

#ifndef OGL_E1_GUI_POSTPROCESSINGWINDOW
#define OGL_E1_GUI_POSTPROCESSINGWINDOW

#include "GUI/BasicWindow.hpp"
#include "Object.hpp"
#include "Utils/Delegate.hpp"
#include "Utils/UtilsLibrary.hpp"

namespace OGL::E1::GUI {

    /**
     * @brief GUI window for postprocessing control
    */
    class PostprocessingWindow : public BasicWindow {

     public:
        /**
         * @brief PostprocessingWindow constructor
         * @param isEnabled isEnabled Flag which indicates if window should be rendered or not
        */
        explicit PostprocessingWindow(
            bool isEnabled = false
        );

        /**
         * @brief Renders PostprocessingWindow
        */
        void render(
        ) override;

    protected:

        Delegate<bool> m_onTogglePostprocessing;
        Delegate<bool> m_onToggleHDR;
        Delegate<float> m_onSetHDRExposure;

    };

} // OGL::E1::GUI

#endif // OGL_E1_GUI_POSTPROCESSINGWINDOW
