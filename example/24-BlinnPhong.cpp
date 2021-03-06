#include "Cubemap.hpp"
#include <map>
#include "Cubemap.hpp"
#include "Skybox.hpp"
#include "CameraCubemap.hpp"
#include "FrameBufferObject.hpp"
#include "OGL_E1.hpp"

const int skyboxTextureID = 15;

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
    
    bool m_isBlinnPhong = false;

    bool userCreate(
    ) override {

        int screenWidth, screenHeight;
        glfwGetFramebufferSize(m_window, &screenWidth, &screenHeight);

        OGL::E1::smartCamPtr gameCamera = OGL::E1::factory<OGL::CameraFree>(
            glm::vec3{ 0.0f, 0.15f, 0.0f },
            glm::vec3{ 0.0f, 0.0f, -1.0f },
            1.0f,
            45.0f,
            static_cast<float>(screenWidth) / static_cast<float>(screenHeight),
            0.01f,
            1000.0f
        );

        m_scene = std::make_unique<OGL::E1::Scene>(std::move(gameCamera));

        OGL::Shader normalShader("shaders/20-BlinnPhong.vert", "shaders/20-BlinnPhong.frag");
        OGL::Shader skyboxShader("shaders/01-playgroundSkybox.vert", "shaders/01-playgroundSkybox.frag");

        m_normalRenderer.emplace(std::move(normalShader));
        m_skyboxRenderer.emplace(std::move(skyboxShader));

        // Objects
        addModel("models/WoodPlanksPlane/woodPlanksPlane.obj", 0);
        addModel("models/Crate/crate.obj", 1);

        glm::vec3 playgroundPosition = { -0.25f, 0.0f, -0.5f };
        float     playgroundScale = 1.0f;
        float     playgroundRotationRadians = glm::radians(0.0f);
        glm::vec3 playgroundRotationAxes = { 0.0f, 1.0f, 0.0f };
        addNormalObject(0, playgroundPosition, playgroundScale, playgroundRotationRadians, playgroundRotationAxes);

        glm::vec3 cratesPosition[] = { { -0.5f, 0.0551f, -0.45f }, { -0.2f, 0.0651f, -0.7f }, { -0.05f, 0.0551f, -0.35f }, { -0.57f, 0.0551f, -0.8f } };
        float     cratesScale[] = { 0.055f, 0.065f, 0.055f, 0.055f };
        float     cratesRotationRadians[] = { glm::radians(30.0f), glm::radians(45.0f), glm::radians(0.0f), glm::radians(60.0f) };
        for (size_t i = 0; i < sizeof(cratesScale) / sizeof(cratesScale[0]); ++i) {
            addNormalObject(1, cratesPosition[i], cratesScale[i], cratesRotationRadians[i]);
        }

        // Lights
        addPointLight({ 0.0f, 0.15f, 0.0f }, {1.5f, 1.5f, 1.5f });

        stbi_set_flip_vertically_on_load(false);
        m_scene->replaceSkybox(OGL::Skybox("textures/Skybox1", GL_TEXTURE0 + skyboxTextureID));
        stbi_set_flip_vertically_on_load(true);

        glDisable(GL_CULL_FACE);

        return true;
    }

    bool userFrameUpdate(
        float elapsedTime
    ) override {
        processInputPerFrame();

        m_normalRenderer->getShader().use();
        m_normalRenderer->getShader().setUniformBool("isBlinnPhong", m_isBlinnPhong);

        m_normalRenderer->render(*m_scene, m_scene->getCamera().get());
        m_skyboxRenderer->render(*m_scene, m_scene->getCamera().get());

        return true;
    }

    bool userDestroy(
    ) override {
        return true;
    }

    void keyCallback(
        int key,
        int scancode,
        int action,
        int mods
    ) override {
        if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
            m_isBlinnPhong ^= true;
        }
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(m_window, true);
        }
    }
};

int main(
) {
    stbi_set_flip_vertically_on_load(true);

    Test t(1920, 1080);
    t.start();
    return 0;
}