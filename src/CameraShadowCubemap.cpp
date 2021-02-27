#include "CameraShadowCubemap.hpp"

namespace OGL {

    CameraShadowCubemap::CameraShadowCubemap(
        PointLight light, 
        float near, 
        float far
    ) : CameraCubemap{
        light.m_position,
        near,
        far } {
    }

} // OGL