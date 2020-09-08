#ifndef MSNODEAPT_H__
#define MSNODEAPT_H__

#include "functionhelper.hpp"


namespace MSRPC
{
	template <typename Enum>
	class EnumApt
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

		operator const char* () const
		{
			return m_szText[m_en];
		}

		void operator = (const char* szText)
		{
			for (int ix = 0; ix != m_n; ++ix)
			{
				if (strcmp(m_szText[ix], szText) == 0)
				{
					m_en = static_cast<Enum>(ix);
				}
			}
		}

	private:
		Enum& m_en;
		const char** m_szText;
		int m_n;
	};

	template <typename Enum>
	class StrApt<EnumApt<Enum> >
	{
	public:
		EnumApt<Enum>& m_data;

	public:
		StrApt(EnumApt<Enum>& data) : m_data(data) {}
		StrApt(const EnumApt<Enum>& data) 
			: m_data(const_cast<EnumApt<Enum>&>(data)) {}

		const char* Get() const
		{
			return m_data;
		}

		void Set(const char* tValue, const size_t& sSize)
		{
			m_data = tValue;
		}
	};

	template<class NODE, class T>
	class ISerialize<NODE, EnumApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const EnumApt<T>& tValue)
		{
			vNewNode.in_serialize(StrApt<EnumApt<T> >(tValue));
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, EnumApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, EnumApt<T>& tValue)
		{
			vNewNode.in_serialize(StrApt<EnumApt<T> >(tValue));
		}
	};


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

	//////////////////////////////////////////////////////////////////////////
	// std::vector<T> 
	template<class NODE, typename T, typename F, typename ELEM>
	class ISerialize<NODE, ArrayReshape<T, F, ELEM> >
	{
	public:
		static void serialize(NODE& vNewNode, const ArrayReshape<T, F, ELEM>& tValue)
		{
			vNewNode.set_array();

			for (; tValue; ++tValue)
			{
				NODE vNode = vNewNode.new_node();
				ISerialize<NODE, typename ArrayReshape<T, F, ELEM>::item_type>
					::serialize(vNode, *tValue);
				vNewNode.push_node(vNode);
			}
		}
	};

	template<class NODE, typename T, typename F, typename ELEM>
	class OSerialize<NODE, ArrayReshape<T, F, ELEM> >
	{
	public:
		static void serialize(const NODE& vNewNode, ArrayReshape<T, F, ELEM>& tValue)
		{
			typename NODE::ArrIter itor = vNewNode.sub_nodes();
			for (; itor; ++itor)
			{
				OSerialize<NODE, typename ArrayReshape<T, F, ELEM>::item_type>
					::serialize(*itor, tValue.push());
			}
		}
	};
}


#endif // MSNODEAPT_H__
