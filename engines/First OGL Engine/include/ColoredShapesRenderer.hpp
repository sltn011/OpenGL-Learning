/**
* @file ColoredShapesRenderer.hpp
*/

#ifndef OGL_E1_COLOREDSHAPESRENDERER_H
#define OGL_E1_COLOREDSHAPESRENDERER_H

#include "Utils/EngineTypes.hpp"
#include "Shader.hpp"
#include "Scene.hpp"

namespace OGL::E1 {

    /**
     * @brief Class for rendering all normal, transparent and mirror Objects in Scene with unique color
    */
    class ColoredShapesRenderer {
    protected:
        /// Shader rendering Object's shape with uniform color
        Shader m_shader;

    public:
        /**
         * @brief ColoredShapesRenderer constructor
         * @param shader Shader rendering Objects shape with uniform color
        */
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

        /**
         * @brief Renders all normal, transparent and mirror objects in Scene with unique uniform colors from {1,0,0} to {255,255,255}. \n
         *        Empty space is colored {0,0,0}
         * @param scene Scene to be rendered
         * @param camera Camera used for rendering
         * @param screenWidth Width of screen
         * @param screenHeight Height of screen
         * @return Pair containing: \n
         * First - FrameBufferObject with rendered scene. \n
         * Second - HashMap with Key - Objects's shape color encoded as R + 1000 * G + 1000000 * B and Value - pointer to Object with this color
        */
        std::pair<FrameBufferObject, std::unordered_map<int, Object*>> render(
            Scene &scene,
            BasicCamera const *camera,
            int screenWidth,
            int screenHeight
        );

        /**
         * @brief Replace Shader with new one
         * @param shader New Shader object
        */
        void setShader(
            Shader &&shader
        );

        /**
         * @brief Get reference to owned Shader object
         * @return Reference to owned Shader object
        */
        Shader &getShader(
        );

     protected:
         /**
          * @brief Converts RGB color into it's integer color code: Code = R + 1000 * G + 1000000 * B
          * @param r Color's red value
          * @param g Color's green value
          * @param b Color's blue value
          * @return Integer color code: Code = R + 1000 * G + 1000000 * B
         */
         int rgbColorToInt(
             int r, 
             int g, 
             int b
         ) const;

         /**
          * @brief Takes RGB color values and increments them to make new unique color used in rendering
          * @param r Reference to color's red value
          * @param g Reference to color's green value
          * @param b Reference to color's blue value
         */
         void nextColor(
             int &r,
             int &g,
             int &b
         ) const;
    };

} // OGL::E1

#endif // OGL_E1_SHADOWMAPRENDERER_H
