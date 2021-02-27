#ifndef OGL_CAMERASHADOWCUBEMAP_H
#define OGL_CAMERASHADOWCUBEMAP_H

#include "CameraCubemap.hpp"
#include "PointLight.hpp"

namespace OGL {

    class CameraShadowCubemap : public CameraCubemap {
     public:
        CameraShadowCubemap(
            PointLight light,
            float near,
            float far
        );
    };

} // OGL

#endif // OGL_CAMERASHADOWCUBEMAP_H
