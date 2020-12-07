#include "First OGL Engine/OGL_E1_v.0.1.hpp"

class Test : public OGL::E1::Engine1Base {
public:
    Test(int width, int height) : Engine1Base{ width, height } {

    }

    bool userCreate
    (
    ) override {
        OGL::E1::GameCamera::cam = OGL::E1::factory<OGL::CameraFirstPerson>(glm::vec3{}, glm::vec3{}, glm::vec3{}, 0.0f, 0.0f, 0.0f);
        return true;
    }

    bool userFrameUpdate
    ( float elapsedTime
    ) override {
        processInput();
        return true;
    }
};

int main() {
    Test t(800, 800);
    t.setClearColor(0.0f, 0.0f, 0.5f, 0.0f);
    t.start();
    return 0;
}