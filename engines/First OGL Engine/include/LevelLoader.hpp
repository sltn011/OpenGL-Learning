#ifndef OGL_E1_LEVELLOADER_HPP
#define OGL_E1_LEVELLOADER_HPP

#include "Scene.hpp"
#include "json.hpp"
#include <fstream>
#include <string>
#include <array>

#include "Utils/factory.hpp"

namespace OGL::E1 {

    class LevelLoader {
     public:
        void load(
            int screenWidth,
            int screenHeight,
            std::string const &filePath,
            modelsTable &modelsTable,
            maybeScene &scene
        );

     protected:

         modelsTable getModelsTable(
             nlohmann::json const &levelJSON
         ) const;

         smartCamPtr getCamera(
             nlohmann::json const &levelJSON,
             float aspect
         ) const;

         Object getNormalObj(
             nlohmann::json const &objJSON,
             modelsTable &modelsTable
         ) const;

         objectsVec getNormalObjs(
             nlohmann::json const &levelJSON,
             modelsTable &modelsTable
         ) const;

         Object getTransparentObj(
             nlohmann::json const &objJSON,
             modelsTable &modelsTable
         ) const;

         objectsVec getTransparentObjs(
             nlohmann::json const &levelJSON,
             modelsTable &modelsTable
         ) const;

         std::pair<Object, maybeCubemap> getMirrorObj(
             nlohmann::json const &objJSON,
             modelsTable &modelsTable
         ) const;

         mirrorObjsVec getMirrorObjs(
             nlohmann::json const &levelJSON,
             modelsTable &modelsTable
         ) const;

         std::pair<Object, uint32_t> getInstancedObj(
             nlohmann::json const &objJSON,
             modelsTable &modelsTable
         ) const;

         instancedObjsVec getInstancedObjs(
             nlohmann::json const &levelJSON,
             modelsTable &modelsTable
         ) const;

         DirectionalLight getDirLight(
             nlohmann::json const &lightJSON
         ) const;

         dirLights getDirLights(
             nlohmann::json const &levelJSON
         ) const;

         PointLight getPointLight(
             nlohmann::json const &lightJSON
         ) const;

         pointLights getPointLights(
             nlohmann::json const &levelJSON
         ) const;

         SpotLight getSpotLight(
             nlohmann::json const &lightJSON
         ) const;

         spotLights getSpotLights(
             nlohmann::json const &levelJSON
         ) const;

         maybeSkybox getSkybox(
             nlohmann::json const &levelJSON
         ) const;
         
    };

} // OGL::E1

#endif // OGL_E1_LEVELLOADER_HPP
