#include "OGL_E1.hpp"

const int shadowMapDirLightFirstTextureID = 10;
const int shadowCubemapFirstTextureID = shadowMapDirLightFirstTextureID + 4;
const int shadowMapSpotLightFirstTextureID = shadowCubemapFirstTextureID + 4;
const int skyboxTextureID = 22;
const int cubemapTextureID = 23;


const int cubemapSize = 512;
const int shadowMapSize = 1024 * 2;
const int shadowCubemapSize = 1024;

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
            glm::vec3{ 0.0f, 0.15f, 0.0f },
            glm::vec3{ 0.0f, 0.0f, -1.0f },
            glm::vec3{ 0.0f, 1.0f, 0.0f },
            1.0f,
            45.0f,
            static_cast<float>(screenWidth) / static_cast<float>(screenHeight),
            0.01f,
            100.0f
        );

        m_scene = OGL::E1::factory<OGL::E1::Scene>(std::move(gameCamera));

        OGL::Shader normalShader(
            "shaders/01-playgroundObj.vert",
            "shaders/01-playgroundObj.frag"
        );

        OGL::Shader transpShader(
            "shaders/01-playgroundObj.vert",
            "shaders/01-playgroundObj.frag"
        );

        OGL::Shader skyboxShader(
            "shaders/01-playgroundSkybox.vert",
            "shaders/01-playgroundSkybox.frag"
        );

        OGL::Shader mirrorShader(
            "shaders/01-playgroundMirror.vert",
            "shaders/01-playgroundMirror.frag"
        );

        OGL::Shader instancesShader(
            "shaders/01-playgroundInstances.vert",
            "shaders/01-playgroundInstances.frag"
        );

        OGL::Shader shadowMapRender(
            "shaders/01-playgroundShadowMapRenderer.vert",
            "shaders/01-playgroundShadowMapRenderer.frag"
        );

        OGL::Shader shadowCubemapRenderer(
            "shaders/01-playgroundShadowCubemapRenderer.vert",
            "shaders/01-playgroundShadowCubemapRenderer.geom",
            "shaders/01-playgroundShadowCubemapRenderer.frag"
        );

        OGL::Shader lightSourcesRenderer(
            "shaders/01-playgroundLightSourcesRenderer.vert",
            "shaders/01-playgroundLightSourcesRenderer.frag"
        );

        m_normalRenderer            = OGL::E1::factory<OGL::E1::NormalRenderer>(std::move(normalShader));
        m_transpRenderer            = OGL::E1::factory<OGL::E1::TransparentRenderer>(std::move(transpShader));
        m_skyboxRenderer            = OGL::E1::factory<OGL::E1::SkyboxRenderer>(std::move(skyboxShader));
        m_mirrorRenderer            = OGL::E1::factory<OGL::E1::MirrorRenderer>(std::move(mirrorShader));
        m_cubemapRenderer           = OGL::E1::factory<OGL::E1::CubemapRenderer>();
        m_instancesRenderer         = OGL::E1::factory<OGL::E1::InstancesRenderer>(std::move(instancesShader));
        m_shadowMapRenderer         = OGL::E1::factory<OGL::E1::ShadowMapRenderer>(std::move(shadowMapRender));
        m_shadowCubemapRenderer     = OGL::E1::factory<OGL::E1::ShadowCubemapRenderer>(std::move(shadowCubemapRenderer));
        m_lightSourcesDebugRenderer = OGL::E1::factory<OGL::E1::LightSourcesDebugRenderer>(std::move(lightSourcesRenderer), 0.005f);

        // Objects
        addModel("models/Playground/playground.obj", 0);
        addModel("models/Crate/crate.obj", 1);
        addModel("models/Window/window.obj", 2);
        addModel("models/Sphere/sphere.obj", 3);
        addModel("models/Planet/Planet/planet.obj", 4);
        addModel("models/Planet/Asteroid/asteroid.obj", 5);
        addModel("models/Gun/gun.obj", 6);

        glm::vec3 playgroundPosition = { 0.0f, 0.0f, 0.0f };
        float     playgroundScale = 0.05f;
        float     playgroundRotation = 0.0f;
        glm::vec3 playgroundRotationAxes = { 0.0f, 1.0f, 0.0f };
        addNormalObject(0, playgroundPosition, playgroundScale, playgroundRotation, playgroundRotationAxes);

        glm::vec3 cratesPosition[] = { { -0.3f, -0.02f, -0.55f }, { 0.0f, -0.0135f, -0.7f }, { -0.15f, -0.02f, -0.35f }, { -0.37f, -0.02f, -0.9f } };
        float     cratesScale[] = { 0.055f, 0.065f, 0.055f, 0.055f };
        float     cratesRotation[] = { 30.0f, 45.0f, 0.0f, 60.0f };
        for (size_t i = 0; i < sizeof(cratesScale) / sizeof(cratesScale[0]); ++i) {
            addNormalObject(1, cratesPosition[i], cratesScale[i], cratesRotation[i]);
        }

        glm::vec3 windowsPosition[] = { { 0.8f, 0.024f, 0.45f }, { 0.6f, 0.024f, 0.1f }, { 0.35f, 0.024f, 0.1f }, { 0.0f, 0.024f, 0.2f } };
        float     windowsScale[] = { 0.1f, 0.1f, 0.1f, 0.1f };
        float     windowsRotation[] = { 100.0f, -25.0f, -65.0f, 80.0f };
        for (size_t i = 0; i < sizeof(windowsPosition) / sizeof(windowsPosition[0]); ++i) {
            addTransparentObject(2, windowsPosition[i], windowsScale[i], windowsRotation[i]);
        }

        glm::vec3 spherePosition{ 0.3f, 0.025f, -0.3f };
        float     sphereScale = 0.1f;
        addMirrorObject(3, spherePosition, sphereScale);

        glm::vec3 planetPosition{ 8.5f, 2.5f, -7.0f };
        float     planetScale = 0.30f;
        float     planetRotation = 90.0f;
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

        addNormalObject(6, { -0.4f, 0.05f, 0.4f }, 0.01f, 60.0f);

        // Lights
        glm::vec3 directionalLightDir = glm::normalize(glm::vec3{ 1.5f, -2.0f, -1.25f });
        directionalLightDir *= 3;
        glm::vec3 directionalLightColor{ 1.0f, 1.0f, 0.75f };
        addDirLight(directionalLightDir, directionalLightColor);

        m_scene->replaceSkybox(OGL::E1::factory<OGL::Skybox>("textures/Skybox1", GL_TEXTURE0 + skyboxTextureID));

        glViewport(0, 0, shadowMapSize, shadowMapSize);
        for (size_t i = 0; i < m_scene->getDirLights().size(); ++i) {
            auto &[dirLight, shadowMap] = m_scene->getDirLights()[i];
            OGL::CameraShadowMap cam{ dirLight, playgroundPosition, 2.5f, 0.1f, 6.0f };
            shadowMap = OGL::E1::factory<OGL::ShadowMap>(
                m_shadowMapRenderer->render(*m_scene, cam, GL_TEXTURE0 + shadowMapDirLightFirstTextureID + i, shadowMapSize)
            );
        }

        glViewport(0, 0, shadowCubemapSize, shadowCubemapSize);
        for (size_t i = 0; i < m_scene->getPointLights().size(); ++i) {
            auto &[pointLight, shadowCubemap] = m_scene->getPointLights()[i];
            OGL::CameraShadowCubemap cam(pointLight, 0.01f, 3.5f);
            shadowCubemap = std::make_unique<OGL::ShadowCubemap>(
                m_shadowCubemapRenderer->render(*m_scene, cam, GL_TEXTURE0 + shadowCubemapFirstTextureID + i, shadowCubemapSize)
            );
        }

        glViewport(0, 0, shadowMapSize, shadowMapSize);
        for (size_t i = 0; i < m_scene->getSpotLights().size(); ++i) {
            auto &[spotLight, shadowMap] = m_scene->getSpotLights()[i];
            OGL::CameraShadowMap cam{ spotLight, 0.1f, 6.0f };
            shadowMap = OGL::E1::factory<OGL::ShadowMap>(
                m_shadowMapRenderer->render(*m_scene, cam, GL_TEXTURE0 + shadowMapSpotLightFirstTextureID + i, shadowMapSize)
            );
        }

        glViewport(0, 0, cubemapSize, cubemapSize);
        for (auto &p : m_scene->getMirrorObjs()) {
            p.second = OGL::E1::factory<OGL::Cubemap>(
                m_cubemapRenderer->render(
                    *m_scene,
                    cubemapSize,
                    GL_TEXTURE0 + cubemapTextureID,
                    p.first.getPosition(),
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

        m_lightSourcesDebugRenderer->render(*m_scene, m_scene->getCamera().get());

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