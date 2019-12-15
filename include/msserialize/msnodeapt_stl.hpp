#ifndef MSNODEAPT_STL_H__
#define MSNODEAPT_STL_H__

#include "functionhelper.hpp"
#include <vector>


namespace MSRPC
{
	template<typename ELEM, typename F>
	class VecReshape
	{
	public:
		VecReshape(std::vector<ELEM>& val, const F& f)
			: conta(val)
			, func(f)
			, idx(0)
		{}

		VecReshape(const VecReshape& other)
			: conta(other.conta)
			, func(other.func)
			, idx(other.idx)
		{

		}

		typedef typename function_traits<F>::return_type item_type;

		item_type operator*() const
		{
			return func(conta[idx]);
		}

		item_type push()
		{
			if (idx >= conta.size())
			{
				ELEM t;
				conta.push_back(t);
			}
			return func(conta[idx++]);
		}

		void operator ++() const
		{
			++(const_cast<VecReshape*>(this)->idx);
		}

		operator bool() const
		{
			return idx < conta.size();
		}

	public:
		std::vector<ELEM>& conta;
		F func;
		size_t idx;
	};

	template<typename C, typename F>
	VecReshape <typename C::value_type, F> ReshapeApt(C& c, const F& f)
	{
		return VecReshape<typename C::value_type, F>(c, f);
	}

	//////////////////////////////////////////////////////////////////////////
	// std::vector<T> 

	template<class NODE, typename T, typename F>
	class ISerialize<NODE, VecReshape<T, F> >
	{
	public:
		static void serialize(NODE& vNewNode, const VecReshape<T, F>& tValue)
		{
			vNewNode.set_array();
			
			for (; tValue; ++tValue)
			{
				NODE vNode = vNewNode.new_node();
				ISerialize<NODE, VecReshape<T, F>::item_type>
					::serialize(vNode, *tValue);
				vNewNode.push_node(vNode);
			}
		}
	};

	template<class NODE, typename T, typename F>
	class OSerialize<NODE, VecReshape<T, F> >
	{
	public:
		static void serialize(const NODE& vNewNode, VecReshape<T, F>& tValue)
		{
			typename NODE::ArrIter itor = vNewNode.sub_nodes();
			for (; itor; ++itor)
			{
				OSerialize<NODE, VecReshape<T, F>::item_type>
					::serialize(*itor, tValue.push());
			}
		}
	};
}

#endif // MSNODEAPT_STL_H__