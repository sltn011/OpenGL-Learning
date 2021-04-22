#include "GUI/BasicWindow.hpp"

namespace OGL::E1::GUI {

    BasicWindow::BasicWindow(
    ) : m_enabled{false} {

    }

    BasicWindow::BasicWindow(
        bool isEnabled
    ) : m_enabled{ isEnabled } {

    }

    BasicWindow::~BasicWindow(
    ) {

    }

} //OGL::E1::GUI