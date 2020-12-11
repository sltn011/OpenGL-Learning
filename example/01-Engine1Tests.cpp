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

        m_shaders.emplace_back("shaders/01-playgroundObj.vert", "shaders/01-playgroundObj.frag");

        int screenWidth, screenHeight;
        glfwGetFramebufferSize(m_window, &screenWidth, &screenHeight);

        m_projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.01f, 100.0f);

        m_shaders[0].use();
        m_shaders[0].setUniformMatrix4("projection", m_projection);
        
        // Objects
        addModel("models/Playground/playground.obj", 0);

        glm::vec3 positions[] = { { 0.0f, 0.0f, 0.0f } };
        float     scales[] = { 0.1f };
        float     rotationRadians[] = { glm::radians(0.0f) };
        glm::vec3 rotationAxes[] = { { 0.0f, 1.0f, 0.0f } };

        for (int i = 0; i < 1; ++i) {
            addObject(0, positions[i], scales[i], rotationRadians[i], rotationAxes[i]);
        }

        // Lights
        glm::vec3 directionalLightDir{ 0.1f, -1.0f, 0.2f };
        glm::vec3 directionalLightColor{ 0.15f, 0.15f, 0.15f };
        //glm::vec3 pointLightPos[2] = { {-1.0f, 2.0f, 1.6f}, {-3.0f, 1.5f, -2.0f} };
        //glm::vec3 pointLightColor[2] = { {0.0f, 0.0f, 0.2f}, {0.2f, 0.0f, 0.0f} };
        
        addDirLight(directionalLightDir, directionalLightColor);
        //for (int i = 0; i < 2; ++i) {
        //    addPointLight(pointLightPos[i], pointLightColor[i]);
        //}
        addSpotLight({}, {}, {0.75f, 0.75f, 0.75f}, glm::radians(22.0f), glm::radians(25.0f), 1.0f, 0.22f, 0.20f);

        return true;
    }

    bool userFrameUpdate
    ( float elapsedTime
    ) override {
        processInput();

        m_shaders[0].use();
        m_shaders[0].setUniformMatrix4("view", OGL::E1::GameCamera::inst->getViewMatrix());
        m_shaders[0].setUniformVec3("viewerPos", OGL::E1::GameCamera::inst->getPos());

        // Update flashlight
        m_spotLights[0]->m_position = OGL::E1::GameCamera::inst->getPos();
        m_spotLights[0]->m_direction = OGL::E1::GameCamera::inst->getForward();

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
        for (auto &obj : m_objects) {
            obj->draw(m_shaders[0]);
        }

        return true;
    }
};

int main() {
    stbi_set_flip_vertically_on_load(true);

    Test t(1920, 1080);
    t.setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    t.start();
    return 0;
}