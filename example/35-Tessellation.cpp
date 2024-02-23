#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <cmath>

#include "GLFWInitRAII.hpp"
#include "Shader.hpp"
#include "CameraFree.hpp"
#include "Plane.hpp"
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
int RenderMode = 0;
OGL::Shader *CurrentShader[2];
int ShaderInd = 0;
bool bEnableHeightMap = false;

int TessLevelOuter[4] = { 64, 64, 64, 64 };
int TessLevelInner[2] = { 64, 64 };

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

    if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
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

    ImGui::Begin("Tesselation");
    ImGui::Text("Display Mode:");
    if (ImGui::Button(RenderMode ? "Wireframe" : "Solid"))
    {
        RenderMode = (RenderMode + 1) % 2;
        if (RenderMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    ImGui::Text("Shader");
    if (ImGui::Button(ShaderInd ? "Tessellation" : "Simple"))
    {
        ShaderInd = (ShaderInd + 1) % 2;
    }
    ImGui::Text("Tesselation Control");
    ImGui::InputInt4("Outer", TessLevelOuter);
    ImGui::InputInt2("Inner", TessLevelInner);
    ImGui::Checkbox("Use Heightmap", &bEnableHeightMap);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/*
 * PRESS TAB ON KEYBOARD TO TOGGLE GUI
 */
int main(
) {
    srand(time(0));

    // Initialize GLFW
    OGL::GLFWInitRAII glfwInitializer;

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow *window = glfwCreateWindow(Screen::width, Screen::height, "Tessellation", nullptr, nullptr);
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

    InitImGUI(window, "#version 410");

    glEnable(GL_DEPTH_TEST);

    glPatchParameteri(GL_PATCH_VERTICES, 4);

    OGL::Plane Plane(50, 50, 4);
    glm::mat4 ModelMatrix = glm::identity<glm::mat4>();
    ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -2.0f, -4.0f));

    OGL::Shader simpleShaderProgramm("shaders/31-tessSimple.vert", "shaders/31-tessSimple.frag");
    OGL::Shader tessShaderProgramm("shaders/31-tess.vert", "shaders/31-tess.tesc", "shaders/31-tess.tese", "shaders/31-tess.frag");

    CurrentShader[0] = &simpleShaderProgramm;
    CurrentShader[1] = &tessShaderProgramm;

    OGL::Texture Heightmap("images", "heightmap.png", GL_TEXTURE_2D);
    OGL::Texture::setActive(GL_TEXTURE0);
    OGL::Texture::bind(GL_TEXTURE_2D, Heightmap.value());

    simpleShaderProgramm.use();
    simpleShaderProgramm.setUniformMatrix4("model", ModelMatrix);
    simpleShaderProgramm.setUniformMatrix4("projection", freeCam.getProjectionMatrix());
    tessShaderProgramm.use();
    tessShaderProgramm.setUniformMatrix4("model", ModelMatrix);
    tessShaderProgramm.setUniformMatrix4("projection", freeCam.getProjectionMatrix());
    tessShaderProgramm.setUniformInt("heightMap", 0);
    // Render loop
    while (!glfwWindowShouldClose(window)) {

        float currentFrameTime = (float)glfwGetTime();
        System::deltaTime = currentFrameTime - System::lastFrameTime;
        System::lastFrameTime = currentFrameTime;

        processKeyInput(window);

        clearScreen();

        OGL::Shader *shader = CurrentShader[ShaderInd];

        shader->use();
        shader->setUniformMatrix4("view", freeCam.getViewMatrix());

        if (ShaderInd)
        {
            shader->setUniformInt("TessLevelOuter[0]", TessLevelOuter[0]);
            shader->setUniformInt("TessLevelOuter[1]", TessLevelOuter[1]);
            shader->setUniformInt("TessLevelOuter[2]", TessLevelOuter[2]);
            shader->setUniformInt("TessLevelOuter[3]", TessLevelOuter[3]);
            shader->setUniformInt("TessLevelInner[0]", TessLevelInner[0]);
            shader->setUniformInt("TessLevelInner[1]", TessLevelInner[1]);
            shader->setUniformBool("bEnableHeightMap", bEnableHeightMap);
            Plane.drawPatches(*shader);
        }
        else
        {
            Plane.draw(*shader);
        }

        RenderGUI(bRenderGUI);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Process pending events
        glfwPollEvents();

    }

    return 0;
}