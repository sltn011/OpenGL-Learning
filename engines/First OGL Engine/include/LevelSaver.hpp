#ifndef OGL_E1_LEVELSAVER_HPP
#define OGL_E1_LEVELSAVER_HPP

#include "Scene.hpp"
#include "json.hpp"
#include <fstream>
#include <string>
#include <iomanip>

namespace OGL::E1 {

    class LevelSaver {
     public:
        void save(
            std::string const &filePath,
            modelsTable const &modelsTable,
            Scene &scene
        );

     protected:
         nlohmann::json modelsTableToJSON(
             modelsTable const &modelsTable
         );

         nlohmann::json cameraToJSON(
             BasicCamera const *camera
         ) const;

         nlohmann::json objectToJSON(
             Object const &obj,
             std::unordered_map<std::string, uint32_t> const &pathToIDTable
         ) const;

         nlohmann::json objectToJSON(
             Object const &obj,
             uint32_t numInstances,
             std::unordered_map<std::string, uint32_t> const &pathToIDTable
         ) const;

         nlohmann::json objectsToJSON(
             objectsVec const &objs,
             std::unordered_map<std::string, uint32_t> const &pathToIDTable
         ) const;

         nlohmann::json objectsToJSON(
             mirrorObjsVec const &objs,
             std::unordered_map<std::string, uint32_t> const &pathToIDTable
         ) const;

         nlohmann::json objectsToJSON(
             instancedObjsVec const &objs,
             std::unordered_map<std::string, uint32_t> const &pathToIDTable
         ) const;

         nlohmann::json lightToJSON(
             DirectionalLight const &dirLight
         ) const;

         nlohmann::json lightsToJSON(
             dirLights const &lights
         ) const;

         nlohmann::json lightToJSON(
             PointLight const &pointLight
         ) const;

         nlohmann::json lightsToJSON(
             pointLights const &lights
         ) const;

         nlohmann::json lightToJSON(
             SpotLight const &spotLight
         ) const;

         nlohmann::json lightsToJSON(
             spotLights const &lights
         ) const;

         nlohmann::json skyboxToJSON(
             Skybox const &skybox
         ) const;

         std::unordered_map<std::string, uint32_t> reverseModelsTable(
             modelsTable const &table
         ) const;
    };

} // OGL::E1

#endif // OGL_E1_LEVELSAVER_HPP
