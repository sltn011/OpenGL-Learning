#include "Model.hpp"

namespace OGL {

    void Model::loadModel( 
        std::string const &path,
        bool bFlipTexturesHorizontally
    ) {
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

        tinyobj::ObjReaderConfig config;
        config.vertex_color = false;

        tinyobj::ObjReader reader;
        if (!reader.ParseFromFile(path, config)) {
            std::cerr << "Error parsing .obj file with path" << path << "!" << std::endl;
            std::cerr << reader.Error() << std::endl;
        }

        if (!reader.Warning().empty()) {
            std::cerr << "Warnings parsing .obj file with path" << path << ":" << std::endl;
            std::cerr << reader.Warning() << std::endl;
        }

        tinyobj::attrib_t const &attrib = reader.GetAttrib();
        std::vector<tinyobj::shape_t> const &shapes = reader.GetShapes(); 
        std::vector<tinyobj::material_t> const &materials = reader.GetMaterials();

        int maxMaterialID = 0;
        for (size_t i = 0; i < shapes.size(); ++i) {
            for (size_t j = 0; j < shapes[i].mesh.material_ids.size(); ++j) {
                if (maxMaterialID < shapes[i].mesh.material_ids[j]) {
                    maxMaterialID = shapes[i].mesh.material_ids[j];
                }
            }
        }

        for (size_t s = 0; s < shapes.size(); ++s) {
            for (int matID = 0; matID <= maxMaterialID; ++matID) {
                Mesh mesh = processMesh(shapes[s].mesh, attrib, materials, matID);
                if (!mesh.isEmpty()) {
                    m_meshes.push_back(std::move(mesh));
                }
            }
        }
    }

    Mesh Model::processMesh(
        tinyobj::mesh_t const &mesh,
        tinyobj::attrib_t const &attrib,
        std::vector<tinyobj::material_t> const &materials,
        int materialID
    ) {
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;
        std::vector<ModelTexture> textures;
        Material                  material{};

        std::unordered_map<Vertex, int> vertexIndex;

        Vertex vertex[3];
        size_t offset = 0;

        for (size_t face = 0; face < mesh.num_face_vertices.size(); ++face) {

            if (mesh.material_ids[face] != materialID) {
                offset += 3;
                continue;
            }

            for (size_t v = 0; v < 3; ++v) {
                tinyobj::index_t index = mesh.indices[offset + v];


                vertex[v].m_pos.x = static_cast<float>(attrib.vertices[3 * index.vertex_index + 0]);
                vertex[v].m_pos.y = static_cast<float>(attrib.vertices[3 * index.vertex_index + 1]);
                vertex[v].m_pos.z = static_cast<float>(attrib.vertices[3 * index.vertex_index + 2]);

                if (index.normal_index >= 0) {
                    vertex[v].m_norm.x = static_cast<float>(attrib.normals[3 * index.normal_index + 0]);
                    vertex[v].m_norm.y = static_cast<float>(attrib.normals[3 * index.normal_index + 1]);
                    vertex[v].m_norm.z = static_cast<float>(attrib.normals[3 * index.normal_index + 2]);
                }

                if (index.texcoord_index >= 0) {
                    vertex[v].m_tex.x = static_cast<float>(attrib.texcoords[2 * index.texcoord_index + 0]);
                    vertex[v].m_tex.y = static_cast<float>(attrib.texcoords[2 * index.texcoord_index + 1]);
                }
            }

            glm::vec3 tangent = CalculateTangent(vertex[0], vertex[1], vertex[2]);
            for (int v = 0; v < 3; ++v) {
                vertex[v].m_tangent = tangent;

                if (vertexIndex.count(vertex[v]) == 0) {
                    indices.push_back(static_cast<unsigned int>(vertexIndex.size()));

                    vertices.push_back(vertex[v]);
                    vertexIndex.emplace(vertex[v], indices.back());
                }
                else {
                    indices.push_back(vertexIndex[vertex[v]]);
                }
            }

            offset += 3;
        }

        tinyobj::material_t const &materialData = materials[materialID];

        std::vector<ModelTexture> diffuseMap = loadMaterialTexture(materialData, TextureType::Diffuse);
        textures.insert(textures.end(), diffuseMap.begin(), diffuseMap.end());

        std::vector<ModelTexture> specularMap = loadMaterialTexture(materialData, TextureType::Specular);
        textures.insert(textures.end(), specularMap.begin(), specularMap.end());

        std::vector<ModelTexture> normalMap = loadMaterialTexture(materialData, TextureType::Normal);
        textures.insert(textures.end(), normalMap.begin(), normalMap.end());

        std::vector<ModelTexture> heightMap = loadMaterialTexture(materialData, TextureType::Height);
        textures.insert(textures.end(), heightMap.begin(), heightMap.end());

        material = loadMaterialParams(materialData);

        return Mesh{ vertices, indices, textures, material };
    }

    std::vector<ModelTexture> Model::loadMaterialTexture(
        tinyobj::material_t const &materialData,
        TextureType typeName
    ) {
        std::vector<ModelTexture> textureVec;

        std::string texturePath;
        switch (typeName) {
        case OGL::TextureType::Diffuse:
            texturePath = materialData.diffuse_texname;
            break;
        case OGL::TextureType::Specular:
            texturePath = materialData.specular_texname;
            break;
        case OGL::TextureType::Normal:
            texturePath = materialData.normal_texname;
            break;
        case OGL::TextureType::Height:
            texturePath = materialData.bump_texname;
            break;
        default:
            break;
        }

        if (!texturePath.empty()) {

            if (m_directory.empty()) {
                texturePath = "./" + texturePath;
            }
            else {
                texturePath = m_directory + "/" + texturePath;
            }

            ModelTexture texture;
            texture.m_id = textureFromFile(texturePath);
            texture.m_path = texturePath;
            texture.m_type = typeName;

            textureVec.push_back(texture);
        }

        return textureVec;
    }

    Material Model::loadMaterialParams(
        tinyobj::material_t const &materialData
    ) {
        Material material;

        for (int i = 0; i < 3; ++i) {
            material.m_ambient[i] = static_cast<float>(materialData.ambient[i]);
            material.m_diffuse[i] = static_cast<float>(materialData.diffuse[i]);
            material.m_specular[i] = static_cast<float>(materialData.specular[i]);
        }
        material.m_specularExponent = static_cast<float>(1.0f);

        return material;
    }

    int Model::textureFromFile(
        std::string const &directory,
        std::string const &path
    ) {
        std::string filename = directory + '/' + std::string{ path };

        return textureFromFile(filename);
    }

    int Model::textureFromFile(
        std::string const &filename
    ) {

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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);

        if (GL_EXT_texture_filter_anisotropic) {
            GLfloat anisoSetting = 1.0f;
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &anisoSetting);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting);
        }

        stbi_image_free(data);

        return texID;
    }

    glm::vec3 Model::CalculateTangent(
        Vertex const &v1,
        Vertex const &v2,
        Vertex const &v3
    ) const {

        glm::vec3 tangent;

        glm::vec3 edge1 = v2.m_pos - v1.m_pos;
        glm::vec3 edge2 = v3.m_pos - v1.m_pos;
        glm::vec2 deltaUV1 = v2.m_tex - v1.m_tex;
        glm::vec2 deltaUV2 = v3.m_tex - v1.m_tex;

        float f = 1.0 / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        return glm::normalize(tangent);
    }

    Model::Model( 
        std::string const &path,
        bool bFlipTexturesHorizontally
    ) {
        loadModel(path, bFlipTexturesHorizontally);
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