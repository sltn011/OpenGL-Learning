#ifndef OGL_E1_SCENE_H
#define OGL_E1_SCENE_H

#include "First OGL Engine/Utils/EngineTypes.hpp"
#include "First OGL Engine/Utils/factory.hpp"
#include "Cubemap.hpp"
#include "Skybox.hpp"
#include "Object.hpp"
#include "CameraFirstPerson.hpp"
#include "CameraFree.hpp"

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
        
        objectsVec &getNormalObjs
        (
        );

        objectsVec &getTransparentObjs
        (
        );

        mirrorObjsVec &getMirrorObjs
        (
        );

        void replaceSkybox
        ( smartSkybox &&skybox
        );

        smartSkybox &getSkybox
        (
        );

    private:
        smartCamPtr m_camera;

        objectsVec m_normalObjects;
        objectsVec m_transparentObjects;
        mirrorObjsVec m_mirrorObjects;

        smartSkybox m_skybox;
    };

} // OGL::E1

#endif // OGL_E1_SCENE_H
