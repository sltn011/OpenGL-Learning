#ifndef OGL_E1_GUI_OBJECTTRANSFORMWINDOW
#define OGL_E1_GUI_OBJECTTRANSFORMWINDOW

#include "GUI/BasicWindow.hpp"
#include "Object.hpp"

namespace OGL::E1::GUI {

    class ObjectTransformWindow : public BasicWindow {
     public:
         ObjectTransformWindow(
             bool isEnabled = false
         );

         void render(
         ) override;

         void setObject(
             Object *obj
         );

     private:
         Object *m_obj;

         void renderEmpty(
         ) const;
    };

} // OGL::E1::GUI

#endif // OGL_E1_GUI_OBJECTTRANSFORMWINDOW
