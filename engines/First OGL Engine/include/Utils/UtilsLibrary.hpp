#ifndef OGL_E1_UTILSLIBRARY
#define OGL_E1_UTILSLIBRARY


namespace OGL::E1 {

    class UtilsLibrary {

        friend class Engine1Base;

        static inline Engine1Base *m_engine = nullptr;

    public:

        static Engine1Base *getEngineInstance(
        ) {
            return m_engine;
        }

    };

} // OGL::E1

#endif // OGL_E1_UTILSLIBRARY
