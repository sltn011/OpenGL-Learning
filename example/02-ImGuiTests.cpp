#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "GLFWInitRAII.hpp"
#include "CameraStatic.hpp"
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

constexpr int screenWidth = 1920;
constexpr int screenHeight = 1080;
constexpr int numSamples = 4;
constexpr auto glslVersion = "#version 330";

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
}

void keyCallback(
    GLFWwindow *window,
    int key,
    int scancode,
    int action,
    int mods
) {
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

    OGL::CameraStatic camera{
        camPos,
        camForward,
        {0.0f, 1.0f, 0.0f},
        45.0f,
        float(screenWidth) / float(screenHeight),
        0.01f,
        100.0f
    };

    shader.use(); shader.showWarnings(true);
    shader.setUniformMatrix4("view", camera.getViewMatrix());
    shader.setUniformMatrix4("projection", camera.getProjectionMatrix());
    shader.setUniformVec3("material.colorAmbient", glm::vec3(0.2f));
    shader.setUniformVec3("material.colorDiffuse", glm::vec3(0.5f));
    light.loadInShader(shader);

    float prevTime = 0.0f;

    glm::vec3 rotVec(0.0f, 1.0f, 0.0f);
    float rotAngle = 0.0f;


    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        clearScreen();

        float currentTime = float(glfwGetTime());
        //object.m_rotationAngle += glm::abs((currentTime - prevTime) * 10);
        prevTime = currentTime;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        shader.use();
        shader.setUniformVec3("objectColor", objectColor);
        object.drawShape(shader);

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

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

}
