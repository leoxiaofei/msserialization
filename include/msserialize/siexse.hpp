#pragma once

#define MS_CONCAT(A, B) _MS_CONCAT(A, B)
#define _MS_CONCAT(A, B) __MS_CONCAT(A, B)
#define __MS_CONCAT(A, B) A##B

#define MS_EMPTY()
#define MS_PROT(FUNC) FUNC MS_EMPTY()

#define _MS_MACROARGCHECK(_0, \
	_01, _02, _03, _04, _05, _06, _07, _08, _09, _10, \
	_11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
	_21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
	_31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
	_41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
	_51, _52, _53, _54, _55, _56, _57, _58, _59, _60, \
	_61, _62, _63, _64, _65, _66, _67, _68, _69, _70, \
	_71, _72, _73, _74, _75, _76, _77, _78, _79, _80, \
	_81, _82, _83, _84, _85, _86, _87, _88, _89, _90, \
	_91, _92, _93, _94, _95, _96, _97, _98, _99, _100, \
	TARGET, ...) TARGET

#define MS_MACROARGCHECK(...) MS_PROT(_MS_MACROARGCHECK)(__VA_ARGS__,\
	N, N, N, N, N, N, N, N, N, N, \
	N, N, N, N, N, N, N, N, N, N, \
	N, N, N, N, N, N, N, N, N, N, \
	N, N, N, N, N, N, N, N, N, N, \
	N, N, N, N, N, N, N, N, N, N, \
	N, N, N, N, N, N, N, N, N, N, \
	N, N, N, N, N, N, N, N, N, N, \
	N, N, N, N, N, N, N, N, N, N, \
	N, N, N, N, N, N, N, N, N, N, \
	N, N, N, N, N, N, N, N, N, N, \
	1)

#define MS_ENUMARGS(MSFUNC,...) MS_EXPAND(MS_FOREACH(MSFUNC, __VA_ARGS__))

#define MS_FOREACH(MSFUNC, ...) MS_PROT(MS_CONCAT(_MS_FOREACH, MS_MACROARGCHECK(__VA_ARGS__))) (MSFUNC, __VA_ARGS__)
#define _MS_FOREACH() MS_FOREACH
#define _MS_FOREACH1(MSFUNC, A) MSFUNC(A)
#define _MS_FOREACHN(MSFUNC, A, ...) MSFUNC(A) MS_PROT(_MS_FOREACH)() (MSFUNC, __VA_ARGS__)


#define MS_EXPAND(...)  MS_EXPAND1(MS_EXPAND1(MS_EXPAND1(__VA_ARGS__)))
#define MS_EXPAND1(...) MS_EXPAND2(MS_EXPAND2(MS_EXPAND2(__VA_ARGS__)))
#define MS_EXPAND2(...) MS_EXPAND3(MS_EXPAND3(MS_EXPAND3(__VA_ARGS__)))
#define MS_EXPAND3(...) MS_EXPAND4(MS_EXPAND4(MS_EXPAND4(__VA_ARGS__)))
#define MS_EXPAND4(...) MS_EXPAND5(MS_EXPAND5(MS_EXPAND5(__VA_ARGS__)))
#define MS_EXPAND5(...) MS_EXPAND6(MS_EXPAND6(MS_EXPAND6(__VA_ARGS__)))
#define MS_EXPAND6(...) __VA_ARGS__


#define SiArIo(MEM) ar.io(#MEM, tValue.MEM); \

#define SiExSe(TYPE, ...) template<class Ar> \
void ex_serialize(Ar& ar, TYPE& tValue) \
{ \
	MS_ENUMARGS(SiArIo, __VA_ARGS__) \
}

#define SiExSePar SiExSeInhe

#define SiExSeInhe(TYPE, PAR, ...) template<class Ar> \
void ex_serialize(Ar& ar, TYPE& tValue) \
{ \
	ar & static_cast<PAR&>(tValue); \
	MS_ENUMARGS(SiArIo, __VA_ARGS__) \
}

#define SiExSeInheVoid(TYPE, PAR) template<class Ar> \
void ex_serialize(Ar& ar, TYPE& tValue) \
{ \
	ar & static_cast<PAR&>(tValue); \
}


#define SiExSeVoid(TYPE) template<class Ar> \
void ex_serialize(Ar& ar, TYPE& tValue) \
{ \
}

#define SiExSeTemp(TMP, TYPE, ...) template<class Ar, typename TMP> \
void ex_serialize(Ar& ar, TYPE& tValue) \
{ \
	MS_ENUMARGS(SiArIo, __VA_ARGS__) \
}
