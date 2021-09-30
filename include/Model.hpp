/**
* @file Model.hpp
*/

#ifndef OGL_MODEL_H
#define OGL_MODEL_H

#include "Mesh.hpp"
#include "Exception.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "stb_image.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace OGL {
    
    /**
     * @brief Class used to load models from files and render them
    */
    class Model {
        /// Container of Mesh objects of Model
        std::vector<Mesh>         m_meshes;

        /// Directory of Model's file
        std::string               m_directory;

        /// Name of Model's file
        std::string               m_name;

        /// Format of Model's file
        std::string               m_format;

        /// Container of Model's textures
        std::vector<ModelTexture> m_loadedTextures;

     public:
        /**
         * @brief Empty Model default constructor
        */
        Model(
        ) = default;

        /**
         * @brief Constructor of Model loaded from file
         * @param path Path to Model file
         * @param bFlipTexturesHorizontally True if textures should be flipped horizontally
         * @param flags Assimp library flags to process Model - triangulate and calculate tangents by default
        */
        explicit Model( 
            std::string const &path, 
            bool bFlipTexturesHorizontally = true,
            int flags = aiProcess_Triangulate | aiProcess_CalcTangentSpace
        );

        /**
         * @brief Renders every Mesh of a Model using supplied Shader
         * @param shader Currently active Shader used to render Model
        */
        void draw( 
            Shader &shader
        ) const;

        /**
         * @brief Instanced render of every Mesh of a Model amount times using supplied Shader
         * @param shader Currently active Shader used to render Model
         * @param amount Number of instances
        */
        void drawInstanced( 
            Shader &shader, 
            uint32_t amount
        ) const;

        /**
         * @brief Renders every Mesh of a Model using supplied Shader without loading textures and materials into Shader
         * @param shader Currently active Shader used to render Model
        */
        void drawShape(
            Shader &shader
        ) const;

        /**
         * @brief Instanced render of every Mesh of a Model amount times using supplied Shader without loading textures and materials into Shader
         * @param shader Currently active Shader used to render Model
         * @param amount Number of instances
        */
        void drawShapeInstanced(
            Shader &shader,
            uint32_t amount
        ) const;

        /**
         * @brief Enables vertex attribute for every Model's Mesh to pass Mat4x4 data into vertex Shader
         * @param attribLocation First location index used by Mat4x4 data - indexes from attribLocation to attribLocation + 3 will be occupied
        */
        void setVertexAttribInstancedModelMat4( 
            int attribLocation
        );

        /**
         * @brief Get Model file directory
         * @return Model file directory
        */
        std::string getDirectory(
        ) const;

        /**
         * @brief Get Model file name
         * @return Model file name
        */
        std::string getName(
        ) const;

        /**
         * @brief Get Model file format
         * @return Model file format
        */
        std::string getFormat(
        ) const;

        /**
         * @brief Get Model file "path/name.format"
         * @return Model file "path/name.format"
        */
        std::string getFullPath(
        ) const;

     private:

         /**
          * @brief Loads Model from file
          * @param path Path to Model file
          * @param bFlipTexturesHorizontally True if textures should be flipped horizontally
          * @param flags Assimp library flags to process Model - triangulate and calculate tangents by default
         */
         void loadModel(
             std::string const &path,
             bool bFlipTexturesHorizontally = true,
             int flags = aiProcess_Triangulate | aiProcess_CalcTangentSpace
         );

         /**
          * @brief Processes data loaded by Assimp and fills Model data
          * @param node Assimp node pointer
          * @param scene Assimp scene pointer
         */
         void processNode(
             aiNode * node,
             aiScene const *scene
         );

         /**
          * @brief Processes data loaded by Assimp to create Mesh object
          * @param mesh Assimp mesh structure pointer
          * @param scene Assimp scene pointer
          * @return Mesh object created from data in Assimp mesh structure
         */
         Mesh processMesh(
             aiMesh * mesh,
             aiScene const *scene
         );

         /**
          * @brief Loads textures used in Model's material
          * @param material Assimp material struct pointer
          * @param texType Assimp type of loaded textures
          * @param typeName Type of loaded texture used in Model
          * @return
         */
         std::vector<ModelTexture> loadMaterialTexture(
             aiMaterial * material,
             aiTextureType texType,
             TextureType typeName
         );

         /**
          * @brief Loads texture from file and creates OpenGL texture id for it
          * @param directory Directory with texture file
          * @param path File name
          * @return ID of OpenGL texture
         */
         static int textureFromFile(
             std::string const &directory,
             std::string const &path
         );

    };

} // OGL

#endif // OGL_MODEL_H