#include "Cubemap.hpp"
#include <map>
#include "Cubemap.hpp"
#include "Skybox.hpp"
#include "CameraCubemap.hpp"
#include "FrameBufferObject.hpp"
#include "First OGL Engine/OGL_E1.hpp"

const int skyboxTextureID = 15;
const int depthMapTextureID = 20;
size_t depthMapSize = 4096;

glm::vec3 lightDir = glm::vec3{ 1.0f, -0.8f, -0.5f };

class Test : public OGL::E1::Engine1Base {
 public:

    int m_screenWidth;
    int m_screenHeight;

    OGL::E1::smartNormalRendererPtr m_depthMapRenderer;
    OGL::E1::smartNormalRendererPtr m_depthMapVisualizer;

    OGL::E1::smartCamPtr m_cameraPlaceholder;

    OGL::FrameBufferObject m_depthMapFBO;

    glm::mat4 m_lightProjView;

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
            glm::vec3{ 0.0f, 0.15f, 0.0f },
            glm::vec3{ 0.0f, 0.0f, -1.0f },
            glm::vec3{ 0.0f, 1.0f, 0.0f },
            1.0f,
            -90.0f,
            0.0f,
            45.0f,
            static_cast<float>(m_screenWidth) / static_cast<float>(m_screenHeight),
            0.01f,
            100.0f
        );

        m_scene = OGL::E1::factory<OGL::E1::Scene>(std::move(gameCamera));

        OGL::Shader normalShader("shaders/22-normalObjWithShadows.vert", "shaders/22-normalObjWithShadows.frag");
        OGL::Shader skyboxShader("shaders/01-playgroundSkybox.vert", "shaders/01-playgroundSkybox.frag");

        OGL::Shader depthMapRender("shaders/22-normalObjDepthMap.vert", "shaders/22-normalObjDepthMap.frag");
        OGL::Shader depthMapVizualizer("shaders/22-depthMapVisualizer.vert", "shaders/22-depthMapVisualizer.frag");

        m_normalRenderer = OGL::E1::factory<OGL::E1::NormalRenderer>(std::move(normalShader));
        m_skyboxRenderer = OGL::E1::factory<OGL::E1::SkyboxRenderer>(std::move(skyboxShader));

        m_depthMapRenderer = OGL::E1::factory<OGL::E1::NormalRenderer>(std::move(depthMapRender));
        m_depthMapVisualizer = OGL::E1::factory<OGL::E1::NormalRenderer>(std::move(depthMapVizualizer));

        // Objects
        addModel("models/WoodPlanksPlane/woodPlanksPlane.obj", 0);
        addModel("models/Crate/crate.obj", 1);

        glm::vec3 playgroundPosition = { -0.0f, 0.0f, -0.5f };
        float     playgroundScale = 1.0f;
        float     playgroundRotationRadians = glm::radians(0.0f);
        glm::vec3 playgroundRotationAxes = { 0.0f, 1.0f, 0.0f };
        addNormalObject(0, playgroundPosition, playgroundScale, playgroundRotationRadians, playgroundRotationAxes);

        glm::vec3 cratesPosition[] = { { -0.5f, 0.2551f, -0.45f }, { -0.2f, 0.0651f, -0.7f }, { -0.05f, 0.1851f, -0.35f }, { -0.57f, 0.0551f, -0.8f } };
        float     cratesScale[] = { 0.055f, 0.065f, 0.055f, 0.055f };
        float     cratesRotationRadians[] = { glm::radians(30.0f), glm::radians(45.0f), glm::radians(135.0f), glm::radians(60.0f) };
        glm::vec3 cratesRotationAxis[] = { {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f} };
        for (size_t i = 0; i < sizeof(cratesScale) / sizeof(cratesScale[0]); ++i) {
            addNormalObject(1, cratesPosition[i], cratesScale[i], cratesRotationRadians[i], cratesRotationAxis[i]);
        }

        // Lights
        glm::vec3 directionalLightDir = lightDir;
        glm::vec3 directionalLightColor{ 1.0f, 1.0f, 0.75f };
        addDirLight(directionalLightDir, directionalLightColor);

        stbi_set_flip_vertically_on_load(false);
        m_scene->replaceSkybox(OGL::E1::factory<OGL::Skybox>("textures/Skybox1", GL_TEXTURE0 + skyboxTextureID));
        stbi_set_flip_vertically_on_load(true);

        // Placeholder
        m_cameraPlaceholder = OGL::E1::factory<OGL::CameraFree>(
            glm::vec3{ 0.0f, 0.15f, 0.0f },
            glm::vec3{ 0.0f, 0.0f, -1.0f },
            glm::vec3{ 0.0f, 1.0f, 0.0f },
            1.0f,
            -90.0f,
            0.0f,
            45.0f,
            static_cast<float>(m_screenWidth) / static_cast<float>(m_screenHeight),
            0.01f,
            1000.0f
        );

        // Prepare framebuffer
        OGL::ColorBufferObject depthMap;
        depthMap.allocateStorage(depthMapSize, depthMapSize, GL_TEXTURE_2D, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
        m_depthMapFBO.bind(GL_FRAMEBUFFER);
        m_depthMapFBO.attachColorBuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, std::move(depthMap));
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        // Render depth map to fbo
        glViewport(0, 0, depthMapSize, depthMapSize);
        glClear(GL_DEPTH_BUFFER_BIT);
        glm::mat4 lightProjMatrix = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.1f, 10.0f);
        glm::mat4 lightViewMatrix = glm::lookAt(-lightDir, glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f });
        m_lightProjView = lightProjMatrix * lightViewMatrix;
        m_depthMapRenderer->getShader().use();
        m_depthMapRenderer->getShader().setUniformMatrix4("lightProjView", m_lightProjView);
        m_depthMapRenderer->render(*m_scene, m_cameraPlaceholder.get());
        OGL::FrameBufferObject::unbind(GL_FRAMEBUFFER);

        glBindTexture(GL_TEXTURE_2D, m_depthMapFBO.getColorBuffers()[GL_DEPTH_ATTACHMENT].value());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        glBindTexture(GL_TEXTURE_2D, 0);

        return true;
    }

    bool userFrameUpdate(
        float elapsedTime
    ) override {
        processInputPerFrame();

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glViewport(0, 0, m_screenWidth, m_screenHeight);
        OGL::Texture::setActive(GL_TEXTURE0 + depthMapTextureID);
        glBindTexture(GL_TEXTURE_2D, m_depthMapFBO.getColorBuffers()[GL_DEPTH_ATTACHMENT].value());
        OGL::Texture::setActive(GL_TEXTURE0);
        m_normalRenderer->getShader().use();
        m_normalRenderer->getShader().setUniformInt("depthMap", depthMapTextureID);
        m_normalRenderer->getShader().setUniformMatrix4("lightProjView", m_lightProjView);

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
    }
};

int main(
) {
    stbi_set_flip_vertically_on_load(true);

    Test t(1920, 1080);
    t.start();
    return 0;
}