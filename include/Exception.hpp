#ifndef OGL_EXCEPTION_H
#define OGL_EXCEPTION_H

#include <exception>
#include <string>

namespace OGL {

    class Exception : public std::exception {
        std::string m_info;

     public:
        Exception(
            std::string const &info
        );

        Exception(
            std::string &&info
        );

        const char *what(
        ) const noexcept override;
    };

}

#endif // OGL_EXCEPTION_H