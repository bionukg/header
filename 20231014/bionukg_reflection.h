#pragma once
#ifndef bionukg_reflection_h
#define bionukg_reflection_h

#include <typeinfo>

#ifdef namespace_bionukg
namespace bionukg
{
#endif

template<typename T>
class withInfo{
public:
    T obj;
    /*constexpr*/ const /*static*/ /*inline*/ type_info& info = typeid(T);

    withInfo(T obj) : obj(obj) {}

    template<typename... Args>
    withInfo(Args... args) : obj(args...) {}
    
};




#ifdef namespace_bionukg
}
#endif

#endif // bionukg_reflection_h
