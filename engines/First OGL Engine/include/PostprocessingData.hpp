/**
* @file PostprocessingData.hpp
*/

#ifndef OGL_E1_POSTPROCESSINGDATA
#define OGL_E1_POSTPROCESSINGDATA

namespace OGL::E1 {

    struct PostprocessingData {
        bool bEnablePostprocessing = true;
        bool bEnableHDR = true;
        float HDRExposure = 1.0f;

        bool bEnableBloom = true;

        bool bEnableSSAO = true;
    };

} // OGL::E1

#endif // OGL_E1_POSTPROCESSINGDATA
