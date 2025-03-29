#include "../include/essentials.h"

template <typename DerivedType, typename BaseType>
bool isClass(){
    return std::is_base_of<BaseType, DerivedType>::value;
}