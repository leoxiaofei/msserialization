#ifndef MSNODEAPT_H__
#define MSNODEAPT_H__

#include "functionhelper.hpp"
#include <memory>
#include <string.h>

namespace MSRPC
{
	template<typename T>
	class StrApt;

	template <typename Enum>
	class EnumApt : public StrApt<EnumApt<Enum> >
	{
	public:
		template<int N>
		EnumApt(Enum& en, const char* (&str)[N])
			: m_en(en)
			, m_szText(str)
			, m_n(N)
		{

		}

		EnumApt(Enum& en, const char** str, int nN)
			: m_en(en)
			, m_szText(str)
			, m_n(nN)
		{

		}

		const char* Get() const
		{
			return m_szText[static_cast<int>(m_en)];
		}

		void Set(const char* tValue, const size_t& sSize)
		{
			for (int ix = 0; ix != m_n; ++ix)
			{
				if (strncmp(m_szText[ix], tValue, sSize) == 0)
				{
					m_en = static_cast<Enum>(ix);
					break;
				}
			}
		}

	private:
		Enum& m_en;
		const char** m_szText;
		int m_n;
	};

	// template<class T>
	// class Serializer<EnumApt<T> >
	// {
	// public:
	// 	template<class NODE>
	// 	static void serialize(NODE& vNewNode, const EnumApt<T>& tValue)
	// 	{
	// 		vNewNode.in_serialize(StrApt<EnumApt<T> >(tValue));
	// 	}

	// 	template<class NODE>
	// 	static void deserialize(NODE& vNewNode, EnumApt<T>& tValue)
	// 	{
	// 		StrApt<EnumApt<T> > aptValue(tValue);
	// 		vNewNode.in_serialize(aptValue);
	// 	}
	// };


	template<typename C, typename F, typename I>
	class ArrayReshape
	{
	public:
		typedef I item_type;

		ArrayReshape(C& val, const F& f)
			: conta(val)
			, func(f)
			, itor(conta.begin())
		{}

		ArrayReshape(const ArrayReshape& other)
			: conta(other.conta)
			, func(other.func)
			, itor(other.itor)
		{}

		item_type operator*() const
		{
			return func(*itor);
		}

		item_type push()
		{
			if (itor == conta.end())
			{
				typename C::value_type t;
				conta.push_back(t);
				itor = conta.end();
				--itor;
			}
			return func(*itor++);
		}

		void operator ++() const
		{
			++(const_cast<ArrayReshape*>(this)->itor);
		}

		operator bool() const
		{
			return itor != const_cast<ArrayReshape*>(this)->conta.end();
		}

	public:
		C& conta;
		F func;
		typename C::iterator itor;
	};

#if ANY_CPP11_OR_GREATER
	template<typename C, typename F>
	ArrayReshape <C, F, typename function_traits<F>::return_type> ReshapeApt(C& c, const F& f)
	{
		return ArrayReshape<C, F, typename function_traits<F>::return_type>(c, f);
	}
#else
	template<typename C, typename I, typename P1>
	ArrayReshape <C, I(*)(P1), I> ReshapeApt(C& c, I(*f)(P1))
	{
		return ArrayReshape<C, I(*)(P1), I>(c, f);
	}
#endif

	template <typename T, typename IsEnum>
	class Serializer;
	//////////////////////////////////////////////////////////////////////////
	// 
	template<typename T, typename F, typename ELEM>
	class Serializer<ArrayReshape<T, F, ELEM>, void>
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const ArrayReshape<T, F, ELEM>& tValue)
		{
			vNewNode.set_array();

			for (; tValue; ++tValue)
			{
				NODE vNode = vNewNode.new_node();
				Serializer<typename ArrayReshape<T, F, ELEM>::item_type>
					::serialize(vNode, *tValue);
				vNewNode.push_node(vNode);
			}
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, ArrayReshape<T, F, ELEM>& tValue)
		{
			typename NODE::ArrIter itor = vNewNode.sub_nodes();
			for (; itor; ++itor)
			{
				ELEM itemValue = tValue.push();
				Serializer<ELEM>::deserialize(*itor, itemValue);
			}
		}
	};

	template<>
	class StrApt<class StrType>
	{
	public:
		const char* m_data;
		size_t m_size;

	public:
		StrApt(const char* data = nullptr)
			: m_data(data)
			, m_size(0)
		{}

		~StrApt()
		{
			if (m_size)
			{
				delete[] m_data;
			}
		}

		StrApt(StrApt&& other)
		: m_data(other.m_data)
		, m_size(other.m_size)
		{
			other.m_data = nullptr;
			other.m_size = 0;
		}

		StrApt& operator=(StrApt&& other)
		{
			if (this != &other)
			{
				this->~StrApt();

				m_data = other.m_data;
				m_size = other.m_size;

				other.m_data = nullptr;
				other.m_size = 0;
			}
			return *this;
		}

		StrApt(const StrApt& other)
		: m_data(other.m_data)
		, m_size(0)
		{

		}

		StrApt& operator = (const StrApt& other)
		{
			if (this != &other)
			{
				this->~StrApt();

				m_data = other.m_data;
				m_size = 0;
			}
			return *this;
		}

		const char* Get() const
		{
			return m_data;
		}

		void Set(const char* tValue, const size_t& sSize)
		{
			auto data = new char[sSize+1];
			memcpy(data, tValue, sSize);
			data[sSize] = '\0';
			m_data = data;
			m_size = sSize;
		}

		bool operator < (const StrApt& other) const
		{
			return strcmp(m_data, other.m_data) < 0;
		}

	private:
	};

	typedef StrApt<StrType> StrTypeApt;

	template<class R, class T, class F>
	class ExtractApt;

	template<class R, class T, class F>
	class Serializer<ExtractApt<R, T, F>, void>
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const ExtractApt<R, T, F>& tValue)
		{
			Serializer<R>::serialize(vNewNode, (const R&)tValue);
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, ExtractApt<R, T, F>& tValue)
		{
			R ptValue = tValue;
			Serializer<R>::deserialize(vNewNode, ptValue);
			tValue = ptValue;
		}
	};
}


#endif // MSNODEAPT_H__
