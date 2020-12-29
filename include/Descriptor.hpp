#ifndef OGL_DESCRIPTOR_H
#define OGL_DESCRIPTOR_H

namespace OGL {

    class Descriptor {
    protected:
        unsigned int m_descriptor;

    public:
        Descriptor
        (
        );

        ~Descriptor
        (
        );

        Descriptor
        ( Descriptor const &rhs
        ) = delete;

        Descriptor &operator=
        ( Descriptor const &rhs
        ) = delete;

        Descriptor
        ( Descriptor &&rhs
        );

        Descriptor &operator=
        ( Descriptor &&rhs
        );

        unsigned int value
        (
        ) const;

    };

} // OGL

#endif // OGL_DESCRIPTOR_H
