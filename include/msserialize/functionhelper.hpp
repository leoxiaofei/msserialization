#ifndef FUNCTIONHELPER_H__
#define FUNCTIONHELPER_H__

#include "cplusplusmacros.h"

#if ANY_CPP11_OR_GREATER

template <typename T> 
struct function_traits  
	: function_traits<decltype(&T::operator())>
{  
	// This generic template is instantiated on both the compilers as expected. 
}; 

template <typename R, typename C, typename... PList>
struct function_traits<R (C::*)(PList...) const>
{
	typedef R return_type; 
}; 

template <typename R, typename C, typename... PList>
struct function_traits<R(C::*)(PList...)>
{
	typedef R return_type;
};

template <typename R, typename... PList> 
struct function_traits<R (*)(PList...)>
{
	typedef R return_type; 
};

#endif

#endif // FUNCTIONHELPER_H__
