/**
* @file LevelLoader.hpp
*/

#ifndef OGL_E1_LEVELLOADER_HPP
#define OGL_E1_LEVELLOADER_HPP

#include "Scene.hpp"
#include "json.hpp"
#include <fstream>
#include <string>
#include <array>

namespace OGL::E1 {

    /**
     * @brief Class to load Scene data from json file
    */
    class LevelLoader {
     public:
        /**
         * @brief Loads Scene data from json file
         * @param screenWidth Width of screen width
         * @param screenHeight Height of screen height
         * @param filePath Path to json file
         * @param modelsTable Hashmap in which will be loaded data: Key - Model's ID, Value - Model object
         * @param scene Reference to object in which Scene with loaded data will be loaded
        */
        void load(
            int screenWidth,
            int screenHeight,
            std::string const &filePath,
            modelsTable &modelsTable,
            smartScenePtr &scene
        );

     protected:

         /**
          * @brief Using data in json loads hashmap: Key - Model's ID, Value - Model object 
          * @param levelJSON json file
          * @return Hashmap: Key - Model's ID, Value - Model object 
         */
         modelsTable getModelsTable(
             nlohmann::json const &levelJSON
         ) const;

         /**
          * @brief Using data in json loads CameraFree object
          * @param levelJSON json file
          * @param aspect Aspect ratio of screen - Width/Height
          * @return CameraFree object
         */
         smartCamPtr getCamera(
             nlohmann::json const &levelJSON,
             float aspect
         ) const;

         /**
          * @brief Loads normal Object from json object
          * @param objJSON json object
          * @param modelsTable Hashmap: Key - Model's ID, Value - Model object 
          * @return Normal Object loaded from json object
         */
         Object getNormalObj(
             nlohmann::json const &objJSON,
             modelsTable &modelsTable
         ) const;

         /**
          * @brief Loads normal Objects from json file
          * @param levelJSON json file
          * @param modelsTable Hashmap: Key - Model's ID, Value - Model object 
          * @return Container with normal Objects
         */
         objectsVec getNormalObjs(
             nlohmann::json const &levelJSON,
             modelsTable &modelsTable
         ) const;

         /**
          * @brief Loads transparent Object from json object
          * @param objJSON json object
          * @param modelsTable Hashmap: Key - Model's ID, Value - Model object
          * @return Transparent Object loaded from json object
         */
         Object getTransparentObj(
             nlohmann::json const &objJSON,
             modelsTable &modelsTable
         ) const;

         /**
          * @brief Loads transparent Objects from json file
          * @param levelJSON json file
          * @param modelsTable Hashmap: Key - Model's ID, Value - Model object
          * @return Container with transparent Objects
         */
         objectsVec getTransparentObjs(
             nlohmann::json const &levelJSON,
             modelsTable &modelsTable
         ) const;

         /**
          * @brief Loads mirror Object from json object
          * @param objJSON json object
          * @param modelsTable Hashmap: Key - Model's ID, Value - Model object
          * @return Mirror Object loaded from json object
         */
         std::pair<Object, maybeCubemap> getMirrorObj(
             nlohmann::json const &objJSON,
             modelsTable &modelsTable
         ) const;

         /**
          * @brief Loads mirror Objects from json file
          * @param levelJSON json file
          * @param modelsTable Hashmap: Key - Model's ID, Value - Model object
          * @return Container with mirror Objects
         */
         mirrorObjsVec getMirrorObjs(
             nlohmann::json const &levelJSON,
             modelsTable &modelsTable
         ) const;

         /**
          * @brief Loads instanced Object from json object
          * @param objJSON json object
          * @param modelsTable Hashmap: Key - Model's ID, Value - Model object
          * @return Instanced Object loaded from json object
         */
         std::pair<Object, uint32_t> getInstancedObj(
             nlohmann::json const &objJSON,
             modelsTable &modelsTable
         ) const;

         /**
          * @brief Loads instanced Objects from json file
          * @param levelJSON json file
          * @param modelsTable Hashmap: Key - Model's ID, Value - Model object
          * @return Container with instanced Objects
         */
         instancedObjsVec getInstancedObjs(
             nlohmann::json const &levelJSON,
             modelsTable &modelsTable
         ) const;

         /**
          * @brief Loads DirectionalLight from json object
          * @param lightJSON json object
          * @return DirectionalLight loaded from json object
         */
         DirectionalLight getDirLight(
             nlohmann::json const &lightJSON
         ) const;

         /**
          * @brief Loads DirectionalLights from json file
          * @param levelJSON json file
          * @return Container with DirectionalLights and empty ShadowMaps
         */
         dirLights getDirLights(
             nlohmann::json const &levelJSON
         ) const;

         /**
          * @brief Loads PointLight from json object
          * @param lightJSON json object
          * @return PointLight loaded from json object
         */
         PointLight getPointLight(
             nlohmann::json const &lightJSON
         ) const;

         /**
          * @brief Loads PointLights from json file
          * @param levelJSON json file
          * @return Container with PointLights and empty ShadowCubemaps
         */
         pointLights getPointLights(
             nlohmann::json const &levelJSON
         ) const;

         /**
          * @brief Loads SpotLight from json object
          * @param lightJSON json object
          * @return SpotLight loaded from json object
         */
         SpotLight getSpotLight(
             nlohmann::json const &lightJSON
         ) const;

         /**
          * @brief Loads SpotLights from json file
          * @param levelJSON json file
          * @return Container with SpotLights and empty ShadowMaps
         */
         spotLights getSpotLights(
             nlohmann::json const &levelJSON
         ) const;

         /**
          * @brief Loads Skybox from json file
          * @param levelJSON json file
          * @return Skybox object if successful, std::nullopt otherwise
         */
         maybeSkybox getSkybox(
             nlohmann::json const &levelJSON
         ) const;
         
    };

} // OGL::E1

#endif // OGL_E1_LEVELLOADER_HPP
