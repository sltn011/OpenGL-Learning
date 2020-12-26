#include "OGL_E1_for_learning_examples/19-Framebuffers/OGL_E1.hpp"
#include <map>

int postprocessMode = 0;

class Test : public OGL::E1::Engine1Base {
public:
    Test(int width, int height) : Engine1Base{ width, height } {}

    glm::mat4 m_projection;

    unsigned int m_fbo; // Frame buffer object
    unsigned int m_texture; // Texture for color buffer
    unsigned int m_rbo; // Renderbuffer object for depth and stencil buffers

    unsigned int m_quadVAO;
    unsigned int m_quadVBO;

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

        m_shaders.emplace_back("shaders/15-Framebuffer_ToFBO.vert", "shaders/15-Framebuffer_ToFBO.frag");
        m_shaders.emplace_back("shaders/15-Framebuffer_ToScreen.vert", "shaders/15-Framebuffer_ToScreen.frag");

        int screenWidth, screenHeight;
        glfwGetFramebufferSize(m_window, &screenWidth, &screenHeight);

        m_projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.01f, 100.0f);

        m_shaders[0].use();
        m_shaders[0].setUniformMatrix4("projection", m_projection);

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

        glGenFramebuffers(1, &m_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo); // Create and bind framebuffer object

        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture); // Create and bind texture

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr); // allocate memory for texture but not fill
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0); // bind texture to framebuffer as color buffer

        glGenRenderbuffers(1, &m_rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, m_rbo); // Create and bind renderbuffer object

        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight); // Allocate memory for renderbuffer object
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo); // bind renderbuffer to framebuffer
        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            fprintf(stderr, "Error creating framebuffer object");
            return false;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        float quadVertices[] {
            -1.0f, +1.0f,    0.0f, 1.0f,
            -1.0f, -1.0f,    0.0f, 0.0f,
            +1.0f, -1.0f,    1.0f, 0.0f,

            -1.0f, +1.0f,    0.0f, 1.0f,
            +1.0f, -1.0f,    1.0f, 0.0f,
            +1.0f, +1.0f,    1.0f, 1.0f,
        };

        glGenVertexArrays(1, &m_quadVAO); // VAO to draw quad on screen
        glBindVertexArray(m_quadVAO);
        glGenBuffers(1, &m_quadVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glBindVertexArray(0);

        m_shaders[1].use(); // Set quad texture to quad shader
        m_shaders[1].setUniformInt("fboTexture", 0);

        return true;
    }

    bool userFrameUpdate
    (float elapsedTime
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

        m_shaders[0].use();

        // Bind Framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // not using stencil
        glEnable(GL_DEPTH_TEST);

        // All draws go to framebuffer

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

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // Set default framebuffer
        setClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_shaders[1].use();
        m_shaders[1].setUniformInt("postprocessMode", postprocessMode);

        glBindVertexArray(m_quadVAO);
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);

        return true;
    }

    bool userDestroy
    (
    ) override {
        glDeleteVertexArrays(1, &m_quadVAO);
        glDeleteBuffers(1, &m_quadVBO);
        glDeleteTextures(1, &m_texture);
        glDeleteRenderbuffers(1, &m_rbo);
        glDeleteFramebuffers(1, &m_fbo);
        return true;
    }
};

// Press 1 to switch postprocessing effect

int main() {
    stbi_set_flip_vertically_on_load(true);

    Test t(1920, 1080);
    t.start();
    return 0;
}