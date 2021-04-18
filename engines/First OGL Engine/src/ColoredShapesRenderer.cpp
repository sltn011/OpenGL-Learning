#include "ColoredShapesRenderer.hpp"

namespace OGL::E1 {

    ColoredShapesRenderer::ColoredShapesRenderer(
        Shader &&shader
    ) : m_shader{ std::move(shader) } {

    }

    std::pair<FrameBufferObject, std::unordered_map<int, Object*>> ColoredShapesRenderer::render(
        Scene &scene,
        BasicCamera const *camera,
        int screenWidth,
        int screenHeight
    ) {
        FrameBufferObject fbo;
        fbo.bind(GL_FRAMEBUFFER);

        ColorBufferObject cbo;
        cbo.allocateStorage(screenWidth, screenHeight, GL_TEXTURE_2D, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
        fbo.attachColorBuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, std::move(cbo));

        RenderBufferObject rbo;
        rbo.allocateStorage(screenWidth, screenHeight, GL_DEPTH_COMPONENT32F);
        fbo.attachRenderBuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, std::move(rbo));

        if (!fbo.isComplete(GL_FRAMEBUFFER)) {
            throw Exception("Error creating framebuffer");
        }

        glm::vec4 origClearColor;
        glGetFloatv(GL_COLOR_CLEAR_VALUE, &(origClearColor.x));
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_shader.use();
        m_shader.setUniformMatrix4("view", camera->getViewMatrix());
        m_shader.setUniformMatrix4("projection", camera->getProjectionMatrix());

        std::unordered_map<int, Object*> colorObjMap;
        
        int colorR = 1, colorG = 0, colorB = 0;
        constexpr float divisor = 1.0f / 255.0f;
        
        for (auto &obj : scene.getNormalObjs()) {

            glm::vec3 color = glm::vec3{ colorR, colorG, colorB } * divisor;

            m_shader.setUniformVec3("objectColor", color);

            int colorCode = rgbColorToInt(colorR, colorG, colorB);
            colorObjMap.emplace(colorCode, &obj);
            obj.drawShape(m_shader);

            nextColor(colorR, colorG, colorB);
        }
        
        for (auto &obj : scene.getTransparentObjs()) {

            glm::vec3 color = glm::vec3{ colorR, colorG, colorB } * divisor;

            m_shader.setUniformVec3("objectColor", color);

            int colorCode = rgbColorToInt(colorR, colorG, colorB);
            colorObjMap.emplace(colorCode, &obj);

            nextColor(colorR, colorG, colorB);
        }

        for (auto &[obj, cubemap] : scene.getMirrorObjs()) {

            glm::vec3 color = glm::vec3{ colorR, colorG, colorB } * divisor;

            m_shader.setUniformVec3("objectColor", color);

            int colorCode = rgbColorToInt(colorR, colorG, colorB);
            colorObjMap.emplace(colorCode, &obj);

            nextColor(colorR, colorG, colorB);
        }
        
        glFlush();
        glFinish();

        FrameBufferObject::unbind(GL_FRAMEBUFFER);
        glClearColor(origClearColor.r, origClearColor.g, origClearColor.b, origClearColor.a);
        return std::make_pair(std::move(fbo), std::move(colorObjMap));
    }

    void ColoredShapesRenderer::setShader(
        Shader &&shader
    ) {
        m_shader = std::move(shader);
    }

    Shader &ColoredShapesRenderer::getShader(
    ) {
        return m_shader;
    }

    int ColoredShapesRenderer::rgbColorToInt(
        int r,
        int g,
        int b
    ) const {
        return r + g * 1000 + b * 1000000;
    }

    void ColoredShapesRenderer::nextColor(
        int &r,
        int &g,
        int &b
    ) const {
        r++;
        if (r == 256) {
            r = 0;
            g++;
            if (g == 256) {
                g = 0;
                b++;
            }
        }
    }

} // OGL::E1