#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "GLFWInitRAII.hpp"
#include "CameraFree.hpp"
#include "Object.hpp"
#include "DirectionalLight.hpp"

#include "UniformBufferObject.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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

namespace Object {
    float ambientStr = 0.3f;
    float ambientDelta = 0.01f;

    float specularStr = 0.5f;
    float specularDelta = 0.01f;

    int shininess = 32;
    int shininessDelta = 1;

    bool doExplode = false;
    float explodeBeginTime = 0.0f;

    bool showNormals = false;
}


OGL::CameraFree freeCam{
    {0.0f, 0.0f, 5.0f},
    {0.0f, 0.0f, -1.0f},
    {0.0f, 1.0f, 0.0f},
    5.0f, 45.0f, static_cast<float>(Screen::width) / static_cast<float>(Screen::height), 0.01f, 100.0f
};

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
    if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

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

void keyCallback(
    GLFWwindow *window,
    int key,
    int scancode,
    int action,
    int mods
) {
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        Object::doExplode ^= true;
        if (Object::doExplode) {
            Object::explodeBeginTime = (float)glfwGetTime();
        }
    }
    if (key == GLFW_KEY_N && action == GLFW_PRESS) {
        Object::showNormals ^= true;
    }
}

void mouseCallback( 
    GLFWwindow *window,
    double xpos, 
    double ypos
) {
    float xOffset = static_cast<float>(xpos) - System::lastMouseXPos;
    float yOffset = System::lastMouseYPos - static_cast<float>(ypos);
    System::lastMouseXPos = static_cast<float>(xpos);
    System::lastMouseYPos = static_cast<float>(ypos);

    freeCam.processRotateInput(xOffset, yOffset, System::mouseSensitivity, true);
}

void clearScreen(
) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int main(
) {
    OGL::GLFWInitRAII glfwInitializer;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(Screen::width, Screen::height, "Model Renderer", nullptr, nullptr);
    if (!window) {
        throw OGL::Exception("Error creating window!");
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, windowResizeCallback);

    glfwSetKeyCallback(window, keyCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetCursorPosCallback(window, mouseCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw OGL::Exception("Error loading GLAD!");
    }

    stbi_set_flip_vertically_on_load(true);

    OGL::Shader shader("shaders/0-renderer.vert", "shaders/0-renderer.geom", "shaders/0-renderer.frag");
    OGL::Shader normalsShader("shaders/0-normalsRender.vert", "shaders/0-normalsRender.geom", "shaders/0-normalsRender.frag");

    OGL::UniformBufferObject ubo;
    ubo.bind();
    ubo.allocateBufferData(2 * 64, GL_DYNAMIC_DRAW);
    ubo.setBindingPoint(0);
    shader.uniformBlockBinding("Matrices", 0);
    normalsShader.uniformBlockBinding("Matrices", 0);
    ubo.unbind();

    OGL::Model myModel("models/Backpack/backpack.obj");

    OGL::Object myObject(myModel);
    myObject.setPosition(glm::vec3{ 0.0f, 0.0f, -2.0f });
    myObject.setScale(1.2f);
    myObject.setRotation(-45.0f, glm::vec3(1.0f, 1.0f, 1.0f));

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glm::vec3 lightDir{ 1.0f, -1.0f, -1.0f };
    glm::vec3 lightColor{ 1.5f, 1.5f, 1.5f };
    OGL::DirectionalLight dirLight(lightDir, lightColor);

    ubo.bind();
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        float currentFrameTime = (float)glfwGetTime();
        System::deltaTime = currentFrameTime - System::lastFrameTime;
        System::lastFrameTime = currentFrameTime;

        clearScreen();

        ubo.setBufferData(0, sizeof(glm::mat4), glm::value_ptr(freeCam.getViewMatrix()));
        ubo.setBufferData(64, sizeof(glm::mat4), glm::value_ptr(freeCam.getProjectionMatrix()));

        shader.use();
        shader.setUniformVec3("viewerPos", freeCam.getPos());
        dirLight.loadInShader(shader);
        shader.setUniformBool("doExplode", Object::doExplode);
        shader.setUniformFloat("explodeTime", (float)glfwGetTime() - Object::explodeBeginTime);
        myObject.draw(shader);

        if (Object::showNormals) {
            normalsShader.use();
            myObject.draw(normalsShader);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}