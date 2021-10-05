/**
* @file PostprocessingData.hpp
*/

#ifndef OGL_E1_POSTPROCESSINGDATA
#define OGL_E1_POSTPROCESSINGDATA

namespace OGL::E1 {

    struct PostprocessingData {
        bool bEnablePostprocessing = false;
        bool bEnableHDR = false;
        float HDRExposure = 1.0f;
    };

} // OGL::E1

#endif // OGL_E1_POSTPROCESSINGDATA
