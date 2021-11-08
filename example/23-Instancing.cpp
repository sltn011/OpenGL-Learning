#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <cmath>
#include <string>

#include "GLFWInitRAII.hpp"
#include "Shader.hpp"
#include "CameraFree.hpp"
#include "VertexArrayObject.hpp"
#include "VertexBufferObject.hpp"
#include "UniformBufferObject.hpp"
#include "Object.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

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
    {0.0f, 10.0f, 250.0f},
    {0.0f, 0.0f, -1.0f},
    5.0f, 45.0f, static_cast<float>(Screen::width) / static_cast<float>(Screen::height), 1.0f, 1000.0f
};

void framebufferSizeCallback(
    GLFWwindow *window,
    int width,
    int height
) {
    glViewport(0, 0, width, height);
}

void processKeyInput(
    GLFWwindow *window, 
    unsigned int moveSpeedMult
) {
    if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    // Camera Controls
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        freeCam.processMoveInput(OGL::CameraMovementDirection::Forward, System::deltaTime * moveSpeedMult);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        freeCam.processMoveInput(OGL::CameraMovementDirection::Backward, System::deltaTime * moveSpeedMult);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        freeCam.processMoveInput(OGL::CameraMovementDirection::Left, System::deltaTime * moveSpeedMult);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        freeCam.processMoveInput(OGL::CameraMovementDirection::Right, System::deltaTime * moveSpeedMult);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        freeCam.processMoveInput(OGL::CameraMovementDirection::Up, System::deltaTime * moveSpeedMult);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        freeCam.processMoveInput(OGL::CameraMovementDirection::Down, System::deltaTime * moveSpeedMult);
    }

    double xMousePos, yMousePos;
    glfwGetCursorPos(window, &xMousePos, &yMousePos);
    float xOffset = static_cast<float>(xMousePos) - System::lastMouseXPos;
    float yOffset = System::lastMouseYPos - static_cast<float>(yMousePos);
    System::lastMouseXPos = static_cast<float>(xMousePos);
    System::lastMouseYPos = static_cast<float>(yMousePos);
    freeCam.processRotateInput(xOffset, yOffset, System::mouseSensitivity, true);
}

void keyboardInputCallback(GLFWwindow *window,
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

std::unique_ptr<glm::mat4[]> generateTranslationMatrices(
    size_t numMatrices
) {
    std::unique_ptr<glm::mat4[]> matrices = std::make_unique<glm::mat4[]>(numMatrices);
    float radius = 250.0f;
    float offset = 15.0f;
    for (size_t i = 0; i < numMatrices; ++i) {
        // translate
        glm::mat4 matr(1.0);
        //matr = glm::translate(matr, center);
        float angle = static_cast<float>(i) / numMatrices * 360.0f;
        float displacement = (std::rand() % static_cast<int>(2 * 100 * offset)) / 100.0f - offset;
        float x = std::sin(angle) * radius + displacement;
        displacement = (std::rand() % static_cast<int>(2 * 100 * offset)) / 100.0f - offset;
        float y = displacement * 0.25f + (rand() % 20) - 10;
        displacement = (std::rand() % static_cast<int>(2 * 100 * offset)) / 100.0f - offset;
        float z = std::cos(angle) * radius + displacement;
        matr = glm::translate(matr, glm::vec3{ x, y, z });

        //scale [0.05, 0.25]
        float scale = (std::rand() % 20) / 100.0f + 0.05f;
        matr = glm::scale(matr, glm::vec3{ scale, scale, scale });

        //rotate
        float rotation = static_cast<float>(std::rand() % 360);
        matr = glm::rotate(matr, rotation, glm::vec3{std::rand(), std::rand(), std::rand()});

        matrices[i] = matr;
    }
    return matrices;
}

int main(
) {
    OGL::GLFWInitRAII glfwInitializer;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(Screen::width, Screen::height, "Instancing", nullptr, nullptr);
    if (!window) {
        std::cout << "Error creating window!" << std::endl;
        glfwTerminate();
        return 2;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    glfwSetKeyCallback(window, keyboardInputCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 3;
    }

    glViewport(0, 0, Screen::width, Screen::height);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    glEnable(GL_DEPTH_TEST);

    OGL::Shader shaderPlanet("shaders/19-instancingPlanet.vert", "shaders/19-instancingPlanet.frag");
    OGL::Shader shaderAsteroid("shaders/19-instancingAsteroid.vert", "shaders/19-instancingAsteroid.frag");

    OGL::Model planet("models/Planet/Planet/planet.obj");
    OGL::Model asteroid("models/Planet/Asteroid/asteroid.obj");

    glm::vec3 planetCenter{ 0.0f, -2.0f, -200.0f };

    OGL::Object planetObj(&planet, planetCenter, 20.0f);
    OGL::Object asteroidObj(&asteroid, planetCenter);

    size_t numInstances = 25000;

    std::unique_ptr<glm::mat4[]> asteroidTranslationMatrices = generateTranslationMatrices(numInstances);
    
    OGL::VertexBufferObject vbo;
    vbo.bind();
    glBufferData(GL_ARRAY_BUFFER, numInstances * sizeof(glm::mat4), asteroidTranslationMatrices.get(), GL_STATIC_DRAW);
    asteroidObj.setVertexAttribInstancedModelMat4(3);
    vbo.unbind();

    while (!glfwWindowShouldClose(window)) {

        float currentFrameTime = (float)glfwGetTime();
        System::deltaTime = currentFrameTime - System::lastFrameTime;
        System::lastFrameTime = currentFrameTime;

        processKeyInput(window, 10);

        clearScreen();

        shaderPlanet.use();
        shaderPlanet.setUniformMatrix4("view", freeCam.getViewMatrix());
        shaderPlanet.setUniformMatrix4("projection", freeCam.getProjectionMatrix());
        planetObj.draw(shaderPlanet);

        shaderAsteroid.use();
        shaderAsteroid.setUniformMatrix4("view", freeCam.getViewMatrix());
        shaderAsteroid.setUniformMatrix4("projection", freeCam.getProjectionMatrix());
        asteroidObj.drawInstanced(shaderAsteroid, numInstances);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    return 0;
}