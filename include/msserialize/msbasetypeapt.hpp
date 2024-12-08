#pragma once

namespace MSRPC
{
	template<typename S, typename T>
	class BaseTypeApt;

	template<typename S, typename T>
	S T2S(const T& t)
	{
		return BaseTypeApt<S, T>(t);
	}

	template<typename S, typename T>
	T S2T(const S& s)
	{
		T t;
		(BaseTypeApt<S, T>(t)) = s;
		return t;
	}
}