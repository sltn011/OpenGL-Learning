#ifndef OGL_E1FACTORY_H
#define OGL_E1FACTORY_H

namespace OGL::E1 {

    template<typename T, typename... Args>
    std::unique_ptr<T> factory(
        Args&&... args
    ) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

} // OGL::E1

#endif // OGL_E1FACTORY_H