#include "OGL_E1_for_learning_examples/16-StencilTesting/OGL_E1.hpp"
#include <iostream>
#include <fstream>

class Test : public OGL::E1::Engine1Base {
public:
    Test(int width, int height) : Engine1Base{ width, height } {}

    glm::mat4 m_projection;

    bool userCreate
    (
    ) override {
        OGL::E1::GameCamera::inst = OGL::E1::factory<OGL::CameraFirstPerson>(
            glm::vec3{ 0.0f, 0.0f, 0.0f },
            glm::vec3{ 0.0f, 0.0f, -1.0f },
            glm::vec3{ 0.0f, 1.0f, 0.0f },
            1.0f,
            -90.0f,
            0.0f
        );

        m_shaders.emplace_back("shaders/13-stencilBufferTest.vert", "shaders/13-stencilBufferTest.frag");
        m_shaders.emplace_back("shaders/13-stencilBufferTest.vert", "shaders/13-objectOutliner.frag");

        int screenWidth, screenHeight;
        glfwGetFramebufferSize(m_window, &screenWidth, &screenHeight);

        m_projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.01f, 100.0f);

        for (auto &shader : m_shaders) {
            shader.use();
            shader.setUniformMatrix4("projection", m_projection);
        }

        // Objects
        addModel("models/Playground/playground.obj", 0);
        addModel("models/Crate/crate.obj", 1);

        glm::vec3 playgroundPosition = { 0.0f, 0.0f, 0.0f };
        float     playgroundScale = 0.1f;
        float     playgroundRotationRadians = glm::radians(0.0f);
        glm::vec3 playgroundRotationAxes = { 0.0f, 1.0f, 0.0f };
        addObject(0, playgroundPosition, playgroundScale, playgroundRotationRadians, playgroundRotationAxes);

        glm::vec3 cratesPosition[] = { { -0.8f, -0.0955f, -0.75f }, { -0.5f, -0.0855f, -1.0f }, { -0.35f, -0.0955f, -0.65f }, { -0.87f, -0.0955f, -1.1f } };
        float     cratesScale[] = { { 0.055f }, { 0.065f }, { 0.055f }, { 0.055f } };
        float     cratesRotationRadians[] = { glm::radians(0.0f), glm::radians(45.0f), glm::radians(30.0f), glm::radians(60.0f) };
        for (size_t i = 0; i < sizeof(cratesScale) / sizeof(cratesScale[0]); ++i) {
            addObject(1, cratesPosition[i], cratesScale[i], cratesRotationRadians[i]);
        }

        // Lights
        glm::vec3 directionalLightDir{ -0.7f, -1.0f, -0.65f };
        glm::vec3 directionalLightColor{ 1.75f, 1.75f, 1.60f };

        addDirLight(directionalLightDir, directionalLightColor);

        return true;
    }

    bool userFrameUpdate
    (float elapsedTime
    ) override {
        processInput(0.5f);

        for (auto &shader : m_shaders) {
            shader.use();
            shader.setUniformMatrix4("view", OGL::E1::GameCamera::inst->getViewMatrix());
        }

        m_shaders[0].use();
        m_shaders[0].setUniformVec3("viewerPos", OGL::E1::GameCamera::inst->getPos());

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

        glEnable(GL_STENCIL_TEST); // Enable stencil test
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // What to do when tests fail/pass
        glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer

        glStencilMask(0x00); // don't update buffer before drawing floor

        // Draw objects
        m_shaders[0].use();
        m_objects[0]->draw(m_shaders[0]); // Draw playground
        
        glStencilFunc(GL_ALWAYS, 1, 0xFF); // All fragments should pass test
        glStencilMask(0xFF); // Write to buffer allowed
        
        for (size_t i = 1; i < m_objects.size(); ++i) {
            m_objects[i]->draw(m_shaders[0]); // Draw containers with normal shader with writing 1s to stencil buffer where containers are
        }

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // Test will pass when stencil value is not 1
        glStencilMask(0x00); // Set buffer to read-only
        glDisable(GL_DEPTH_TEST); // Or containers will be overwritten by floor

        m_shaders[1].use();
        for (size_t i = 1; i < m_objects.size(); ++i) {
            float oldSize = m_objects[i]->m_scale;
            float newSize = oldSize * 1.1f;

            m_objects[i]->m_scale = newSize; // Resize

            m_objects[i]->draw(m_shaders[1]); // Draw bigger container with only one color only where stencil value won't be 1

            m_objects[i]->m_scale = oldSize; // Resize back
        }

        glStencilMask(0xFF); // Write to buffer allowed
        glStencilFunc(GL_ALWAYS, 1, 0xFF); // All fragments should pass test
        glEnable(GL_DEPTH_TEST); // Enable depth test back

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