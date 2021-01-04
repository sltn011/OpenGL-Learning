#ifndef OGL_MODEL_H
#define OGL_MODEL_H

#include "Mesh.hpp"
#include "Exception.hpp"
#include "Utils.hpp"  

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace OGL {
    
    class Model {
        std::vector<Mesh>         m_meshes;
        std::string               m_directory;
        std::vector<ModelTexture> m_loadedTextures;

        void loadModel
        ( std::string const &path
        , int flags = aiProcess_Triangulate | aiProcess_FlipUVs
        );

        void processNode
        ( aiNode *node
        , aiScene const *scene
        );

        Mesh processMesh
        ( aiMesh *mesh
        , aiScene const *scene
        );

        std::vector<ModelTexture> loadMaterialTexture
        ( aiMaterial *material
        , aiTextureType texType
        , TextureType typeName
        );

    public:
        Model
        (
        ) = default;

        Model
        ( char const *path
        , int flags = aiProcess_Triangulate | aiProcess_FlipUVs
        );

        void draw
        ( Shader &shader
        );
    };

} // OGL

#endif // OGL_MODEL_H