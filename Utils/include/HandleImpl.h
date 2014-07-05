#pragma once
#include <cstdint>
//#include <memory>

//template <class ClassName>
//ClassName* CreateHandleObject()
//{
//    return new HandleImpl<ClassName>();
//}

// Due to MSVC compiler issue expand of __VA_ARGS__ functions that accepts it should be 
// wrapped with fake function. Otherwise the whole param list will go to first parameter of nested macro
// MSVC compiler is GUANO!
#define VA_NUM_ARGS(...) VA_NUM_ARGS_(VA_NUM_PREFIX_ ## __VA_ARGS__ ## _VA_NUM_POSTFIX, VA_SEQ)
#define VA_NUM_PREFIX__VA_NUM_POSTFIX ,,,,,,,,,,,,,,,0
#define VA_NUM_ARGS_(...) EXPAND(VA_NUM_ARGS__(__VA_ARGS__))
#define VA_NUM_ARGS__(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, N, ...) N
#define VA_SEQ 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define EXPAND(x) x

#define VARIADIC_EXPAND(CONSTRUCTOR) \
    CONSTRUCTOR() \
    CONSTRUCTOR(T0) \
    CONSTRUCTOR(T0, T1) \
    CONSTRUCTOR(T0, T1, T2) \
    CONSTRUCTOR(T0, T1, T2, T3) \
    CONSTRUCTOR(T0, T1, T2, T3, T4) \
    CONSTRUCTOR(T0, T1, T2, T3, T4, T5)

#define MACRO_DISPATCHER(func, ...) \
            MACRO_DISPATCHER_(func, VA_NUM_ARGS(__VA_ARGS__))

#define MACRO_DISPATCHER_(func, nargs) \
        MACRO_DISPATCHER__(func, nargs)

#define MACRO_DISPATCHER__(func, nargs) \
        func ## nargs


#define VARIADIC_TEMPLATE(...) MACRO_DISPATCHER(VARIADIC_TEMPLATE, __VA_ARGS__)(__VA_ARGS__)
#define VARIADIC_VALUES_LIST(...) MACRO_DISPATCHER(VARIADIC_VALUES_LIST, __VA_ARGS__)(__VA_ARGS__)
#define VARIADIC_VALUES(...) MACRO_DISPATCHER(VARIADIC_VALUES, __VA_ARGS__)(__VA_ARGS__)
#define COMMA(...) MACRO_DISPATCHER(COMMA, __VA_ARGS__)
#define CLASSNAME(name, ...) MACRO_DISPATCHER(name, __VA_ARGS__)

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

#define COMMA0
#define COMMA1 ,
#define COMMA2 ,
#define COMMA3 ,
#define COMMA4 ,
#define COMMA5 ,
#define COMMA6 ,


#define CREATE_HANDLE_OBJECT(...)                                                   \
    template <class ClassName COMMA(__VA_ARGS__) VARIADIC_TEMPLATE(__VA_ARGS__)>    \
    class CLASSNAME(HandleImpl, __VA_ARGS__) : public ClassName                     \
    {                                                                               \
    public:                                                                         \
        CLASSNAME(HandleImpl, __VA_ARGS__)(VARIADIC_VALUES_LIST(__VA_ARGS__))       \
            : m_refCount(1)                                                         \
            , ClassName(VARIADIC_VALUES(__VA_ARGS__))                               \
        {                                                                           \
        }                                                                           \
        virtual ~CLASSNAME(HandleImpl, __VA_ARGS__)() {}                            \
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
    template<class ClassName COMMA(__VA_ARGS__) VARIADIC_TEMPLATE(__VA_ARGS__)>     \
    ClassName* CreateHandleObject(VARIADIC_VALUES_LIST(__VA_ARGS__)                 \
    {                                                                               \
        return new HandleImpl<ClassName COMMA(__VA_ARGS__) __VA_ARGS__>(VARIADIC_VALUES(__VA_ARGS__));\
    }

//VARIADIC_EXPAND(CREATE_HANDLE_OBJECT)
#undef CREATE_HANDLE_OBJECT

#define MAKE_SHARED_HANDLE(...)                                                     \
    template <class ClassName COMMA(__VA_ARGS__) VARIADIC_TEMPLATE(__VA_ARGS__)>    \
    class CLASSNAME(HandleImpl, __VA_ARGS__) : public ClassName                     \
    {                                                                               \
    public:                                                                         \
        CLASSNAME(HandleImpl, __VA_ARGS__)(VARIADIC_VALUES_LIST(__VA_ARGS__))       \
            : ClassName(VARIADIC_VALUES(__VA_ARGS__)) {}                            \
        virtual ~CLASSNAME(HandleImpl, __VA_ARGS__)() {}                            \
        virtual void Release() {delete this;}                                       \
    };                                                                              \
                                                                                    \
    template<class ClassName COMMA(__VA_ARGS__) VARIADIC_TEMPLATE(__VA_ARGS__)>     \
    ClassName* make_handle(VARIADIC_VALUES_LIST(__VA_ARGS__))                       \
    {                                                                               \
        return new CLASSNAME(HandleImpl, __VA_ARGS__)<ClassName COMMA(__VA_ARGS__) __VA_ARGS__>(VARIADIC_VALUES(__VA_ARGS__)); \
    }                                                                               \
                                                                                    \
    template<class ClassName COMMA(__VA_ARGS__) VARIADIC_TEMPLATE(__VA_ARGS__)>     \
    std::shared_ptr<ClassName> make_shared_handle(VARIADIC_VALUES_LIST(__VA_ARGS__))\
    {                                                                               \
        std::shared_ptr< CLASSNAME(HandleImpl, __VA_ARGS__)<ClassName COMMA(__VA_ARGS__) __VA_ARGS__> > t \
        (make_handle<CLASSNAME(HandleImpl, __VA_ARGS__)<ClassName COMMA(__VA_ARGS__) __VA_ARGS__>>(VARIADIC_VALUES(__VA_ARGS__)), [&](IHandle* obj) {obj->Release();}); \
        return std::static_pointer_cast<ClassName>(t); \
    }                                                                                                                                               

VARIADIC_EXPAND(MAKE_SHARED_HANDLE)
#undef MAKE_SHARED_HANDLE

#define CLONE_HANDLE(OutType, InternalType) \
    std::shared_ptr<InternalType> obj = make_shared_handle<InternalType>(std::ref(*this));  \
    return static_pointer_cast<OutType>(obj);

#define CLONE_HANDLE_IMPLEMENTATION(OutType, InternalType) \
    virtual std::shared_ptr<OutType> InternalType::Clone() const { \
        std::shared_ptr<InternalType> obj = make_shared_handle<InternalType>(*this);  \
        return static_pointer_cast<OutType>(obj); \
    }