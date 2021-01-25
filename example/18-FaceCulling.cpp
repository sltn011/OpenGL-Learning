#include "OGL_E1_for_learning_examples/18-FaceCulling/OGL_E1.hpp"
#include <map>

class Test : public OGL::E1::Engine1Base {
public:
    Test(
        int width,
        int height
    ) : Engine1Base{
        width, 
        height 
    } {
    }

    bool userCreate(
    ) override {
        m_shaders.emplace_back("shaders/14-blending.vert", "shaders/14-blending.frag");

        int screenWidth, screenHeight;
        glfwGetFramebufferSize(m_window, &screenWidth, &screenHeight);

        OGL::E1::GameCamera::inst = OGL::E1::factory<OGL::CameraFirstPerson>(
            glm::vec3{ 0.0f, 0.0f, 0.0f },
            glm::vec3{ 0.0f, 0.0f, -1.0f },
            glm::vec3{ 0.0f, 1.0f, 0.0f },
            1.0f, -90.0f, 0.0f,
            45.0f, static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.01f, 100.0f
            );

        m_shaders[0].use();
        m_shaders[0].setUniformMatrix4("projection", OGL::E1::GameCamera::inst->getProjectionMatrix());

        // Objects
        addModel("models/Playground/playground.obj", 0);
        addModel("models/Crate/crate.obj", 1);
        addModel("models/Window/window.obj", 2);

        glm::vec3 playgroundPosition = { 0.0f, 0.0f, 0.0f };
        float     playgroundScale = 0.1f;
        float     playgroundRotationRadians = glm::radians(0.0f);
        glm::vec3 playgroundRotationAxes = { 0.0f, 1.0f, 0.0f };
        addObject(0, 0, playgroundPosition, playgroundScale, playgroundRotationRadians, playgroundRotationAxes);

        glm::vec3 cratesPosition[] = { { -0.8f, -0.0955f, -0.75f }, { -0.5f, -0.0855f, -1.0f }, { -0.35f, -0.0955f, -0.65f }, { -0.87f, -0.0955f, -1.1f } };
        float     cratesScale[] = { 0.055f, 0.065f, 0.055f, 0.055f };
        float     cratesRotationRadians[] = { glm::radians(0.0f), glm::radians(45.0f), glm::radians(30.0f), glm::radians(60.0f) };
        for (size_t i = 0; i < sizeof(cratesScale) / sizeof(cratesScale[0]); ++i) {
            addObject(1, 1, cratesPosition[i], cratesScale[i], cratesRotationRadians[i]);
        }

        glm::vec3 windowsPosition[] = { { 0.8f, -0.05f, 0.75f }, { 0.5f, -0.05f, 1.0f }, { 0.35f, -0.05f, 0.65f }, { 0.87f, -0.05f, 1.1f } };
        float     windowsScale[] = { 0.1f, 0.1f, 0.1f, 0.1f };
        float     windowsRotation[] = { glm::radians(0.0f), glm::radians(45.0f), glm::radians(30.0f), glm::radians(60.0f) };
        for (size_t i = 0; i < sizeof(windowsPosition) / sizeof(windowsPosition[0]); ++i) {
            addObject(2, 2, windowsPosition[i], windowsScale[i], windowsRotation[i]);
        }

        // Lights
        glm::vec3 directionalLightDir{ -0.7f, -1.0f, -0.65f };
        glm::vec3 directionalLightColor{ 1.75f, 1.75f, 1.60f };

        addDirLight(directionalLightDir, directionalLightColor);
        //addSpotLight({}, {}, { 1.0f, 1.0f, 1.0f }, glm::radians(23.0f), glm::radians(25.0f), 1.0f, 0.22f, 0.20f);

        glEnable(GL_CULL_FACE);

        return true;
    }

    bool userFrameUpdate(
        float elapsedTime
    ) override {
        processInput(0.5f);

        m_shaders[0].use();
        m_shaders[0].setUniformMatrix4("view", OGL::E1::GameCamera::inst->getViewMatrix());
        m_shaders[0].setUniformVec3("viewerPos", OGL::E1::GameCamera::inst->getPos());

        // Update flashlight
        if (m_spotLights.size() != 0) {
            m_spotLights[0]->m_position = OGL::E1::GameCamera::inst->getPos();
            m_spotLights[0]->m_direction = OGL::E1::GameCamera::inst->getForward();
        }

        // Load lights
        m_shaders[0].setUniformInt("numDirLights", (int)m_dirLights.size());
        for (size_t i = 0; i < m_dirLights.size(); ++i) {
            m_dirLights[i]->loadInShader(m_shaders[0], i);
        }
        m_shaders[0].setUniformInt("numPointLights", (int)m_pointLights.size());
        for (size_t i = 0; i < m_pointLights.size(); ++i) {
            m_pointLights[i]->loadInShader(m_shaders[0], i);
        }
        m_shaders[0].setUniformInt("numSpotLights", (int)m_spotLights.size());
        for (size_t i = 0; i < m_spotLights.size(); ++i) {
            m_spotLights[i]->loadInShader(m_shaders[0], i);
        }

        // Draw objects
        for (size_t group = 0; group < 2; ++group) { // Draw playground and boxes
            for (auto &obj : m_objects[group]) {
                obj->draw(m_shaders[0]);
            }
        }

        // Sort transparent objects by distance to player using map
        std::map<float, OGL::E1::smartObjPtr const&> map;
        for (auto const &obj : m_objects[2]) {
            float dist = glm::distance(OGL::E1::GameCamera::inst->getPos(), obj->m_postiton);
            map.emplace(dist, obj);
        }

        // Draw transparent windows starting from farthes to closest
        for (auto windowIt = map.rbegin(); windowIt != map.rend(); ++windowIt) {
            windowIt->second->draw(m_shaders[0]);
        }

        return true;
    }
};

// Key 1 - switch glCullFace GL_BACK/GL_FRONT/GL_FRONTAND_BACK
// Key 2 - switch glFrontFace GL_CCW/GL_CW

int main(
) {
    stbi_set_flip_vertically_on_load(true);

    Test t(1920, 1080);
    t.setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    t.start();
    return 0;
}