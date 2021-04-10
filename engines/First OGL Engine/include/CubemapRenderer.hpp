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

    class CubemapRenderer {
     public:
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

        Cubemap render( 
            Scene &scene, 
            int cubemapSize,
            GLenum cubemapTextureUnit,
            glm::vec3 mirrorPos,
            NormalRenderer *normalRenderer,
            SkyboxRenderer *skyboxRenderer, 
            TransparentRenderer *transparentRenderer,
            InstancesRenderer *instancesRenderer
        );

     protected:
        void renderSide(
            Scene &scene,
            CameraCubemap const *cubemapCamera,
            Cubemap &cubemap, 
            NormalRenderer *normalRenderer,
            SkyboxRenderer *skyboxRenderer,
            TransparentRenderer *transparentRenderer,
            InstancesRenderer *instancesRenderer
        );
    };

} // OGL::E1

#endif // OGL_E1_CUBEMAPRENDERER_H
