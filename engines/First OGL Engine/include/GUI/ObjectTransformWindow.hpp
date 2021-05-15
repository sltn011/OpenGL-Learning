/**
* @file ObjectTransformWindow.hpp
*/

#ifndef OGL_E1_GUI_OBJECTTRANSFORMWINDOW
#define OGL_E1_GUI_OBJECTTRANSFORMWINDOW

#include "GUI/BasicWindow.hpp"
#include "Object.hpp"

namespace OGL::E1::GUI {

    /**
     * @brief GUI window for changing Object's position, scale and rotation
    */
    class ObjectTransformWindow : public BasicWindow {
     private:
        /// Pointer to selected Object that can be modified, or nullptr if none selected
        Object *m_obj;

     public:
        /**
         * @brief ObjectTransformWindow constructor
         * @param isEnabled isEnabled Flag which indicates if window should be rendered or not
        */
        explicit ObjectTransformWindow(
            bool isEnabled = false
        );

        /**
         * @brief Renders ObjectTransformWindow
        */
        void render(
        ) override;

        /**
         * @brief Sets Object that can be modified using ObjectTransformWindow
         * @param obj Pointer to selected Object or nullptr if none is selected
        */
        void setObject(
            Object *obj
        );

     private:

        /**
         * @brief Renders ObjectTransformWindow empty when no Object selected 
        */
        void renderEmpty(
        ) const;
    };

} // OGL::E1::GUI

#endif // OGL_E1_GUI_OBJECTTRANSFORMWINDOW
