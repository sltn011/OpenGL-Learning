#ifndef OGL_E1_SCENE_H
#define OGL_E1_SCENE_H

#include "First OGL Engine/Utils/EngineTypes.hpp"
#include "Cubemap.hpp"
#include "Skybox.hpp"
#include "Object.hpp"
#include "CameraFirstPerson.hpp"
#include "CameraFree.hpp"
#include "DirectionalLight.hpp"
#include "PointLight.hpp"
#include "SpotLight.hpp"

namespace OGL::E1 {

    class Scene {
    public:
        Scene
        ( smartCamPtr &&camera
        );

        Scene
        ( Scene const &rhs
        ) = delete;

        Scene
        ( Scene &&rhs
        ) = default;

        Scene &operator=
        ( Scene const &rhs
        ) = delete;

        Scene &operator=
        ( Scene &&rhs
        ) = default;

        void addNormalObj
        ( smartObjPtr &&obj
        );

        void addTransparentObj
        ( smartObjPtr &&obj
        );

        void addMirrorObj
        ( smartObjPtr &&obj
        );

        void addDirLight
        ( DirectionalLight dirLight
        );

        void addPointLight
        ( PointLight pointLight
        );

        void addSpotLight
        ( SpotLight spotLight
        );
        
        objectsVec &getNormalObjs
        (
        );

        objectsVec &getTransparentObjs
        (
        );

        mirrorObjsVec &getMirrorObjs
        (
        );

        dirLights &getDirLights
        (
        );

        pointLights &getPointLights
        (
        );

        spotLights &getSpotLights
        (
        );

        void replaceSkybox
        ( smartSkybox &&skybox
        );

        smartSkybox &getSkybox
        (
        );

        smartCamPtr &getCamera
        (
        );

    private:
        smartCamPtr m_camera;

        objectsVec m_normalObjects;
        objectsVec m_transparentObjects;
        mirrorObjsVec m_mirrorObjects;

        dirLights m_dirLights;
        pointLights m_pointLights;
        spotLights m_spotLights;

        smartSkybox m_skybox;
    };

} // OGL::E1

#endif // OGL_E1_SCENE_H
