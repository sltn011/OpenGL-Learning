#include "Cubemap.hpp"
#include <map>
#include "Cubemap.hpp"
#include "Skybox.hpp"
#include "CameraCubemap.hpp"
#include "FrameBufferObject.hpp"
#include "First OGL Engine/OGL_E1.hpp"

const int skyboxTextureID = 15;
const int cubemapTextureID = 16;
const int shadowMapFirstTextureID = 17;

const int cubemapSize = 512;
const int shadowMapSize = 1024 * 5;

std::unique_ptr<glm::mat4[]> generateTranslationMatrices(
    size_t numMatrices,
    glm::vec3 center,
    float baseSize
);

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

    OGL::VertexBufferObject m_asteroidsVBO;

    bool userCreate(
    ) override {

        int screenWidth, screenHeight;
        glfwGetFramebufferSize(m_window, &screenWidth, &screenHeight);

        OGL::E1::smartCamPtr gameCamera = OGL::E1::factory<OGL::CameraFree>(
            glm::vec3{ 0.0f, 0.0f, 0.0f },
            glm::vec3{ 0.0f, 0.0f, -1.0f },
            glm::vec3{ 0.0f, 1.0f, 0.0f },
            1.0f,
            -90.0f,
            0.0f,
            45.0f,
            static_cast<float>(screenWidth) / static_cast<float>(screenHeight),
            0.01f,
            1000.0f
        );

        m_scene = OGL::E1::factory<OGL::E1::Scene>(std::move(gameCamera));

        OGL::Shader normalShader("shaders/01-playgroundObj.vert", "shaders/01-playgroundObj.frag");
        OGL::Shader transpShader("shaders/01-playgroundObj.vert", "shaders/01-playgroundObj.frag");
        OGL::Shader skyboxShader("shaders/01-playgroundSkybox.vert", "shaders/01-playgroundSkybox.frag");
        OGL::Shader mirrorShader("shaders/01-playgroundMirror.vert", "shaders/01-playgroundMirror.frag");
        OGL::Shader instancesShader("shaders/01-playgroundInstances.vert", "shaders/01-playgroundInstances.frag");
        OGL::Shader shadowMapRender("shaders/01-playgroundShadowMapRenderer.vert", "shaders/01-playgroundShadowMapRenderer.frag");

        m_normalRenderer = OGL::E1::factory<OGL::E1::NormalRenderer>(std::move(normalShader));
        m_transpRenderer = OGL::E1::factory<OGL::E1::TransparentRenderer>(std::move(transpShader));
        m_skyboxRenderer = OGL::E1::factory<OGL::E1::SkyboxRenderer>(std::move(skyboxShader));
        m_mirrorRenderer = OGL::E1::factory<OGL::E1::MirrorRenderer>(std::move(mirrorShader));
        m_cubemapRenderer = OGL::E1::factory<OGL::E1::CubemapRenderer>();
        m_instancesRenderer = OGL::E1::factory<OGL::E1::InstancesRenderer>(std::move(instancesShader));
        m_shadowMapRenderer = OGL::E1::factory<OGL::E1::ShadowMapRenderer>(std::move(shadowMapRender));

        // Objects
        addModel("models/Playground/playground.obj", 0);
        addModel("models/Crate/crate.obj", 1);
        addModel("models/Window/window.obj", 2);
        addModel("models/Sphere/sphere.obj", 3);
        addModel("models/Planet/Planet/planet.obj", 4);
        addModel("models/Planet/Asteroid/asteroid.obj", 5);

        glm::vec3 playgroundPosition = { 0.0f, 0.0f, 0.0f };
        float     playgroundScale = 0.05f;
        float     playgroundRotationRadians = glm::radians(0.0f);
        glm::vec3 playgroundRotationAxes = { 0.0f, 1.0f, 0.0f };
        addNormalObject(0, playgroundPosition, playgroundScale, playgroundRotationRadians, playgroundRotationAxes);

        glm::vec3 cratesPosition[] = { { -0.3f, -0.02f, -0.55f }, { 0.0f, -0.0135f, -0.7f }, { -0.15f, -0.02f, -0.35f }, { -0.37f, -0.02f, -0.9f } };
        float     cratesScale[] = { 0.055f, 0.065f, 0.055f, 0.055f };
        float     cratesRotationRadians[] = { glm::radians(30.0f), glm::radians(45.0f), glm::radians(0.0f), glm::radians(60.0f) };
        for (size_t i = 0; i < sizeof(cratesScale) / sizeof(cratesScale[0]); ++i) {
            addNormalObject(1, cratesPosition[i], cratesScale[i], cratesRotationRadians[i]);
        }

        glm::vec3 windowsPosition[] = { { 0.8f, 0.024f, 0.45f }, { 0.6f, 0.024f, 0.1f }, { 0.35f, 0.024f, 0.1f }, { 0.0f, 0.024f, 0.2f } };
        float     windowsScale[] = { 0.1f, 0.1f, 0.1f, 0.1f };
        float     windowsRotation[] = { glm::radians(100.0f), glm::radians(-25.0f), glm::radians(-65.0f), glm::radians(80.0f) };
        for (size_t i = 0; i < sizeof(windowsPosition) / sizeof(windowsPosition[0]); ++i) {
            addTransparentObject(2, windowsPosition[i], windowsScale[i], windowsRotation[i]);
        }

        glm::vec3 spherePosition{ 0.3f, 0.025f, -0.3f };
        float     sphereScale = 0.1f;
        addMirrorObject(3, spherePosition, sphereScale);

        glm::vec3 planetPosition{ 8.5f, 2.5f, -7.0f };
        float     planetScale = 0.30f;
        float     planetRotation = glm::radians(90.0f);
        glm::vec3 planetRotationVec{ 1.0f, 0.0f, 0.0f };
        addNormalObject(4, planetPosition, planetScale, planetRotation, planetRotationVec);

        size_t    asteroidInstances = 1000;
        glm::vec3 asteroidCircleCenter = planetPosition;
        float     asteroidScale = 0.1f;
        auto      asteroidInstanceMatrices = generateTranslationMatrices(asteroidInstances, asteroidCircleCenter, asteroidScale);
        addInstancedObject(5, asteroidInstances, asteroidCircleCenter, asteroidScale);
        m_asteroidsVBO.bind();
        glBufferData(GL_ARRAY_BUFFER, asteroidInstances * sizeof(glm::mat4), asteroidInstanceMatrices.get(), GL_STATIC_DRAW);
        m_scene->getInstancedObjs()[0].first.setVertexAttribInstancedModelMat4(3);
        m_asteroidsVBO.unbind();

        // Lights
        glm::vec3 directionalLightDir = glm::vec3{ -1.5f, -2.0f, -1.25f };
        glm::vec3 directionalLightColor{ 1.0f, 1.0f, 0.75f };
        addDirLight(directionalLightDir, directionalLightColor);

        stbi_set_flip_vertically_on_load(false);
        m_scene->replaceSkybox(OGL::E1::factory<OGL::Skybox>("textures/Skybox1", GL_TEXTURE0 + skyboxTextureID));
        stbi_set_flip_vertically_on_load(true);

        glViewport(0, 0, shadowMapSize, shadowMapSize);
        int cnt = 0;
        for (auto &p : m_scene->getDirLights()) {
            OGL::CameraShadowMap cam{ p.first, playgroundPosition, 4.0f, 0.1f, 4.0f };
            p.second = OGL::E1::factory<OGL::ShadowMap>(
                m_shadowMapRenderer->render(*m_scene, cam, GL_TEXTURE0 + shadowMapFirstTextureID + cnt, shadowMapSize)
                );
            ++cnt;
        }

        glViewport(0, 0, cubemapSize, cubemapSize);
        for (auto &p : m_scene->getMirrorObjs()) {
            p.second = OGL::E1::factory<OGL::Cubemap>(
                m_cubemapRenderer->render(
                    *m_scene,
                    cubemapSize,
                    GL_TEXTURE0 + cubemapTextureID,
                    p.first.m_postiton,
                    m_normalRenderer.get(),
                    m_skyboxRenderer.get(),
                    m_transpRenderer.get(),
                    m_instancesRenderer.get()
                )
            );
        }
        glViewport(0, 0, screenWidth, screenHeight);

        return true;
    }

    bool userFrameUpdate( 
        float elapsedTime
    ) override {
        processInputPerFrame();

        m_normalRenderer->render(*m_scene, m_scene->getCamera().get());

        m_mirrorRenderer->render(*m_scene, m_scene->getCamera().get());

        m_instancesRenderer->render(*m_scene, m_scene->getCamera().get());

        m_skyboxRenderer->render(*m_scene, m_scene->getCamera().get());

        m_transpRenderer->render(*m_scene, m_scene->getCamera().get());

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
    t.start();
    return 0;
}

std::unique_ptr<glm::mat4[]> generateTranslationMatrices(
    size_t numMatrices,
    glm::vec3 center,
    float baseSize
) {
    std::unique_ptr<glm::mat4[]> matrices = std::make_unique<glm::mat4[]>(numMatrices);
    float radius = 3.5f;
    float offset = 0.25f;
    for (size_t i = 0; i < numMatrices; ++i) {
        // translate
        glm::mat4 matr(1.0);
        matr = glm::translate(matr, center);
        float angle = static_cast<float>(i) / numMatrices * 360.0f;
        float displacement = (std::rand() % static_cast<int>(2 * 100 * offset)) / 100.0f - offset;
        float x = std::sin(angle) * radius + displacement;
        displacement = (std::rand() % static_cast<int>(2 * 100 * offset)) / 100.0f - offset;
        float y = displacement * 0.25f;
        displacement = (std::rand() % static_cast<int>(2 * 100 * offset)) / 100.0f - offset;
        float z = std::cos(angle) * radius + displacement;
        matr = glm::translate(matr, glm::vec3{ x, y, z });

        //scale
        float scale = (std::rand() % 20) / 100.0f + 0.05f;
        matr = glm::scale(matr, baseSize * glm::vec3{ scale, scale, scale });

        //rotate
        float rotation = static_cast<float>(std::rand() % 360);
        matr = glm::rotate(matr, rotation, glm::vec3{ std::rand(), std::rand(), std::rand() });

        matrices[i] = matr;
    }
    return matrices;
}