#include "LightSourcesDebugRenderer.hpp"

namespace OGL::E1 {

    LightSourcesDebugRenderer::LightSourcesDebugRenderer(
        Shader &&shader,
        float lightCubeSize
    ) : m_shader{ std::move(shader) },
        m_lightCubeSize{ lightCubeSize } {

        float constexpr cubeVertices[] = {
            -1.0f, -1.0f, -1.0f, // 1
            -1.0f, -1.0f, +1.0f, // 2
            +1.0f, -1.0f, +1.0f, // 3
            +1.0f, -1.0f, -1.0f, // 4
            -1.0f, +1.0f, -1.0f, // 5
            -1.0f, +1.0f, +1.0f, // 6
            +1.0f, +1.0f, +1.0f, // 7
            +1.0f, +1.0f, -1.0f, // 8
        };

        int constexpr cubeIndices[] = {
            4, 0, 3,    7, 3, 2,    6, 2, 1,
            3, 7, 4,    2, 6, 7,    1, 5, 6,

            5, 1, 0,    5, 4, 7,    0, 1, 2,
            0, 4, 5,    7, 6, 5,    2, 3, 0, 
        };

        m_vao.bind();
        m_vbo.bind();
        m_ebo.bind();
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), &cubeIndices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        VertexArrayObject::unbind();

    }

    void LightSourcesDebugRenderer::render(
        Scene &scene,
        BasicCamera const *camera
    ) {
        m_shader.use();
        m_shader.setUniformMatrix4("view", camera->getViewMatrix());
        m_shader.setUniformMatrix4("projection", camera->getProjectionMatrix());

        m_vao.bind();
        
        for (auto const &[light, pShadow] : scene.getPointLights()) {
            glm::mat4 model = glm::identity<glm::mat4>();
            model = glm::translate(model, light.m_position);
            model = glm::scale(model, glm::vec3{ m_lightCubeSize, m_lightCubeSize, m_lightCubeSize });
            m_shader.setUniformMatrix4("model", model);
            m_shader.setUniformVec3("color", light.m_color);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
        }

        for (auto const &light : scene.getSpotLights()) {
            glm::mat4 model = glm::identity<glm::mat4>();
            model = glm::translate(model, light.m_position);
            model = glm::scale(model, glm::vec3{ m_lightCubeSize, m_lightCubeSize, m_lightCubeSize });
            m_shader.setUniformMatrix4("model", model);
            m_shader.setUniformVec3("color", light.m_color);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
        }

        VertexArrayObject::unbind();
    }

    void LightSourcesDebugRenderer::setShader(
        Shader &&shader
    ) {
        m_shader = std::move(shader);
    }

    Shader &LightSourcesDebugRenderer::getShader(
    ) {
        return m_shader;
    }

} // OGL::E1