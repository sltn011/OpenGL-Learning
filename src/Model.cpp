#include "Model.hpp"

namespace OGL {

    void Model::loadModel
    ( std::string const &path
    , int flags
    ) {
        Assimp::Importer importer;
        aiScene const *scene = importer.ReadFile(path, flags);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            throw Exception("Error importing model!:\n" + std::string{importer.GetErrorString()});
        }

        m_directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);
    }

    void Model::processNode
    ( aiNode *node
    , aiScene const *scene
    ) {
        for (size_t i = 0; i < node->mNumMeshes; ++i) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            m_meshes.push_back(processMesh(mesh, scene));
        }
        
        for (size_t i = 0; i < node->mNumChildren; ++i) {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh Model::processMesh
    ( aiMesh *mesh
    , aiScene const *scene
    ) {
        std::vector<Vertex>       vertices(mesh->mNumVertices);
        std::vector<unsigned int> indices;
        std::vector<ModelTexture>      textures;
        Colors                    colors{};

        for (size_t i = 0; i < mesh->mNumVertices; ++i) {
            vertices[i].pos.x = mesh->mVertices[i].x;
            vertices[i].pos.y = mesh->mVertices[i].y;
            vertices[i].pos.z = mesh->mVertices[i].z;

            vertices[i].norm.x = mesh->mNormals[i].x;
            vertices[i].norm.y = mesh->mNormals[i].y;
            vertices[i].norm.z = mesh->mNormals[i].z;

            if (mesh->mTextureCoords[0]) {
                vertices[i].tex.x = mesh->mTextureCoords[0][i].x;
                vertices[i].tex.y = mesh->mTextureCoords[0][i].y;
            }
            else {
                vertices[i].tex = glm::vec2{0.0f, 0.0f};
            }
        }

        for (size_t i = 0; i < mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            for (size_t j = 0; j < face.mNumIndices; ++j) {
                indices.push_back(face.mIndices[j]);
            }
        }

        if (mesh->mMaterialIndex >= 0) {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

            std::vector<ModelTexture> diffuseMap = loadMaterialTexture(material, aiTextureType_DIFFUSE, TextureType::Diffuse);
            textures.insert(textures.end(), diffuseMap.begin(), diffuseMap.end());

            std::vector<ModelTexture> specularMap = loadMaterialTexture(material, aiTextureType_SPECULAR, TextureType::Specular);
            textures.insert(textures.end(), specularMap.begin(), specularMap.end());

            std::vector<ModelTexture> normalMap = loadMaterialTexture(material, aiTextureType_NORMALS, TextureType::Normal);
            textures.insert(textures.end(), normalMap.begin(), normalMap.end());

            std::vector<ModelTexture> heightMap = loadMaterialTexture(material, aiTextureType_HEIGHT, TextureType::Height);
            textures.insert(textures.end(), heightMap.begin(), heightMap.end());

            aiColor3D amb{};
            material->Get(AI_MATKEY_COLOR_AMBIENT, amb);

            aiColor3D diff{};
            material->Get(AI_MATKEY_COLOR_DIFFUSE, diff);

            aiColor3D spec{};
            material->Get(AI_MATKEY_COLOR_SPECULAR, spec);

            for (int i = 0; i < 3; ++i) {
                colors.ambient[i] = amb[i];
                colors.diffuse[i] = diff[i];
                colors.specular[i] = spec[i];
            }

            material->Get(AI_MATKEY_SHININESS, colors.specularExponent);
        }
        
        return Mesh{ vertices, indices, textures, colors };
    }

    std::vector<ModelTexture> Model::loadMaterialTexture
    ( aiMaterial *material
    , aiTextureType texType
    , TextureType typeName
    ) {
        std::vector<ModelTexture> textures;
        for (size_t i = 0; i < material->GetTextureCount(texType); ++i) {
            aiString str;
            material->GetTexture(texType, i, &str);

            bool alreadyLoaded = false;
            for (size_t i = 0; i < m_loadedTextures.size(); ++i) {
                if (!std::strcmp(m_loadedTextures[i].path.c_str(), str.C_Str())) {
                    textures.push_back(m_loadedTextures[i]);
                    alreadyLoaded = true;
                    break;
                }
            }

            if (!alreadyLoaded) {
                ModelTexture texture;
                texture.id = textureFromFile(str.C_Str(), m_directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                m_loadedTextures.push_back(texture);
            }
        }
        return textures;
    }

    Model::Model
    ( char const *path
    , int flags
    ) {
        loadModel(path, flags);
    }

    void Model::draw
    ( Shader &shader
    ) const {
        for (size_t i = 0; i < m_meshes.size(); ++i) {
            m_meshes[i].draw(shader);
        }
    }

    void Model::drawInstanced
    ( Shader &shader
    , size_t amount
    ) const {
        for (size_t i = 0; i < m_meshes.size(); ++i) {
            m_meshes[i].drawInstanced(shader, amount);
        }
    }

    void Model::setVertexAttribInstancedModelMat4
    ( int attribLocation
    ) {
        for (size_t i = 0; i < m_meshes.size(); ++i) {
            m_meshes[i].setVertexAttribInstancedModelMat4(attribLocation);
        }
    }

} // OGL