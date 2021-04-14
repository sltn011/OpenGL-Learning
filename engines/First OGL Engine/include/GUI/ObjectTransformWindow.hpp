#ifndef OGL_E1_GUI_OBJECTTRANSFORMWINDOW
#define OGL_E1_GUI_OBJECTTRANSFORMWINDOW

#include "GUI/BasicWindow.hpp"
#include "Object.hpp"

namespace OGL::E1::GUI {

    class ObjectTransformWindow : public BasicWindow {
     public:
         ObjectTransformWindow(
         );

         void render(
         ) override;

         void setObject(
             Object *obj
         );

     private:
         Object *m_obj;
    };

} // OGL::E1::GUI

#endif // OGL_E1_GUI_OBJECTTRANSFORMWINDOW
