/**
* @file CubemapRenderer.hpp
*/

#ifndef OGL_E1_CUBEMAPRENDERER_H
#define OGL_E1_CUBEMAPRENDERER_H

#include "Utils/EngineTypes.hpp"
#include "Object.hpp"
#include "Shader.hpp"
#include "Scene.hpp"
#include "CameraCubemap.hpp"

#include "FrameBufferObject.hpp"

#include "NormalRenderer.hpp"
#include "TransparentRenderer.hpp"
#include "SkyboxRenderer.hpp"
#include "InstancesRenderer.hpp"

namespace OGL::E1 {

    /**
     * @brief Class to render Scene into Cubemap
    */
    class CubemapRenderer {
     public:
        /**
         * @brief CubemapRenderer constructor
        */
        CubemapRenderer(
        );

        CubemapRenderer( 
            CubemapRenderer const &rhs
        ) = delete;

        CubemapRenderer( 
            CubemapRenderer &&rhs
        ) = default;

        CubemapRenderer &operator=( 
            CubemapRenderer const &rhs
        ) = delete;

        CubemapRenderer &operator=( 
            CubemapRenderer &&rhs
        ) = default;

        /**
         * @brief Renders Scene into Cubemap
         * @param scene Scene to be rendered
         * @param cubemapSize Size of Cubemap side
         * @param cubemapTextureUnit Cubemap's texture unit GL_TEXTURE{N}
         * @param cubemapPos Position of Cubemap in worldspace
         * @param normalRenderer NormalRenderer if normal Objects should be rendered into Cubemap, std::nullopt otherwise
         * @param skyboxRenderer SkyboxRenderer if Skybox should be rendered into Cubemap, std::nullopt otherwise
         * @param transparentRenderer TransparentRenderer if transparent Objects should be rendered into Cubemap, std::nullopt otherwise
         * @param instancesRenderer InstancesRenderer if instanced Objects should be rendered into Cubemap, std::nullopt otherwise
         * @return Created Cubemap
        */
        Cubemap render( 
            Scene &scene, 
            int cubemapSize,
            GLenum cubemapTextureUnit,
            glm::vec3 cubemapPos,
            maybeNormalRenderer &normalRenderer,
            maybeSkyboxRenderer &skyboxRenderer, 
            maybeTransparentRenderer &transparentRenderer,
            maybeInstancesRenderer &instancesRenderer
        );

     protected:
        /**
         * @brief Renders one side of Cubemap
         * @param scene Scene to be rendered
         * @param cubemapCamera Camera used to render Scene to Cubemap's side
         * @param normalRenderer NormalRenderer if normal Objects should be rendered into Cubemap, std::nullopt otherwise
         * @param skyboxRenderer SkyboxRenderer if Skybox should be rendered into Cubemap, std::nullopt otherwise
         * @param transparentRenderer TransparentRenderer if transparent Objects should be rendered into Cubemap, std::nullopt otherwise
         * @param instancesRenderer InstancesRenderer if instanced Objects should be rendered into Cubemap, std::nullopt otherwise
        */
        void renderSide(
            Scene &scene,
            CameraCubemap const *cubemapCamera,
            maybeNormalRenderer &normalRenderer,
            maybeSkyboxRenderer &skyboxRenderer,
            maybeTransparentRenderer &transparentRenderer,
            maybeInstancesRenderer &instancesRenderer
        );
    };

} // OGL::E1

#endif // OGL_E1_CUBEMAPRENDERER_H
