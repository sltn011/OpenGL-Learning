#ifndef OGL_E1_SCENE_H
#define OGL_E1_SCENE_H

#include "Utils/EngineTypes.hpp"
#include "Cubemap.hpp"
#include "Skybox.hpp"
#include "ShadowMap.hpp"
#include "ShadowCubemap.hpp"
#include "Object.hpp"
#include "CameraFirstPerson.hpp"
#include "CameraFree.hpp"
#include "DirectionalLight.hpp"
#include "PointLight.hpp"
#include "SpotLight.hpp"

namespace OGL::E1 {

    class Scene {
     public:
        Scene( 
            smartCamPtr &&camera
        );

        Scene( 
            Scene const &rhs
        ) = delete;

        Scene( 
            Scene &&rhs
        ) = default;

        Scene &operator=(
            Scene const &rhs
        ) = delete;

        Scene &operator=(
            Scene &&rhs
        ) = default;

        Object &addNormalObj( 
            Object obj
        );

        Object &addTransparentObj(
            Object obj
        );

        std::pair<Object, smartCubemap> &addMirrorObj(
            Object obj
        );

        std::pair<Object, size_t> &addInstancedObj(
            Object obj,
            size_t numInstances
        );

        std::pair<DirectionalLight, smartShadowMap> &addDirLight( 
            DirectionalLight dirLight
        );

        std::pair<PointLight, smartShadowCubemap> &addPointLight(
            PointLight pointLight
        );

        std::pair<SpotLight, smartShadowMap> &addSpotLight(
            SpotLight spotLight
        );
        
        objectsVec &getNormalObjs(
        );

        objectsVec &getTransparentObjs(
        );

        mirrorObjsVec &getMirrorObjs(
        );

        instancedObjsVec &getInstancedObjs(
        );

        dirLights &getDirLights(
        );

        pointLights &getPointLights(
        );

        spotLights &getSpotLights(
        );

        void replaceSkybox( 
            smartSkybox &&skybox
        );

        smartSkybox &getSkybox(
        );

        smartCamPtr &getCamera(
        );

     private:
        smartCamPtr m_camera;

        objectsVec m_normalObjects;
        objectsVec m_transparentObjects;
        mirrorObjsVec m_mirrorObjects;
        instancedObjsVec m_instancedObjects;

        dirLights m_dirLights;
        pointLights m_pointLights;
        spotLights m_spotLights;

        smartSkybox m_skybox;
    };

} // OGL::E1

#endif // OGL_E1_SCENE_H
