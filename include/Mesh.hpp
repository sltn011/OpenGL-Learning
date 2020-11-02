#ifndef OGL_MESH_H
#define OGL_MESH_H

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "Shader.hpp"

#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <sstream>

namespace OGL {

    struct Vertex {
        glm::vec3 pos;
        glm::vec3 norm;
        glm::vec2 tex;
    };

    enum TextureType {
        Diffuse,
        Specular,
        Normal,
        Height
    };

    struct Texture {
        unsigned int id;
        TextureType  type;
        std::string  path;
    };

    struct Colors {
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float     specularExponent;
    };

    class Mesh {
        unsigned int mVAO;
        unsigned int mVBO;
        unsigned int mEBO;

        std::vector<Vertex>       m_vertices;
        std::vector<unsigned int> m_indices;
        std::vector<Texture>      m_textures;
        Colors                    m_colors;
        

        void setup
        (
        );

    public:
        
        Mesh
        ( std::vector<Vertex> vertices
        , std::vector<unsigned int> indices
        , std::vector<Texture> textures
        , Colors color
        );

        void draw
        ( OGL::Shader &shader
        );

    };

}

#endif // OGL_MESH_H