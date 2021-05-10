/**
* @file Descriptor.hpp
*/

#ifndef OGL_DESCRIPTOR_H
#define OGL_DESCRIPTOR_H

#include <utility>

namespace OGL {

    /**
     * @brief Descriptor class that should be used to implement RAII on move-only integer values(for OpenGL data types)
    */
    class Descriptor {
     protected:
        /// Descriptor value
        unsigned int m_descriptor;

     public:

        /**
         * @brief Descriptor constructor
        */
        Descriptor(
        );

        /**
         * @brief Descriptor destructor
        */
        ~Descriptor(
        );

        Descriptor( 
            Descriptor const &rhs
        ) = delete;

        Descriptor &operator=( 
            Descriptor const &rhs
        ) = delete;

        /**
         * @brief Descriptor move constructor
         * @param rhs Moved value
        */
        Descriptor( 
            Descriptor &&rhs
        ) noexcept;

        /**
         * @brief Descriptor move assignment
         * @param rhs Moved value
         * @return Reference to *this
        */
        Descriptor &operator=( 
            Descriptor &&rhs
        ) noexcept;

        /**
         * @brief Getter for Descriptor value
         * @return Descriptor value
        */
        unsigned int value(
        ) const;

    };

} // OGL

#endif // OGL_DESCRIPTOR_H
