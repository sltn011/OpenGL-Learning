#include "Model.hpp"

namespace OGL {

    void Model::loadModel( 
        std::string const &path, 
        int flags
    ) {
        Assimp::Importer importer;
        aiScene const *scene = importer.ReadFile(path, flags);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            throw Exception("Error importing model!:\n" + std::string{importer.GetErrorString()});
        }

        size_t pathFileDivisor = path.find_last_of('/');
        size_t pathFormatDivisor = path.find_last_of('.');

        if (pathFileDivisor == std::string::npos) {
            pathFileDivisor = -1;
        }
        else {
            m_directory = path.substr(0, pathFileDivisor);
        }
        
        m_name = path.substr(pathFileDivisor + 1, pathFormatDivisor - pathFileDivisor - 1);
        m_format = path.substr(pathFormatDivisor + 1, path.size() - pathFormatDivisor);

        processNode(scene->mRootNode, scene);
    }

    void Model::processNode( 
        aiNode *node, 
        aiScene const *scene
    ) {
        for (size_t i = 0; i < node->mNumMeshes; ++i) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            m_meshes.push_back(processMesh(mesh, scene));
        }
        
        for (size_t i = 0; i < node->mNumChildren; ++i) {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh Model::processMesh( 
        aiMesh *mesh,
        aiScene const *scene
    ) {
        std::vector<Vertex>       vertices(mesh->mNumVertices);
        std::vector<unsigned int> indices;
        std::vector<ModelTexture>      textures;
        Colors                    colors{};

        for (size_t i = 0; i < mesh->mNumVertices; ++i) {
            vertices[i].m_pos.x = mesh->mVertices[i].x;
            vertices[i].m_pos.y = mesh->mVertices[i].y;
            vertices[i].m_pos.z = mesh->mVertices[i].z;

            vertices[i].m_norm.x = mesh->mNormals[i].x;
            vertices[i].m_norm.y = mesh->mNormals[i].y;
            vertices[i].m_norm.z = mesh->mNormals[i].z;

            if (mesh->mTextureCoords[0]) {
                vertices[i].m_tex.x = mesh->mTextureCoords[0][i].x;
                vertices[i].m_tex.y = mesh->mTextureCoords[0][i].y;
            }
            else {
                vertices[i].m_tex = glm::vec2{0.0f, 0.0f};
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
                colors.m_ambient[i] = amb[i];
                colors.m_diffuse[i] = diff[i];
                colors.m_specular[i] = spec[i];
            }

            material->Get(AI_MATKEY_SHININESS, colors.m_specularExponent);
        }
        
        return Mesh{ vertices, indices, textures, colors };
    }

    std::vector<ModelTexture> Model::loadMaterialTexture( 
        aiMaterial *material, 
        aiTextureType texType,
        TextureType typeName
    ) {
        std::vector<ModelTexture> textures;
        for (size_t i = 0; i < material->GetTextureCount(texType); ++i) {
            aiString str;
            material->GetTexture(texType, static_cast<unsigned int>(i), &str);

            bool alreadyLoaded = false;
            for (size_t i = 0; i < m_loadedTextures.size(); ++i) {
                if (!std::strcmp(m_loadedTextures[i].m_path.c_str(), str.C_Str())) {
                    textures.push_back(m_loadedTextures[i]);
                    alreadyLoaded = true;
                    break;
                }
            }

            if (!alreadyLoaded) {
                ModelTexture texture;
                texture.m_id = textureFromFile(str.C_Str(), m_directory);
                texture.m_type = typeName;
                texture.m_path = str.C_Str();
                textures.push_back(texture);
                m_loadedTextures.push_back(texture);
            }
        }
        return textures;
    }

    int Model::textureFromFile(
        std::string const &path,
        std::string const &directory,
        bool gamma
    ) {
        std::string filename = directory + '/' + std::string{ path };

        unsigned int texID;
        glGenTextures(1, &texID);

        int width, height, nrComponents;
        unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
        if (!data) {
            throw Exception("Error loading texture!");
        }
        GLenum format;
        switch (nrComponents) {
        case 1:
            format = GL_RED;
            break;

        case 3:
            format = GL_RGB;
            break;

        case 4:
            format = GL_RGBA;
            break;

        default:
            format = 0;
            break;
        }

        glBindTexture(GL_TEXTURE_2D, texID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);

        return texID;
    }

    Model::Model( 
        std::string const &path,
        int flags
    ) {
        loadModel(path, flags);
    }

    void Model::draw( 
        Shader &shader
    ) const {
        for (size_t i = 0; i < m_meshes.size(); ++i) {
            m_meshes[i].draw(shader);
        }
    }

    void Model::drawInstanced( 
        Shader &shader,
        uint32_t amount
    ) const {
        for (size_t i = 0; i < m_meshes.size(); ++i) {
            m_meshes[i].drawInstanced(shader, amount);
        }
    }

    void Model::drawShape(
        Shader &shader
    ) const {
        for (size_t i = 0; i < m_meshes.size(); ++i) {
            m_meshes[i].drawShape(shader);
        }
    }

    void Model::drawShapeInstanced(
        Shader &shader, 
        uint32_t amount
    ) const {
        for (size_t i = 0; i < m_meshes.size(); ++i) {
            m_meshes[i].drawShapeInstanced(shader, amount);
        }
    }

    void Model::setVertexAttribInstancedModelMat4( 
        int attribLocation
    ) {
        for (size_t i = 0; i < m_meshes.size(); ++i) {
            m_meshes[i].setVertexAttribInstancedModelMat4(attribLocation);
        }
    }

    std::string Model::getDirectory(
    ) const {
        return m_directory;
    }

    std::string Model::getName(
    ) const {
        return m_name;
    }

    std::string Model::getFormat(
    ) const {
        return m_format;
    }

    std::string Model::getFullPath(
    ) const {
        return (m_directory.empty() ? "" : getDirectory() + "/") + getName() + "." + getFormat();
    }

} // OGL