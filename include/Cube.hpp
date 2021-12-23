#ifndef OGL_CUBE_HPP
#define OGL_CUBE_HPP

#include "Model.hpp"

namespace OGL {

    class Cube : public Model {
    public:

        Cube(
            float side
        );

        Cube(
            float a,
            float b,
            float c
        );

    protected:

        void generateMesh(
        );

        float m_sideA;
        float m_sideB;
        float m_sideC;
    };

} // OGL

#endif // OGL_CUBE_HPP
