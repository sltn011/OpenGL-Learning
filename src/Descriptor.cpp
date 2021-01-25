#include "Descriptor.hpp"

namespace OGL {

    Descriptor::Descriptor(
    ) : m_descriptor{0} {
    }

    Descriptor::~Descriptor(
    ) {
    }

    Descriptor::Descriptor( 
        Descriptor &&rhs
    ) : m_descriptor{rhs.m_descriptor} {
        rhs.m_descriptor = 0;
    }

    Descriptor &Descriptor::operator=( 
        Descriptor &&rhs
    ) {
        unsigned int buf = m_descriptor;
        m_descriptor = rhs.m_descriptor;
        rhs.m_descriptor = buf;
        return *this;
    }

    unsigned int Descriptor::value(
    ) const {
        return m_descriptor;
    }

} // OGL