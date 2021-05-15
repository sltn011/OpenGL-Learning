/**
* @file Scene.hpp
*/

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

    /**
     * @brief Class that represents ingame scene
    */
    class Scene {
     private:
        /// Scene's camera
        smartCamPtr m_camera;

        /// Container with Scene's normal Objects
        objectsVec m_normalObjects;

        /// Container with Scene's transparent Objects
        objectsVec m_transparentObjects;

        /// Container with Scene's mirror Objects and their Cubemaps
        mirrorObjsVec m_mirrorObjects;

        /// Container with Scene's normal Objects and their numbers of instances
        instancedObjsVec m_instancedObjects;

        /// Container with Scene's DirectionalLights and their ShadowMaps
        dirLights m_dirLights;

        /// Container with Scene's PointLights and their ShadowCubemaps
        pointLights m_pointLights;

        /// Container with Scene's SpotLights and their ShadowMaps
        spotLights m_spotLights;

        /// Container with Scene's Skybox
        maybeSkybox m_skybox;

     public:
        /**
         * @brief Scene constructor
         * @param camera Camera used in Scene
        */
        explicit Scene(
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

        /**
         * @brief Adds normal Object into Scene
         * @param obj New normal Object
         * @return Reference to new normal Object in Scene's container
        */
        Object &addNormalObj( 
            Object obj
        );

        /**
         * @brief Adds normal Object into Scene
         * @param obj New normal Object
         * @return Reference to new normal Object in Scene's container
        */
        Object &addTransparentObj(
            Object obj
        );

        /**
         * @brief Adds mirror Object with empty Cubemap into Scene
         * @param obj New mirror Object
         * @return Reference to pair of new mirror Object and it's empty Cubemap in Scene's container
        */
        pMirrorCubemap &addMirrorObj(
            Object obj
        );

        /**
         * @brief Adds instanced Object into Scene
         * @param obj New instanced Object
         * @param numInstances number of Object's instances
         * @return Reference to pair of new instanced Object and it's number of instances in Scene's container
        */
        pObjInstancesNum &addInstancedObj(
            Object obj,
            uint32_t numInstances
        );

        /**
         * @brief Adds DirectionalLight and it's empty ShadowMap into Scene
         * @param dirLight New DirectionalLight
         * @return Reference to pair of new DirectionalLight and it's empty ShadowMap in Scene's container
        */
        pDirLightShadow &addDirLight(
            DirectionalLight dirLight
        );

        /**
         * @brief Adds PointLight and it's empty ShadowCubemap into Scene
         * @param pointLight New PointLight
         * @return Reference to pair of new PointLight and it's empty ShadowCubemap in Scene's container
        */
        pPointLightShadows &addPointLight(
            PointLight pointLight
        );

        /**
         * @brief Adds SpotLight and it's empty ShadowMap into Scene
         * @param spotLight New SpotLight
         * @return Reference to pair of new SpotLight and it's empty ShadowMap in Scene's container
        */
        pSpotLightShadows &addSpotLight(
            SpotLight spotLight
        );
        
        /**
         * @brief Get container of normal Objects
         * @return Container of normal Objects
        */
        objectsVec &getNormalObjs(
        );

        /**
         * @brief Get container of transparent Objects
         * @return Container of transparent Objects
        */
        objectsVec &getTransparentObjs(
        );

        /**
         * @brief Get container of mirror Objects and their Cubemaps
         * @return Container of mirror Objects and their Cubemaps
        */
        mirrorObjsVec &getMirrorObjs(
        );

        /**
         * @brief Get container of instanced Objects and their number of instances
         * @return Container of instanced Objects and their number of instances
        */
        instancedObjsVec &getInstancedObjs(
        );

        /**
         * @brief Get container of DirectionalLights and their ShadowMaps
         * @return Container of DirectionalLights and their ShadowMaps
        */
        dirLights &getDirLights(
        );

        /**
         * @brief Get container of PointLights and their ShadowCubemaps
         * @return Container of PointLights and their ShadowCubemaps
        */
        pointLights &getPointLights(
        );

        /**
         * @brief Get container of SpotLights and their ShadowMaps
         * @return Container of SpotLights and their ShadowMaps
        */
        spotLights &getSpotLights(
        );

        /**
         * @brief Set Skybox used in Scene
         * @param skybox New Skybox
        */
        void replaceSkybox( 
            Skybox &&skybox
        );

        /**
         * @brief Get reference to Scene's Skybox if it exists, std::nullopt otherwise
         * @return Reference to Scene's Skybox if it exists, std::nullopt otherwise
        */
        maybeSkybox &getSkybox(
        );

        /**
         * @brief Get reference to Scene's camera
         * @return Reference to Scene's camera
        */
        smartCamPtr &getCamera(
        );

    };

} // OGL::E1

#endif // OGL_E1_SCENE_H
