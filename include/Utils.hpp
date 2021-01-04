#ifndef OGL_UTILS_H
#define OGL_UTILS_H

#include "glad/glad.h"
#include "Exception.hpp"

#include "stb_image.h"

#include <string>

namespace OGL {

    unsigned int textureFromFile
    ( char const *path
    , std::string const &directory
    , bool gamma = false
    );

} // OGL

#endif // OGL_UTILS_H