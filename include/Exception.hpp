/**
* @file Exception.hpp
*/

#ifndef OGL_EXCEPTION_H
#define OGL_EXCEPTION_H

#include <exception>
#include <string>

namespace OGL {

    /**
     * @brief Class for OGL exceptions
    */
    class Exception : public std::exception {
        /// Exception info
        std::string m_info;

     public:
        /**
         * @brief Exception constructor
         * @param info Exception message
        */
        explicit Exception(
            std::string const &info
        );

        /**
         * @brief Exception constructor
         * @param info Exception message
        */
        explicit Exception(
            std::string &&info
        );

        /**
         * @brief Returns Exception message
         * @return Pointer to null-terminated Exception message
        */
        const char *what(
        ) const noexcept override;
    };

}

#endif // OGL_EXCEPTION_H