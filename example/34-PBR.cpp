#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <cmath>

#include "GLFWInitRAII.hpp"
#include "Shader.hpp"
#include "CameraFree.hpp"
#include "Sphere.hpp"
#include "Cube.hpp"
#include "Object.hpp"
#include "PointLight.hpp"
#include "Texture.hpp"
#include "FrameBufferObject.hpp"
#include "Cubemap.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

bool bRenderGUI = false;
glm::vec3 AlbedoColor = glm::vec3{ 0.80f, 0.0, 0.0f };
float Roughness = 0.0f;
float Metallic = 0.0f;
bool bIsTextured = true;
bool bUseIBL = true;

constexpr int EnviroMapID = 10;
constexpr int IrradianceMapID = 11;
constexpr int CubemapSize = 1024;
constexpr int IrradianceCubemapSize = 32;

struct CursorPos {
    double x;
    double y;
} CursorPosition;

enum Screen {
    width = 1920,
    height = 1080
};

namespace System {
    float deltaTime = 0.0f;
    float lastFrameTime = 0.0f;

    float lastMouseXPos = Screen::width / 2;
    float lastMouseYPos = Screen::height / 2;

    float mouseSensitivity = 0.15f;
}

OGL::CameraFree freeCam{
    glm::vec3{1.0f, 0.0f, 3.0f},
    {0.0f, 0.0f, -1.0f},
    5.0f, 45.0f, static_cast<float>(Screen::width) / static_cast<float>(Screen::height), 0.01f, 100.0f
};

void framebufferSizeCallback(
    GLFWwindow *window,
    int width,
    int height
) {
    glViewport(0, 0, width, height);
}

void processKeyInput(
    GLFWwindow *window
) {
    if (bRenderGUI) {
        return;
    }

    // Camera Controls
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        freeCam.processMoveInput(OGL::CameraMovementDirection::Forward, System::deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        freeCam.processMoveInput(OGL::CameraMovementDirection::Backward, System::deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        freeCam.processMoveInput(OGL::CameraMovementDirection::Left, System::deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        freeCam.processMoveInput(OGL::CameraMovementDirection::Right, System::deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        freeCam.processMoveInput(OGL::CameraMovementDirection::Up, System::deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        freeCam.processMoveInput(OGL::CameraMovementDirection::Down, System::deltaTime);
    }
}

void mouseCallback(
    GLFWwindow *window,
    double xpos,
    double ypos
) {
    if (bRenderGUI) {
        return;
    }

    float xOffset = static_cast<float>(xpos) - System::lastMouseXPos;
    float yOffset = System::lastMouseYPos - static_cast<float>(ypos);
    System::lastMouseXPos = static_cast<float>(xpos);
    System::lastMouseYPos = static_cast<float>(ypos);

    freeCam.processRotateInput(xOffset, yOffset, System::mouseSensitivity, true);
}

void keyboardCallback(
    GLFWwindow *window,
    int key,
    int scancode,
    int action,
    int mods
) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        bRenderGUI ^= true;
        if (bRenderGUI) {
            glfwGetCursorPos(window, &CursorPosition.x, &CursorPosition.y);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPos(window, CursorPosition.x, CursorPosition.y);
        }
    }
}

void clearScreen(
) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Also clears depth buffer
}

void InitImGUI(
    GLFWwindow *window,
    std::string const &glslVersion
) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion.c_str());
    ImGui::StyleColorsDark();
}

void RenderGUI(
    bool bDoRender
) {
    if (!bDoRender) {
        return;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("PBR");

    ImGui::ColorPicker3("Sphere Albedo Color", &(AlbedoColor.x));
    ImGui::SliderFloat("Roughness", &Roughness, 0.0f, 1.0f);
    ImGui::SliderFloat("Metallic", &Metallic, 0.0f, 1.0f);
    ImGui::Checkbox("Is Texured", &bIsTextured);
    ImGui::Checkbox("Use IBL", &bUseIBL);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

OGL::Texture EnvironmentMapConvert(
    OGL::Cube &cube,
    OGL::Texture &enviroTex,
    GLenum enviroTexType,
    int SkyboxSize,
    OGL::Shader &shader
) {
    OGL::FrameBufferObject framebuffer;
    framebuffer.bind(GL_FRAMEBUFFER);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    OGL::RenderBufferObject renderbuffer;
    renderbuffer.allocateStorage(SkyboxSize, SkyboxSize, GL_DEPTH_COMPONENT24);
    framebuffer.attachRenderBuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, std::move(renderbuffer));

    OGL::Texture Cubemap;
    Cubemap.bind(GL_TEXTURE_CUBE_MAP);
    for (int i = 0; i < 6; ++i) {
        Cubemap.allocate(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            SkyboxSize,
            SkyboxSize,
            GL_RGB16F,
            GL_RGB,
            GL_FLOAT,
            nullptr
        );

        Cubemap.setParameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        Cubemap.setParameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        Cubemap.setParameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        Cubemap.setParameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        Cubemap.setParameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[6] = {
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };

    glActiveTexture(GL_TEXTURE0 + EnviroMapID);
    enviroTex.bind(enviroTexType);
    glActiveTexture(GL_TEXTURE0);

    glViewport(0, 0, SkyboxSize, SkyboxSize);
    glDisable(GL_CULL_FACE);

    shader.use();
    shader.setUniformMatrix4("projection", captureProjection);
    shader.setUniformInt("environmentMap", EnviroMapID);
    for (int i = 0; i < 6; ++i) {
        shader.setUniformMatrix4("view", captureViews[i]);

        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            Cubemap.value(),
            0
        );

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cube.draw(shader);
    }

    glEnable(GL_CULL_FACE);
    glViewport(0, 0, Screen::width, Screen::height);
    OGL::FrameBufferObject::unbind(GL_FRAMEBUFFER);

    return Cubemap;
}

void RenderCubemap(
    OGL::Cube &SkyboxCube,
    OGL::Texture &Cubemap,
    OGL::Shader &Shader
) {
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);

    Cubemap.bind(GL_TEXTURE_CUBE_MAP);
    Shader.use();
    Shader.setUniformInt("environmentMap", 0);
    Shader.setUniformMatrix4("projection", freeCam.getProjectionMatrix());
    Shader.setUniformMatrix4("view", freeCam.getViewMatrix());

    SkyboxCube.draw(Shader);

    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
}




struct SphereData {
    glm::vec3 Color;
    float Roughness;
    float Metallic;
};

void GenerateSphereData(
    SphereData Data[49]
) {
    Data[0].Color  = glm::vec3{ 0.0f, 0.0f, 0.0f };
    Data[6].Color  = glm::vec3{ 0.0f, 0.6f, 0.0f };
    Data[42].Color = glm::vec3{ 0.0f, 0.0f, 0.6f };
    Data[48].Color = glm::vec3{ 0.6f, 0.0f, 0.0f };
    for (int column = 1; column < 6; ++column) {
        Data[0 * 7 + column].Color =
            (1.0f - column / 6.0f) * Data[0 * 7 + 0].Color + (column / 6.0f) * Data[0 * 7 + 6].Color;

        Data[6 * 7 + column].Color =
            (1.0f - column / 6.0f) * Data[6 * 7 + 0].Color + (column / 6.0f) * Data[6 * 7 + 6].Color;
    }
    for (int column = 0; column < 7; ++column) {
        for (int row = 1; row < 6; ++row) {
            Data[row * 7 + column].Color =
                (1.0f - row / 6.0f) * Data[0 * 7 + column].Color + (row / 6.0f) * Data[6 * 7 + column].Color;

            Data[row * 7 + column].Color =
                (1.0f - row / 6.0f) * Data[0 * 7 + column].Color + (row / 6.0f) * Data[6 * 7 + column].Color;
        }
    }

    float RowRoughness[7] = { 0.01f, 0.05f, 0.1f, 0.15f, 0.25f, 0.5f, 1.0f };
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            Data[i * 7 + j].Roughness = RowRoughness[j];
        }
    }

    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            Data[i * 7 + j].Metallic = i > 3 ? 1.0f : 0.0f;
        }
    }
}



/*
 * PRESS 1 ON KEYBOARD TO TOGGLE GUI 
 */
int main(
) {
    srand(time(0));

    // Initialize GLFW
    OGL::GLFWInitRAII glfwInitializer;

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow *window = glfwCreateWindow(Screen::width, Screen::height, "Hello, PBR", nullptr, nullptr);
    if (!window) {
        std::cout << "Error creating window!" << std::endl;
        glfwTerminate();
        return 2;
    }

    // Set window as current context
    glfwMakeContextCurrent(window);

    // Set callback function for window resize
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // Set callback for mouse movement events
    glfwSetCursorPosCallback(window, mouseCallback);

    // Set keyboard callback
    glfwSetKeyCallback(window, keyboardCallback);

    // Tell GLFW to capture mouse input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Load OpenGL functions pointer given by GLFW
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 3;
    }

    InitImGUI(window, "#version 330");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Shader
    OGL::Shader ShaderProgramm("shaders/30-PBR.vert", "shaders/30-PBR.frag");
    OGL::Shader EquirectangularToSkybox("shaders/EquirectangularToSkybox.vert", "shaders/EquirectangularToSkybox.frag");
    OGL::Shader EnvironmentSkyboxShader("shaders/EnvironmentSkybox.vert", "shaders/EnvironmentSkybox.frag");
    OGL::Shader EnvironmentToIrradianceShader("shaders/EnvironmentToIrradiance.vert", "shaders/EnvironmentToIrradiance.frag");


    // Rendered object
    OGL::Sphere SphereModel(1.0f, 36, 36);
    OGL::Object Sphere(&SphereModel, glm::vec3{ 3.0f, 3.0f, 3.0f }, 0.5, glm::vec3{ 90.0f, 0.0f, 0.0f });



    // Different untextured spheres
    SphereData ColoredSpheresData[49];
    GenerateSphereData(ColoredSpheresData);

    std::vector<OGL::Object> ColoredSpheres;
    ColoredSpheres.reserve(49);
    for (size_t i = 0; i < 7; ++i) {
        for (size_t j = 0; j < 7; ++j) {
            ColoredSpheres.push_back(OGL::Object{ &SphereModel, glm::vec3{1.15f * j, 1.15f * i, 0.0f}, 0.5f });
        }
    }


    // Environment map skybox
    OGL::Cube CubeModel(1.0f);

    // Textures
    OGL::Texture AlbedoTexture("materials/OldCopper", "Albedo.png", GL_TEXTURE_2D);
    OGL::Texture NormalTexture("materials/OldCopper", "Normal.png", GL_TEXTURE_2D);
    OGL::Texture RoughnessTexture("materials/OldCopper", "Roughness.png", GL_TEXTURE_2D);
    OGL::Texture MetallicTexture("materials/OldCopper", "Metallic.png", GL_TEXTURE_2D);
    OGL::Texture AOTexture("materials/OldCopper", "AO.png", GL_TEXTURE_2D);
    OGL::Texture *Textures[5] = { &AlbedoTexture, &NormalTexture, &RoughnessTexture, &MetallicTexture, &AOTexture };
    std::string TextureNames[5] = { "AlbedoTexture", "NormalTexture", "RoughnessTexture", "MetallicTexture", "AOTexture" };

    OGL::Texture HDREnvironmentMap;
    HDREnvironmentMap.loadHDR("textures/HDR/Topanga_Forest_B", "Topanga_Forest_B_3k.hdr", GL_TEXTURE_2D);
    //HDREnvironmentMap.loadHDR("textures/HDR/Mono_Lake_C", "Mono_Lake_C_Ref.hdr", GL_TEXTURE_2D);

    // Light Source
    OGL::PointLight Lights[4] = {
        {glm::vec3{ 8.0f, 8.0f, 8.0f }, glm::vec3{ 100.0f, 99.0f, 98.5f }},
        {glm::vec3{ 2.0f, 0.0f, 10.0f }, glm::vec3{ 100.0f, 99.0f, 98.5f }},
        {glm::vec3{ -4.0f, -2.0f, 5.0f }, glm::vec3{ 100.0f, 99.0f, 98.5f }},
        {glm::vec3{ -1.0f, 4.0f, 5.0f }, glm::vec3{ 100.0f, 99.0f, 98.5f }},
    };

    ShaderProgramm.use();
    for (int i = 0; i < 4; ++i) {
        Lights[i].loadInShader(ShaderProgramm, i);
    }
    ShaderProgramm.setUniformInt("numPointLights", 4);
    ShaderProgramm.setUniformMatrix4("projection", freeCam.getProjectionMatrix());

    for (int i = 0; i < 5; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        Textures[i]->bind(GL_TEXTURE_2D);
        ShaderProgramm.setUniformInt(TextureNames[i], i);
    }
    glActiveTexture(GL_TEXTURE0);

    OGL::Texture EnvironmentCubemap = EnvironmentMapConvert(
        CubeModel,
        HDREnvironmentMap,
        GL_TEXTURE_2D,
        CubemapSize,
        EquirectangularToSkybox
    );

    OGL::Texture IrradianceCubemap = EnvironmentMapConvert(
        CubeModel,
        EnvironmentCubemap,
        GL_TEXTURE_CUBE_MAP,
        IrradianceCubemapSize,
        EnvironmentToIrradianceShader
    );

    // 6. Render loop
    while (!glfwWindowShouldClose(window)) {

        float currentFrameTime = (float)glfwGetTime();
        System::deltaTime = currentFrameTime - System::lastFrameTime;
        System::lastFrameTime = currentFrameTime;

        processKeyInput(window);

        clearScreen();

        ShaderProgramm.use();

        glActiveTexture(GL_TEXTURE0 + IrradianceMapID);
        IrradianceCubemap.bind(GL_TEXTURE_CUBE_MAP);
        glActiveTexture(GL_TEXTURE0);
        ShaderProgramm.setUniformBool("bUseIBL", bUseIBL);
        ShaderProgramm.setUniformInt("IrradianceCubemap", IrradianceMapID);

        // Draw Textured
        ShaderProgramm.setUniformVec3("viewerPos", freeCam.getPos());
        ShaderProgramm.setUniformMatrix4("view", freeCam.getViewMatrix());
        ShaderProgramm.setUniformVec3("AlbedoColor", AlbedoColor);
        ShaderProgramm.setUniformFloat("RoughnessValue", Roughness);
        ShaderProgramm.setUniformFloat("MetallicValue", Metallic);
        ShaderProgramm.setUniformBool("bIsTextured", bIsTextured);
        Sphere.draw(ShaderProgramm);

        for (size_t i = 0; i < 49; ++i) {
            ShaderProgramm.setUniformVec3("AlbedoColor", ColoredSpheresData[i].Color);
            ShaderProgramm.setUniformFloat("RoughnessValue", ColoredSpheresData[i].Roughness);
            ShaderProgramm.setUniformFloat("MetallicValue", ColoredSpheresData[i].Metallic);
            ShaderProgramm.setUniformBool("bIsTextured", false);
            ColoredSpheres[i].draw(ShaderProgramm);
        }

        RenderCubemap(CubeModel, EnvironmentCubemap, EnvironmentSkyboxShader);

        RenderGUI(bRenderGUI);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Process pending events
        glfwPollEvents();

    }

    return 0;
}