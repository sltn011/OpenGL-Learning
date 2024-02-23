#ifndef OGL_PLANE_HPP
#define OGL_PLANE_HPP

#include "Model.hpp"

namespace OGL {

    class Plane : public Model {
    public:

        Plane(
            float side
        );

        Plane(
            float a,
            float b
        );

        Plane(
            float a,
            float b,
            int res
        );

    protected:

        void generateMesh(
        );

        float m_sideA;
        float m_sideB;
        int m_res;
    };

} // OGL

#endif // OGL_PLANE_HPP
