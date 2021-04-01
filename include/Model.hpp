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
    
    class Model {
        std::vector<Mesh>         m_meshes;
        std::string               m_directory;
        std::string               m_name;
        std::string               m_format;
        std::vector<ModelTexture> m_loadedTextures;

        void loadModel( 
            std::string const &path, 
            int flags = aiProcess_Triangulate | aiProcess_FlipUVs
        );

        void processNode( 
            aiNode *node, 
            aiScene const *scene
        );

        Mesh processMesh( 
            aiMesh *mesh, 
            aiScene const *scene
        );

        std::vector<ModelTexture> loadMaterialTexture( 
            aiMaterial *material, 
            aiTextureType texType, 
            TextureType typeName
        );

        static int textureFromFile(
            std::string const &path,
            std::string const &directory,
            bool gamma = false
        );

     public:
        Model(
        ) = default;

        Model( 
            char const *path, 
            int flags = aiProcess_Triangulate | aiProcess_FlipUVs
        );

        void draw( 
            Shader &shader
        ) const;

        void drawInstanced( 
            Shader &shader, 
            size_t amount
        ) const;

        void drawShape(
            Shader &shader
        ) const;

        void drawShapeInstanced(
            Shader &shader,
            size_t amount
        ) const;

        void setVertexAttribInstancedModelMat4( 
            int attribLocation
        );

        std::string getDirectory(
        ) const;

        std::string getName(
        ) const;

        std::string getFormat(
        ) const;

        std::string getFullPath(
        ) const;

    };

} // OGL

#endif // OGL_MODEL_H