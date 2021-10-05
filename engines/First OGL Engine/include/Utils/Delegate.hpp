/**
* @file Delegate.hpp
*/

#ifndef OGL_E1_DELEGATE
#define OGL_E1_DELEGATE

#include <memory>

namespace OGL::E1 {

    /**
     * @brief Delegate class to call member function of objects
    */
    template<typename... Args>
    class Delegate {

    public:

        Delegate(
        ) = default;

        template<typename T, typename F>
        Delegate(
            T *Object,
            F Method
        ) {
            bind(Object, Method);
        }

        template<typename T, typename F>
        void bind(
            T *Object,
            F Method
        ) {
            m_invoker = std::make_unique<Invoker<T, F, Args...>>(Object, Method);
        }

        void operator()(
            Args... args
            ) const {
            if (m_invoker) {
                m_invoker->invoke(std::forward<Args>(args)...);
            }
        }

    protected:

        template<typename... Args>
        class BaseInvoker {
        public:
            virtual void invoke(
                Args... args
            ) const = 0;
        };

        template<typename T, typename F, typename... Args>
        struct Invoker : public BaseInvoker<Args...> {
        public:
            Invoker(
                T *Object,
                F Method
            ) {
                m_object = Object;
                m_method = Method;
            }

            virtual void invoke(
                Args... args
            ) const override {
                if (m_object && m_method) {
                    (m_object->*m_method)(std::forward<Args>(args)...);
                }
            }

        private:
            T *m_object;
            F  m_method;
        };

        std::unique_ptr<BaseInvoker<Args...>> m_invoker;

    };

}

#endif // OGL_E1_DELEGATE
