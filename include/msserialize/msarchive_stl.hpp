#ifndef MSARCHIVE_STL_H__
#define MSARCHIVE_STL_H__

#include "msarchive.hpp"

#include <vector>
#include <string>


namespace MSRPC
{
	template<>
	class StrApt<std::string>
	{
	private:
		std::string& m_str;

	public:
		StrApt(std::string& str)
			: m_str(str) {}

		StrApt(const std::string& str) 
			: m_str(const_cast<std::string&>(str)) {}

		const char* Get() const
		{
			return m_str.c_str();
		}

		void Set(const char* tValue, const size_t& sSize)
		{
			m_str.assign(tValue, sSize);
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// std::string

	template<class NODE>
	class ISerialize<NODE, std::string>
	{
	public:
		static void serialize(NODE& vNewNode, const std::string& tValue)
		{
			vNewNode.in_serialize(StrApt<std::string>(tValue));
		}
	};

	template<class NODE>
	class OSerialize<NODE, std::string>
	{
	public:
		static void serialize(NODE& vNewNode, std::string& tValue)
		{
			vNewNode.in_serialize(StrApt<std::string>(tValue));
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// std::vector<T> 

	template<class NODE, class T>
	class ISerialize<NODE, std::vector<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const std::vector<T>& tValue)
		{
			vNewNode.set_array();
			for (int ix = 0; ix != tValue.size(); ++ix)
			{
				NODE vNode = vNewNode.new_node();
				ISerialize<NODE, T>::serialize(vNode, tValue[ix]);
				vNewNode.push_node(vNode);
			}
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, std::vector<T> >
	{
	public:
		static void serialize(NODE& vNewNode, std::vector<T>& tValue)
		{
			typename NODE::ArrIter itor = vNewNode.sub_nodes();
			for (; itor; ++itor)
			{
				T t;
				OSerialize<NODE, T>::serialize(*itor, t);
				tValue.push_back(t);
			}

		}
	};
}

#endif // MSARCHIVE_STL_H__
