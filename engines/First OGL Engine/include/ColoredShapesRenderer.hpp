#ifndef OGL_E1_COLOREDSHAPESRENDERER_H
#define OGL_E1_COLOREDSHAPESRENDERER_H

#include "Utils/EngineTypes.hpp"
#include "Shader.hpp"
#include "Scene.hpp"

namespace OGL::E1 {

    class ColoredShapesRenderer {
    protected:
        Shader m_shader;

    public:
        ColoredShapesRenderer(
            Shader &&shader
        );

        ColoredShapesRenderer(
            ColoredShapesRenderer const &rhs
        ) = delete;

        ColoredShapesRenderer(
            ColoredShapesRenderer &&rhs
        ) = default;

        ColoredShapesRenderer &operator=(
            ColoredShapesRenderer const &rhs
        ) = delete;

        ColoredShapesRenderer &operator=(
            ColoredShapesRenderer &&rhs
        ) = default;

        std::pair<FrameBufferObject, std::unordered_map<int, Object*>> render(
            Scene &scene,
            BasicCamera const *camera,
            int screenWidth,
            int screenHeight
        );

        void setShader(
            Shader &&shader
        );

        Shader &getShader(
        );

     protected:
         int rgbColorToInt(
             int r, 
             int g, 
             int b
         ) const;

         void nextColor(
             int &r,
             int &g,
             int &b
         ) const;
    };

} // OGL::E1

#endif // OGL_E1_SHADOWMAPRENDERER_H
