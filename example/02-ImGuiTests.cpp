#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "GLFWInitRAII.hpp"
#include "CameraFree.hpp"
#include "Object.hpp"
#include "DirectionalLight.hpp"

#include "VertexArrayObject.hpp"
#include "VertexBufferObject.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glm/gtc/quaternion.hpp"

#include <memory>

constexpr int screenWidth = 1920;
constexpr int screenHeight = 1080;
constexpr int numSamples = 4;
constexpr auto glslVersion = "#version 330";

bool openGUI = false;

std::unique_ptr<OGL::CameraFree> cameraPtr;

namespace System {
    float deltaTime = 0.0f;
    float lastFrameTime = 0.0f;

    float lastMouseXPos = screenWidth / 2;
    float lastMouseYPos = screenHeight / 2;

    float mouseSensitivity = 0.15f;
}

void windowResizeCallback(
    GLFWwindow *window,
    int newWidth,
    int newHeight
) {
    glViewport(0, 0, newWidth, newHeight);
}

void processInput(
    GLFWwindow *window
) {
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (!openGUI) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            cameraPtr->processMoveInput(OGL::CameraMovementDirection::Forward, System::deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            cameraPtr->processMoveInput(OGL::CameraMovementDirection::Backward, System::deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            cameraPtr->processMoveInput(OGL::CameraMovementDirection::Left, System::deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            cameraPtr->processMoveInput(OGL::CameraMovementDirection::Right, System::deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            cameraPtr->processMoveInput(OGL::CameraMovementDirection::Up, System::deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            cameraPtr->processMoveInput(OGL::CameraMovementDirection::Down, System::deltaTime);
        }
    }
}

void keyCallback(
    GLFWwindow *window,
    int key,
    int scancode,
    int action,
    int mods
) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        openGUI ^= true;
    }
}

void mouseCallback(
    GLFWwindow *window,
    double xpos,
    double ypos
) {
    if (!openGUI) {
        float xOffset = static_cast<float>(xpos) - System::lastMouseXPos;
        float yOffset = System::lastMouseYPos - static_cast<float>(ypos);
        System::lastMouseXPos = static_cast<float>(xpos);
        System::lastMouseYPos = static_cast<float>(ypos);

        cameraPtr->processRotateInput(xOffset, yOffset, System::mouseSensitivity, true);
    }
}

void clearScreen(
) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int main() {
    OGL::GLFWInitRAII glfwInitializer;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, numSamples);

    GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "GUI Tests", nullptr, nullptr);
    if (!window) {
        throw OGL::Exception("Error creating window!");
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, windowResizeCallback);

    glfwSetKeyCallback(window, keyCallback);

    glfwSetCursorPosCallback(window, mouseCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw OGL::Exception("Error loading GLAD!");
    }



    // INIT IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);

    ImGui::StyleColorsDark();



    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.6f, 0.6f, 0.7f, 1.0f);

    OGL::Shader shader("shaders/02-imguiTest.vert", "shaders/02-imguiTest.frag");

    OGL::Model model{ "models/Axises/axises.obj" };

    glm::vec3 objectPosition = glm::vec3{ 0.0f };
    OGL::Object object{ model, objectPosition };

    glm::vec3 objectColor{ 0.0f, 1.0f, 0.0f };

    glm::vec3 camPos = glm::vec3{ 0.0f, 2.0f, 4.0f } + objectPosition;
    glm::vec3 camForward = glm::normalize(objectPosition - camPos);

    OGL::DirectionalLight light{ {-0.3f, -0.5f, -0.2f} };

    cameraPtr = std::make_unique<OGL::CameraFree>(
        camPos,
        camForward,
        glm::vec3{0.0f, 1.0f, 0.0f},
        1.0f,
        0.0f,
        0.0f,
        45.0f,
        float(screenWidth) / float(screenHeight),
        0.01f,
        100.0f
     );

    shader.use();
    shader.setUniformMatrix4("view", cameraPtr->getViewMatrix());
    shader.setUniformMatrix4("projection", cameraPtr->getProjectionMatrix());
    shader.setUniformVec3("material.colorAmbient", glm::vec3(0.2f));
    shader.setUniformVec3("material.colorDiffuse", glm::vec3(0.5f));
    light.loadInShader(shader);

    float prevTime = 0.0f;

    glm::vec3 rotVec(0.0f, 1.0f, 0.0f);
    float rotAngle = 0.0f;


    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        float currentFrameTime = (float)glfwGetTime();
        System::deltaTime = currentFrameTime - System::lastFrameTime;
        System::lastFrameTime = currentFrameTime;

        clearScreen();

        if (openGUI) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPos(window, System::lastMouseXPos, System::lastMouseYPos);
        }

        shader.use();
        shader.setUniformMatrix4("view", cameraPtr->getViewMatrix());
        shader.setUniformMatrix4("projection", cameraPtr->getProjectionMatrix());
        shader.setUniformVec3("objectColor", objectColor);
        object.drawShape(shader);

        if (openGUI) {
            ImGui::Begin("GUI window");
            ImGui::ColorPicker3("Object Color", &(objectColor.x));
            if (ImGui::Button("Reset")) {
                object.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
                object.setScale(1.0f);
                object.setRotation(glm::angleAxis(0.0f, glm::vec3{ 0.0f, 1.0f, 0.0f }));
            }
            if (ImGui::Button("Position test")) {
                object.setPosition(object.getPosition() + glm::vec3(0.1f, 0.0f, 0.0f));
            }
            if (ImGui::Button("Scale test")) {
                object.setScale(object.getScale() * 0.95f);
            }
            if (ImGui::Button("Rotation test 1")) {
                object.setRotation(glm::angleAxis(glm::radians(90.0f), glm::vec3{ 0.0f, 1.0f, 0.0f }));
            }
            if (ImGui::Button("Rotation test 2")) {
                object.setRotation(90.0f, glm::vec3{ 0.0f, 1.0f, 0.0f });
            }
            if (ImGui::Button("Rotation test 3")) {
                object.setRotation(0.0f, 90.0f, 0.0f);
            }
            if (ImGui::Button("Rotation test 4")) {
                object.setRotation(glm::vec3{ 0.0f, 90.0f, 0.0f });
            }
            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

}
