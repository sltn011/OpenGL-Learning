#ifndef OGL_SPHERE_HPP
#define OGL_SPHERE_HPP

#include "Model.hpp"

namespace OGL {

    class Sphere : public Model {
    public:

        Sphere(
            float radius,
            int sectors,
            int stacks
        );

    protected:

        void generateMesh(
        );

        float m_radius;
        int m_sectors;
        int m_stacks;

    };

} // OGL

#endif // OGL_SPHERE_HPP
