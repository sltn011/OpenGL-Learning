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
    ) noexcept : m_descriptor{std::exchange(rhs.m_descriptor, 0)} {
        
    }

    Descriptor &Descriptor::operator=( 
        Descriptor &&rhs
    ) noexcept {
        std::swap(m_descriptor, rhs.m_descriptor);
        return *this;
    }

    unsigned int Descriptor::value(
    ) const {
        return m_descriptor;
    }

} // OGL