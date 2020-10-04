#include "Exception.hpp"

namespace OGL {

    Exception::Exception
    ( std::string const &info
    ) : m_info{info} {
    }

    Exception::Exception
    ( std::string &&info
    ) : m_info{std::move(info)} {
    }

    const char *Exception::what
    (
    ) const noexcept {
        return m_info.c_str();
    }

} // OGL