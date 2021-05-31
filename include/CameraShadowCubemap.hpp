/**
* @file CameraShadowCubemap.hpp
*/

#ifndef OGL_CAMERASHADOWCUBEMAP_H
#define OGL_CAMERASHADOWCUBEMAP_H

#include "CameraCubemap.hpp"
#include "PointLight.hpp"

namespace OGL {

    /**
     * @brief Camera class to render shadow cubemap produced by PointLight
    */
    class CameraShadowCubemap : public CameraCubemap {
     public:

        /**
         * @brief CameraShadowCubemap constructor
         * @param light PointLight for which shadow cubemap will be rendered
         * @param near Near clip plane
         * @param far Far clip plane
        */
        CameraShadowCubemap(
            PointLight light,
            float near,
            float far
        );
    };

} // OGL

#endif // OGL_CAMERASHADOWCUBEMAP_H
