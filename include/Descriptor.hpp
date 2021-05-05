#ifndef OGL_DESCRIPTOR_H
#define OGL_DESCRIPTOR_H

#include <utility>

namespace OGL {

    class Descriptor {
     protected:
        unsigned int m_descriptor;

     public:
        Descriptor(
        );

        ~Descriptor(
        );

        Descriptor( 
            Descriptor const &rhs
        ) = delete;

        Descriptor &operator=( 
            Descriptor const &rhs
        ) = delete;

        Descriptor( 
            Descriptor &&rhs
        ) noexcept;

        Descriptor &operator=( 
            Descriptor &&rhs
        ) noexcept;

        unsigned int value(
        ) const;

    };

} // OGL

#endif // OGL_DESCRIPTOR_H
