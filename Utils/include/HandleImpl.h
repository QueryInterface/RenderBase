#pragma once
#include <cstdint>

//template <class ClassName>
//ClassName* CreateHandleObject()
//{
//    return new HandleImpl<ClassName>();
//}

#define VA_NUM_ARGS(...) VA_NUM_ARGS_IMPL(__VA_ARGS__, 6,5,4,3,2,1,0)
#define VA_NUM_ARGS_IMPL(_1, _2, _3, _4, _5, _6, _N, _N_1, _N_2, _N_3, _N_4, _N_5, N_6) _1, _2, _3, _4, _5, _6, _N

VA_NUM_ARGS(x,y,z)

VA_NUM_ARGS(x,y,z, t, 2)

#define VARIADIC_EXPAND(CONSTRUCTOR) \
    CONSTRUCTOR() \    
    CONSTRUCTOR(T0) \
    CONSTRUCTOR(T0, T1) \
    CONSTRUCTOR(T0, T1, T2) \
    CONSTRUCTOR(T0, T1, T2, T3) \
    CONSTRUCTOR(T0, T1, T2, T3, T4) \
    CONSTRUCTOR(T0, T1, T2, T3, T4, T5) \

#define MACRO_DISPATCHER(func, ...) \
            MACRO_DISPATCHER_(func, VA_NUM_ARGS(__VA_ARGS__))

#define MACRO_DISPATCHER_(func, nargs) \
        MACRO_DISPATCHER__(func, nargs)

#define MACRO_DISPATCHER__(func, nargs) \
        func ## nargs


#define VARIADIC_TEMPLATE(...) MACRO_DISPATCHER(VARIADIC_TEMPLATE, __VA_ARGS__)(__VA_ARGS__)
#define VARIADIC_VALUES_LIST(...) MACRO_DISPATCHER(VARIADIC_VALUES_LIST, __VA_ARGS__)(__VA_ARGS__)
#define VARIADIC_VALUES(...) MACRO_DISPATCHER(VARIADIC_VALUES, __VA_ARGS__)(__VA_ARGS__)

#define VARIADIC_TEMPLATE0()
#define VARIADIC_TEMPLATE1(T0) class T0
#define VARIADIC_TEMPLATE2(T0, T1) class T0, class T1
#define VARIADIC_TEMPLATE3(T0, T1, T2) class T0, class T1, class T2
#define VARIADIC_TEMPLATE4(T0, T1, T2, T3) class T0, class T1, class T2, class T3
#define VARIADIC_TEMPLATE5(T0, T1, T2, T3, T4) class T0, class T1, class T2, class T3, class T4
#define VARIADIC_TEMPLATE6(T0, T1, T2, T3, T4, T5) class T0, class T1, class T2, class T3, class T4, class T5

#define VARIADIC_VALUES_LIST0()
#define VARIADIC_VALUES_LIST1(T0) T0 v0
#define VARIADIC_VALUES_LIST2(T0, T1) T0 v0, T1 v1
#define VARIADIC_VALUES_LIST3(T0, T1, T2) T0 v0, T1 v1, T2 v2
#define VARIADIC_VALUES_LIST4(T0, T1, T2, T3) T0 v0, T1 v1, T2 v2, T3 v3
#define VARIADIC_VALUES_LIST5(T0, T1, T2, T3, T4) T0 v0, T1 v1, T2 v2, T3 v3, T4 v4
#define VARIADIC_VALUES_LIST6(T0, T1, T2, T3, T4, T5) T0 v0, T1 v1, T2 v2, T3 v3, T4 v4, T5 v5

#define VARIADIC_VALUES0()
#define VARIADIC_VALUES1(T0) v0
#define VARIADIC_VALUES2(T0, T1) v0, v1
#define VARIADIC_VALUES3(T0, T1, T2) v0, v1, v2
#define VARIADIC_VALUES4(T0, T1, T2, T3) v0, v1, v2, v3
#define VARIADIC_VALUES5(T0, T1, T2, T3, T4) v0, v1, v2, v3, v4
#define VARIADIC_VALUES6(T0, T1, T2, T3, T4, T5) v0, v1, v2, v3, v4, v5


#define CREATE_HANDLE_OBJECT(...)                                                   \
    template <class ClassName, VARIADIC_TEMPLATE(__VA_ARGS__)>                      \
    class HandleImpl : public ClassName                                             \
    {                                                                               \
    public:                                                                         \
        HandleImpl(VARIADIC_VALUES_LIST(__VA_ARGS__))                               \
            : m_refCount(1)                                                         \
            , ClassName(VARIADIC_VALUES(__VA_ARGS__))                               \
        {                                                                           \
        }                                                                           \
        virtual uint32_t AddRef()                                                   \
        {                                                                           \
            return ++m_refCount;                                                    \
        }                                                                           \
        virtual uint32_t Release()                                                  \
        {                                                                           \
            if (m_refCount > 0)                                                     \
            {                                                                       \
                --m_refCount;                                                       \
                if (!m_refCount) delete this;                                       \
            }                                                                       \
        }                                                                           \
    private:                                                                        \
        uint32_t m_refCount;                                                        \
    };                                                                              \
                                                                                    \
    template<class ClassName, VARIADIC_TEMPLATE(__VA_ARGS__)>                       \
    ClassName* CreateHandleObject(VARIADIC_VALUES_LIST(__VA_ARGS__)                 \
    {                                                                               \
        return new HandleImpl<ClassName, __VA_ARGS__>(VARIADIC_VALUES(__VA_ARGS__));\
    }

VARIADIC_EXPAND(CREATE_HANDLE_OBJECT)
#undef CREATE_HANDLE_OBJECT