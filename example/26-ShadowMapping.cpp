#include "Cubemap.hpp"
#include <map>
#include "Cubemap.hpp"
#include "Skybox.hpp"
#include "ShadowMap.hpp"
#include "CameraCubemap.hpp"
#include "FrameBufferObject.hpp"
#include "OGL_E1.hpp"

const int skyboxTextureID = 15;
const int shadowMapTextureID = 20;
size_t shadowMapSize = 4096;

class Test : public OGL::E1::Engine1Base {
 public:

    int m_screenWidth;
    int m_screenHeight;

    Test(
        int width,
        int height
    ) : Engine1Base{
        width,
        height
    } {
        m_screenWidth = width;
        m_screenHeight = height;
    }

    bool userCreate(
    ) override {

        OGL::E1::smartCamPtr gameCamera = OGL::E1::factory<OGL::CameraFree>(
            glm::vec3{ 10.0f, 0.2f, 0.5f },
            glm::vec3{ 0.0f, 0.0f, -1.0f },
            1.0f,
            45.0f,
            static_cast<float>(m_screenWidth) / static_cast<float>(m_screenHeight),
            0.01f,
            1000.0f
        );

        m_scene = std::make_unique<OGL::E1::Scene>(std::move(gameCamera));

        OGL::Shader normalShader("shaders/22-normalObjWithShadows.vert", "shaders/22-normalObjWithShadows.frag");
        OGL::Shader skyboxShader("shaders/01-playgroundSkybox.vert", "shaders/01-playgroundSkybox.frag");
        OGL::Shader shadowMapRender("shaders/22-normalObjDepthMap.vert", "shaders/22-normalObjDepthMap.frag");

        m_normalRenderer.emplace(std::move(normalShader));
        m_skyboxRenderer.emplace(std::move(skyboxShader));
        m_shadowMapRenderer.emplace(std::move(shadowMapRender));

        // Objects
        addModel("models/WoodPlanksPlane/woodPlanksPlane.obj", 0);
        addModel("models/Crate/crate.obj", 1);

        glm::vec3 playgroundPosition = { 10.0f, 0.0f, 0.0f };
        float     playgroundScale = 1.0f;
        float     playgroundRotationRadians = glm::radians(0.0f);
        glm::vec3 playgroundRotationAxes = { 0.0f, 1.0f, 0.0f };
        addNormalObject(0, playgroundPosition, playgroundScale, playgroundRotationRadians, playgroundRotationAxes);

        glm::vec3 cratesPositionOffset[] = { { -0.5f, 0.2551f, 0.5f }, { -0.2f, 0.0651f, 0.2f }, { -0.05f, 0.1851f, -0.25f }, { -0.57f, 0.0551f, 0.4f } };
        float     cratesScale[] = { 0.055f, 0.065f, 0.055f, 0.055f };
        float     cratesRotationRadians[] = { glm::radians(30.0f), glm::radians(45.0f), glm::radians(135.0f), glm::radians(60.0f) };
        glm::vec3 cratesRotationAxis[] = { {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f} };
        for (size_t i = 0; i < sizeof(cratesScale) / sizeof(cratesScale[0]); ++i) {
            addNormalObject(1, cratesPositionOffset[i] + playgroundPosition, cratesScale[i], cratesRotationRadians[i], cratesRotationAxis[i]);
        }

        // Lights
        glm::vec3 lightDir1 = glm::normalize(glm::vec3{ 1.0f, -0.8f, -0.5f });
        glm::vec3 lightDir2 = glm::normalize(glm::vec3{ -0.5f, -1.7f, -0.3f });
        glm::vec3 lightDir3 = glm::normalize(glm::vec3{ 0.5f, -1.0f, 0.5f });
        glm::vec3 lightCol1{ 1.0f, 1.0f, 0.75f };
        glm::vec3 lightCol2{ 0.0f, 0.0f, 0.75f };
        glm::vec3 lightCol3{ 0.0f, 0.75f, 0.0f };
        addDirLight(lightDir1, lightCol1);
        addDirLight(lightDir2, lightCol2);
        addDirLight(lightDir3, lightCol3);

        stbi_set_flip_vertically_on_load(false);
        m_scene->replaceSkybox(OGL::Skybox("textures/Skybox1", GL_TEXTURE0 + skyboxTextureID));
        stbi_set_flip_vertically_on_load(true);

        glViewport(0, 0, shadowMapSize, shadowMapSize);
        int cnt = 0;
        for (auto &p : m_scene->getDirLights()) {
            OGL::CameraShadowMap cam{ p.first, playgroundPosition, 2.0f, 0.1f, 10.0f };
            p.second = m_shadowMapRenderer->render(*m_scene, cam, GL_TEXTURE0 + shadowMapTextureID + cnt, shadowMapSize);
            ++cnt;
        }
        glViewport(0, 0, m_screenWidth, m_screenHeight);

        return true;
    }

    bool userFrameUpdate(
        float elapsedTime
    ) override {
        processInputPerFrame();

        m_normalRenderer->render(*m_scene, m_scene->getCamera().get());
        m_skyboxRenderer->render(*m_scene, m_scene->getCamera().get());

        return true;
    }

    bool userDestroy(
    ) override {
        return true;
    }
};

int main(
) {
    stbi_set_flip_vertically_on_load(true);

    Test t(1920, 1080);
    t.setClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    t.start();
    return 0;
}