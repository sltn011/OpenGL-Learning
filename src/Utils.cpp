#include "Utils.hpp"


unsigned int OGL::textureFromFile( 
    char const *path, 
    std::string const &directory,
    bool gamma
) {
    std::string filename = directory + '/' + std::string{path};

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
