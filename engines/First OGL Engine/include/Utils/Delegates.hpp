/**
* @file Delegates.hpp
*/

#ifndef OGL_E1_DELEGATES
#define OGL_E1_DELEGATES

#include <memory>

namespace OGL::E1 {

    /**
     * @brief Delegate class to call member function of objects
    */
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
            m_invoker = std::make_unique<Invoker<T, F>>(Object, Method);
        }

        void operator()(
        ) const {
            if (m_invoker) {
                m_invoker->invoke();
            }
        }

     protected:

         class BaseInvoker {
         public:
             virtual void invoke(
             ) const = 0;
         };

         template<typename T, typename F>
         struct Invoker : public BaseInvoker {
         public:
             Invoker(
                 T *Object,
                 F Method
             ) {
                 m_object = Object;
                 m_method = Method;
             }

             virtual void invoke(
             ) const override {
                 if (m_object && m_method) {
                     (m_object->*m_method)();
                 }
             }

         private:
             T *m_object;
             F  m_method;
         };

        std::unique_ptr<BaseInvoker> m_invoker;

    };

} // OGL::E1

#endif // OGL_E1_DELEGATES
