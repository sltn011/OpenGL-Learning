#include "OGL_E1_for_learning_examples/20-Cubemaps/OGL_E1.hpp"
#include <map>

int playgroundRenderMode = 0;
int crateRenderMode = 0;
int windowRenderMode = 0;

const int skyboxTextureID = 15;

unsigned int loadSkyboxTexture(std::vector<std::string> const &paths) {
    unsigned int textureId;
    glGenTextures(1, &textureId);
    glActiveTexture(GL_TEXTURE0 + skyboxTextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    stbi_set_flip_vertically_on_load(false);

    for (size_t i = 0; i < 6; ++i) {
        int width, height, nrChannels;
        unsigned char *data = stbi_load(paths[i].c_str(), &width, &height, &nrChannels, 0);
        if (!data) {
            stbi_set_flip_vertically_on_load(true);
            stbi_image_free(data);
            throw OGL::Exception("Error loading cubemap texture " + paths[i]);
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glActiveTexture(GL_TEXTURE0);

    stbi_set_flip_vertically_on_load(true);

    return textureId;
}

class Test : public OGL::E1::Engine1Base {
public:
    Test(int width, int height) : Engine1Base{ width, height } {}

    glm::mat4 m_projection;

    unsigned int m_skyboxVAO;
    unsigned int m_skyboxVBO;
    unsigned int m_skyboxTexture;

    bool userCreate
    (
    ) override {
        OGL::E1::GameCamera::inst = OGL::E1::factory<OGL::CameraFree>(
            glm::vec3{ 0.0f, 0.0f, 0.0f },
            glm::vec3{ 0.0f, 0.0f, -1.0f },
            glm::vec3{ 0.0f, 1.0f, 0.0f },
            1.0f,
            -90.0f,
            0.0f
        );

        m_shaders.emplace_back("shaders/16-cubemapsObjs.vert", "shaders/16-cubemapsObjs.frag");
        m_shaders.emplace_back("shaders/16-cubemapsSkybox.vert", "shaders/16-cubemapsSkybox.frag");

        int screenWidth, screenHeight;
        glfwGetFramebufferSize(m_window, &screenWidth, &screenHeight);

        m_projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.01f, 100.0f);

        m_shaders[0].use();
        m_shaders[0].setUniformMatrix4("projection", m_projection);

        m_shaders[1].use();
        m_shaders[1].setUniformMatrix4("projection", m_projection);

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
        glm::vec3 directionalLightDir{ 0.85f, -1.5f, -1.0f };
        glm::vec3 directionalLightColor{ 1.55f, 1.55f, 1.35f };

        addDirLight(directionalLightDir, directionalLightColor);
        //addSpotLight({}, {}, { 1.0f, 1.0f, 1.0f }, glm::radians(23.0f), glm::radians(25.0f), 1.0f, 0.22f, 0.20f);

        std::vector<std::string> cubemathPaths {
            "textures/Skybox1/right.jpg",
            "textures/Skybox1/left.jpg",
            "textures/Skybox1/top.jpg",
            "textures/Skybox1/bottom.jpg",
            "textures/Skybox1/front.jpg",
            "textures/Skybox1/back.jpg",
        };

        float skyboxVertices[] = {
            // positions          
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };

        glGenVertexArrays(1, &m_skyboxVAO);
        glBindVertexArray(m_skyboxVAO);
        glGenBuffers(1, &m_skyboxVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glBindVertexArray(0);

        m_skyboxTexture = loadSkyboxTexture(cubemathPaths);

        m_shaders[0].use();
        m_shaders[0].setUniformInt("skyboxSampler", skyboxTextureID);
        m_shaders[1].use();
        m_shaders[1].setUniformInt("skyboxSampler", skyboxTextureID);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);

        glEnable(GL_CULL_FACE);

        return true;
    }

    bool userFrameUpdate
    ( float elapsedTime
    ) override {
        processInput(0.5f);

        m_shaders[0].use();
        m_shaders[0].setUniformMatrix4("view", OGL::E1::GameCamera::inst->getViewMatrix());
        m_shaders[0].setUniformVec3("viewerPos", OGL::E1::GameCamera::inst->getPos());

        m_shaders[1].use();
        m_shaders[1].setUniformMatrix4("view", glm::mat4(glm::mat3(OGL::E1::GameCamera::inst->getViewMatrix()))); // Remove translation

        // Update flashlight
        if (m_spotLights.size() != 0) {
            m_spotLights[0]->m_position = OGL::E1::GameCamera::inst->getPos();
            m_spotLights[0]->m_direction = OGL::E1::GameCamera::inst->getForward();
        }

        m_shaders[0].use();

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
        m_shaders[0].use();
        m_shaders[0].setUniformInt("renderMode", playgroundRenderMode);
        for (auto &obj : m_objects[0]) { // Render playground
            obj->draw(m_shaders[0]);
        }

        m_shaders[0].setUniformInt("renderMode", crateRenderMode);
        for (auto &obj : m_objects[1]) { // Render crates
            obj->draw(m_shaders[0]);
        }

        // Draw skybox
        m_shaders[1].use();
        glBindVertexArray(m_skyboxVAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);
        glDepthFunc(GL_LEQUAL);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        glBindVertexArray(0);

        // Sort transparent objects by distance to player using map
        std::map<float, OGL::E1::smartObjPtr const&> map;
        for (auto const &obj : m_objects[2]) {
            float dist = glm::distance(OGL::E1::GameCamera::inst->getPos(), obj->m_postiton);
            map.emplace(dist, obj);
        }

        // Draw transparent windows starting from farthes to closest
        m_shaders[0].use();
        m_shaders[0].setUniformInt("renderMode", windowRenderMode);
        for (auto windowIt = map.rbegin(); windowIt != map.rend(); ++windowIt) {
            windowIt->second->draw(m_shaders[0]);
        }

        return true;
    }

    bool userDestroy
    (
    ) override {
        glDeleteTextures(1, &m_skyboxTexture);
        return true;
    }
};

// Keys^
// 1 - Switch playground render mode
// 2 - Switch crate render mode
// 3 - Switch window render mode

int main() {
    stbi_set_flip_vertically_on_load(true);

    Test t(1920, 1080);
    t.start();
    return 0;
}