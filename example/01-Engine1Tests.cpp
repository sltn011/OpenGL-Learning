#include "First OGL Engine/OGL_E1.hpp"


class Test : public OGL::E1::Engine1Base {
public:
    Test(int width, int height) : Engine1Base{ width, height } {}

    glm::mat4 m_projection;

    bool userCreate
    (
    ) override {
        OGL::E1::GameCamera::inst = OGL::E1::factory<OGL::CameraFree>(
            glm::vec3{0.0f, 0.0f, 0.0f}, 
            glm::vec3{ 0.0f, 0.0f, -1.0f },
            glm::vec3{ 0.0f, 1.0f, 0.0f },
            1.0f, 
            -90.0f, 
            0.0f
        );

        m_shaders.emplace_back("shaders/11-multLightsObj.vert", "shaders/11-multLightsObj.frag");

        int screenWidth, screenHeight;
        glfwGetFramebufferSize(m_window, &screenWidth, &screenHeight);

        m_projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.01f, 100.0f);

        m_shaders[0].use();
        m_shaders[0].setUniformMatrix4("projection", m_projection);

        // Objects
        m_modelsTable[0] = OGL::E1::factory<OGL::Model>("models/Backpack/backpack.obj");

        m_objects.emplace_back(OGL::E1::factory<OGL::Object>(*m_modelsTable[0].get()));
        m_objects[0]->m_postiton = glm::vec3{ 0.0f, 0.0f, -2.0f };
        m_objects[0]->m_scale = 0.25f;
        m_objects[0]->m_rotationAngleRadians = glm::radians(-45.0f);
        m_objects[0]->m_rotationAxis = glm::vec3(1.0f, 1.0f, 1.0f);

        m_objects.emplace_back(OGL::E1::factory<OGL::Object>(*m_modelsTable[0].get()));
        m_objects[1]->m_postiton = glm::vec3{ 1.0f, -2.0f, -3.0f };
        m_objects[1]->m_scale = 0.25f;
        m_objects[1]->m_rotationAngleRadians = glm::radians(125.0f);
        m_objects[1]->m_rotationAxis = glm::vec3(1.0f, 1.0f, 0.0f);

        m_objects.emplace_back(OGL::E1::factory<OGL::Object>(*m_modelsTable[0].get()));
        m_objects[2]->m_postiton = glm::vec3{ -1.0f, -0.5f, -1.0f };
        m_objects[2]->m_scale = 0.35f;
        m_objects[2]->m_rotationAngleRadians = glm::radians(0.0f);
        m_objects[2]->m_rotationAxis = glm::vec3(1.0f, 1.0f, 1.0f);

        m_objects.emplace_back(OGL::E1::factory<OGL::Object>(*m_modelsTable[0].get()));
        m_objects[3]->m_postiton = glm::vec3{ 0.0f, 1.5f, -0.75f };
        m_objects[3]->m_scale = 0.25f;
        m_objects[3]->m_rotationAngleRadians = glm::radians(30.0f);
        m_objects[3]->m_rotationAxis = glm::vec3(1.0f, 0.35f, 0.0f);

        m_objects.emplace_back(OGL::E1::factory<OGL::Object>(*m_modelsTable[0].get()));
        m_objects[4]->m_postiton = glm::vec3{ 1.0f, 0.0f, -4.0f };
        m_objects[4]->m_scale = 0.65f;
        m_objects[4]->m_rotationAngleRadians = glm::radians(9.0f);
        m_objects[4]->m_rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);

        // Lights
        glm::vec3 directionalLightDir{ -2.0f, -1.5f, 2.0f };
        glm::vec3 directionalLightColor{ 0.0f, 0.5f, 0.0f };
        glm::vec3 pointLightPos[2] = { {2.0f, 1.75f, 1.75f}, {-3.0f, -1.5f, -2.0f} };
        glm::vec3 pointLightColor[2] = { {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f} };
        m_dirLights.emplace_back(OGL::E1::factory<OGL::DirectionalLight>(directionalLightDir, directionalLightColor));
        for (int i = 0; i < 2; ++i) {
            m_pointLights.emplace_back(OGL::E1::factory<OGL::PointLight>(pointLightPos[i], pointLightColor[i]));
        }
        m_spotLights.emplace_back(OGL::E1::factory<OGL::SpotLight>());
        m_spotLights[0]->m_cutOffAngle = glm::radians(15.0f);
        m_spotLights[0]->m_cutOffOuterAngle = glm::radians(25.0f);

        return true;
    }

    bool userFrameUpdate
    ( float elapsedTime
    ) override {
        processInput();

        m_shaders[0].use();
        m_shaders[0].setUniformMatrix4("view", OGL::E1::GameCamera::inst->getViewMatrix());

        // Update flashlight
        m_spotLights[0]->m_position = OGL::E1::GameCamera::inst->getPos();
        m_spotLights[0]->m_direction = OGL::E1::GameCamera::inst->getForward();

        // Load lights
        for (auto &l : m_dirLights) {
            l->loadInShader(m_shaders[0]);
        }
        for (size_t i = 0; i < m_pointLights.size(); ++i) {
            m_pointLights[i]->loadInShader(m_shaders[0], i);
        }
        for (auto &l : m_spotLights) {
            l->loadInShader(m_shaders[0]);
        }

        // Draw objects
        for (auto &obj : m_objects) {
            obj->draw(m_shaders[0]);
        }

        return true;
    }
};

int main() {
    stbi_set_flip_vertically_on_load(true);

    Test t(1400, 1000);
    t.setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    t.start();
    return 0;
}