#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <cmath>

#include "GLFWInitRAII.hpp"
#include "Shader.hpp"
#include "CameraFree.hpp"
#include "Sphere.hpp"
#include "Object.hpp"
#include "PointLight.hpp"
#include "Texture.hpp"

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

void InitImGUI(GLFWwindow *window, std::string const &glslVersion) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion.c_str());
    ImGui::StyleColorsDark();
}

void RenderGUI(bool bDoRender) {
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

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/*
 * PRESS 1 ON KEYBOARD TO TOGGLE GUI 
 */
int main(
) {
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
    OGL::Shader shaderProgramm("shaders/30-PBR.vert", "shaders/30-PBR.frag");


    // Rendered object
    OGL::Sphere SphereModel(1.0f, 36, 36);
    OGL::Object Sphere(&SphereModel, glm::vec3{ 1.0f, 0.0f, 1.0f }, 0.5, glm::vec3{ 90.0f, 0.0f, 0.0f });

    // Textures
    OGL::Texture AlbedoTexture("materials/OldCopper", "Albedo.png", GL_TEXTURE_2D);
    OGL::Texture NormalTexture("materials/OldCopper", "Normal.png", GL_TEXTURE_2D);
    OGL::Texture RoughnessTexture("materials/OldCopper", "Roughness.png", GL_TEXTURE_2D);
    OGL::Texture MetallicTexture("materials/OldCopper", "Metallic.png", GL_TEXTURE_2D);
    OGL::Texture AOTexture("materials/OldCopper", "AO.png", GL_TEXTURE_2D);
    OGL::Texture *Textures[5] = { &AlbedoTexture, &NormalTexture, &RoughnessTexture, &MetallicTexture, &AOTexture };
    std::string TextureNames[5] = { "AlbedoTexture", "NormalTexture", "RoughnessTexture", "MetallicTexture", "AOTexture" };

    // Light Source
    OGL::PointLight Light(glm::vec3{ 2.0f, 2.0f, 2.0f }, glm::vec3{ 23.47f, 21.31f, 20.79f });

    shaderProgramm.use();
    Light.loadInShader(shaderProgramm, 0);
    shaderProgramm.setUniformInt("numPointLights", 1);
    shaderProgramm.setUniformMatrix4("projection", freeCam.getProjectionMatrix());

    for (int i = 0; i < 5; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        Textures[i]->bind(GL_TEXTURE_2D);
        shaderProgramm.setUniformInt(TextureNames[i], i);
    }
    glActiveTexture(GL_TEXTURE0);


    // 6. Render loop
    while (!glfwWindowShouldClose(window)) {

        float currentFrameTime = (float)glfwGetTime();
        System::deltaTime = currentFrameTime - System::lastFrameTime;
        System::lastFrameTime = currentFrameTime;

        processKeyInput(window);

        clearScreen();

        shaderProgramm.use();
        shaderProgramm.setUniformVec3("viewerPos", freeCam.getPos());
        shaderProgramm.setUniformMatrix4("view", freeCam.getViewMatrix());
        shaderProgramm.setUniformVec3("AlbedoColor", AlbedoColor);
        shaderProgramm.setUniformFloat("RoughnessValue", Roughness);
        shaderProgramm.setUniformFloat("MetallicValue", Metallic);
        shaderProgramm.setUniformBool("bIsTextured", bIsTextured);
        Sphere.draw(shaderProgramm);

        RenderGUI(bRenderGUI);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Process pending events
        glfwPollEvents();

    }

    return 0;
}