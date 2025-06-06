#ifndef MSARCHIVE_STL_H__
#define MSARCHIVE_STL_H__

#include "cplusplusmacros.h"
#include "msarchive.hpp"

#include <memory>
#include <vector>
#include <string>
#include <list>
#include <map>
#include <set>
#include <iostream>
#include <deque>

#if ANY_CPP11_OR_GREATER
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#endif

namespace MSRPC
{
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
		static void serialize(const NODE& vNewNode, std::string& tValue)
		{
			StrApt<std::string> apt(tValue);
			vNewNode.in_serialize(apt);
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
		static void serialize(const NODE& vNewNode, std::vector<T>& tValue)
		{
			typename NODE::ArrIter itor = vNewNode.sub_nodes();
			for (; itor; ++itor)
			{
				T t;
				OSerialize<NODE, T>::serialize(*itor, t);
#if ANY_CPP11_OR_GREATER
				tValue.emplace_back(std::move(t));
#else
				tValue.push_back(t);
#endif
			}

		}
	};

	//////////////////////////////////////////////////////////////////////////
	// std::list<T> 

	template<class NODE, class T>
	class ISerialize<NODE, std::list<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const std::list<T>& tValue)
		{
			vNewNode.set_array();
			for (typename std::list<T>::const_iterator itor = tValue.begin();
				itor != tValue.end(); ++itor)
			{
				NODE vNode = vNewNode.new_node();
				ISerialize<NODE, T>::serialize(vNode, *itor);
				vNewNode.push_node(vNode);
			}
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, std::list<T> >
	{
	public:
		static void serialize(const NODE& vNewNode, std::list<T>& tValue)
		{
			typename NODE::ArrIter itor = vNewNode.sub_nodes();
			for (; itor; ++itor)
			{
				T t;
				OSerialize<NODE, T>::serialize(*itor, t);
#if ANY_CPP11_OR_GREATER
				tValue.emplace_back(std::move(t));
#else
				tValue.push_back(t);
#endif
			}

		}
	};

	//////////////////////////////////////////////////////////////////////////
	// std::set<T> 

	template<class NODE, class T>
	class ISerialize<NODE, std::set<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const std::set<T>& tValue)
		{
			vNewNode.set_array();
			for (typename std::set<T>::const_iterator itor = tValue.begin();
				itor != tValue.end(); ++itor)
			{
				NODE vNode = vNewNode.new_node();
				ISerialize<NODE, T>::serialize(vNode, *itor);
				vNewNode.push_node(vNode);
			}
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, std::set<T> >
	{
	public:
		static void serialize(const NODE& vNewNode, std::set<T>& tValue)
		{
			typename NODE::ArrIter itor = vNewNode.sub_nodes();
			for (; itor; ++itor)
			{
				T t;
				OSerialize<NODE, T>::serialize(*itor, t);
#if ANY_CPP11_OR_GREATER
				tValue.emplace(std::move(t));
#else
				tValue.insert(t);
#endif
			}

		}
	};

	//////////////////////////////////////////////////////////////////////////
	// std::deque<T> 

	template<class NODE, class T>
	class ISerialize<NODE, std::deque<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const std::deque<T>& tValue)
		{
			vNewNode.set_array();
			for (typename std::deque<T>::const_iterator itor = tValue.begin();
				itor != tValue.end(); ++itor)
			{
				NODE vNode = vNewNode.new_node();
				ISerialize<NODE, T>::serialize(vNode, *itor);
				vNewNode.push_node(vNode);
			}
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, std::deque<T> >
	{
	public:
		static void serialize(const NODE& vNewNode, std::deque<T>& tValue)
		{
			typename NODE::ArrIter itor = vNewNode.sub_nodes();
			for (; itor; ++itor)
			{
				T t;
				OSerialize<NODE, T>::serialize(*itor, t);
#if ANY_CPP11_OR_GREATER
				tValue.emplace_back(std::move(t));
#else
				tValue.push_back(t);
#endif
			}

		}
	};
	//////////////////////////////////////////////////////////////////////////
	// std::map<std::string, T> 
	template<class NODE, class T>
	class ISerialize<NODE, std::map<std::string, T> >
	{
	public:
		static void serialize(NODE& vNewNode, const std::map<std::string, T>& tValue)
		{
			vNewNode.set_object();
			for (typename std::map<std::string, T>::const_iterator citor = tValue.begin();
				citor != tValue.end(); ++citor)
			{
				NODE vNode = vNewNode.new_node();
				ISerialize<NODE, T>::serialize(vNode, citor->second);
				vNewNode.add_member(citor->first.c_str(), vNode);
			}
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, std::map<std::string, T> >
	{
	public:
		static void serialize(const NODE& vNewNode, std::map<std::string, T>& tValue)
		{
			std::set<std::string> setKey;

			typename NODE::ObjIter itor = vNewNode.sub_members();
			for (; itor; ++itor)
			{
				if (NODE node = *itor)
				{
					T& t = tValue[itor.key()];
					OSerialize<NODE, T>::serialize(node, t);
					setKey.insert(itor.key());
				}
			}

			if (setKey.size() != tValue.size())
			{
				for (typename std::map<std::string, T>::iterator itor = tValue.begin();
					itor != tValue.end();)
				{
					if (setKey.find(itor->first) == setKey.end())
					{
						itor = tValue.erase(itor);
					}
					else
					{
						++itor;
					}
				}
			}
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// std::pair<K, T> 
	template<class NODE, class K, class T>
	class ISerialize<NODE, std::pair<K, T> >
	{
	public:
		static void serialize(NODE& vNewNode, const std::pair<K, T>& tValue)
		{
			vNewNode.set_array();

			NODE vKeyNode = vNewNode.new_node();
			ISerialize<NODE, K>::serialize(vKeyNode, tValue.first);
			vNewNode.push_node(vKeyNode);

			NODE vValueNode = vNewNode.new_node();
			ISerialize<NODE, T>::serialize(vValueNode, tValue.second);
			vNewNode.push_node(vValueNode);
		}
	};

	template<class NODE, class K, class T>
	class OSerialize<NODE, std::pair<K, T> >
	{
	public:
		static void serialize(const NODE& vNewNode, std::pair<K, T>& tValue)
		{
			if (typename NODE::ArrIter itor = vNewNode.sub_nodes())
			{
				OSerialize<NODE, K>::serialize(*itor, tValue.first);

				if (++itor)
				{
					OSerialize<NODE, T>::serialize(*itor, tValue.second);
				}
			}
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// std::map<K, T> 
	template<class NODE, class K, class T>
	class ISerialize<NODE, std::map<K, T> >
	{
	public:
		static void serialize(NODE& vNewNode, const std::map<K, T>& tValue)
		{
			vNewNode.set_array();

			for (typename std::map<K, T>::const_iterator citor = tValue.begin();
				citor != tValue.end(); ++citor)
			{
				NODE vSubNode = vNewNode.new_node();
				vSubNode.set_array();

				NODE vKeyNode = vSubNode.new_node();
				ISerialize<NODE, K>::serialize(vKeyNode, citor->first);
				vSubNode.push_node(vKeyNode);

				NODE vValueNode = vSubNode.new_node();
				ISerialize<NODE, T>::serialize(vValueNode, citor->second);
				vSubNode.push_node(vValueNode);

				vNewNode.push_node(vSubNode);
			}
		}
	};

	template<class NODE, class K, class T>
	class OSerialize<NODE, std::map<K, T> >
	{
	public:
		static void serialize(const NODE& vNewNode, std::map<K, T>& tValue)
		{
			for (typename NODE::ArrIter itor = vNewNode.sub_nodes();
				itor; ++itor)
			{
				if (typename NODE::ArrIter itorSub = (*itor).sub_nodes())
				{
					K k;
					OSerialize<NODE, K>::serialize(*itorSub, k);

					T& v = tValue[k];
					if (++itorSub)
					{
						OSerialize<NODE, T>::serialize(*itorSub, v);
					}
				}
			}
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// std::multimap<K, T> 
	template<class NODE, class K, class T>
	class ISerialize<NODE, std::multimap<K, T> >
	{
	public:
		static void serialize(NODE& vNewNode, const std::multimap<K, T>& tValue)
		{
			vNewNode.set_array();

			for (typename std::multimap<K, T>::const_iterator citor = tValue.begin();
				citor != tValue.end(); ++citor)
			{
				NODE vSubNode = vNewNode.new_node();
				vSubNode.set_array();

				NODE vKeyNode = vSubNode.new_node();
				ISerialize<NODE, K>::serialize(vKeyNode, citor->first);
				vSubNode.push_node(vKeyNode);

				NODE vValueNode = vSubNode.new_node();
				ISerialize<NODE, T>::serialize(vValueNode, citor->second);
				vSubNode.push_node(vValueNode);

				vNewNode.push_node(vSubNode);
			}
		}
	};

	template<class NODE, class K, class T>
	class OSerialize<NODE, std::multimap<K, T> >
	{
	public:
		static void serialize(const NODE& vNewNode, std::multimap<K, T>& tValue)
		{
			for (typename NODE::ArrIter itor = vNewNode.sub_nodes();
				itor; ++itor)
			{
				if (typename NODE::ArrIter itorSub = (*itor).sub_nodes())
				{
					K k;
					T t;
					OSerialize<NODE, K>::serialize(*itorSub, k);

					if (++itorSub)
					{
						OSerialize<NODE, T>::serialize(*itorSub, t);
					}

#if ANY_CPP11_OR_GREATER
					tValue.emplace(std::move(k), std::move(t));
#else
					tValue.insert(std::multimap<K, T>::value_type(k, t));
#endif
				}
			}
		}
	};

#if ANY_CPP11_OR_GREATER

	template<class NODE, typename T>
	class ISerialize<NODE, std::shared_ptr<T>>
	{
	public:
		static void serialize(NODE& vNewNode, const std::shared_ptr<T>& tValue)
		{
			ISerialize<NODE, T*>::serialize(vNewNode, tValue.get());
		}
	};

	template<class NODE, typename T>
	class OSerialize<NODE, std::shared_ptr<T>>
	{
	public:
		static void serialize(const NODE& vNewNode, std::shared_ptr<T>& tValue)
		{
			T* pRet = 0;
			OSerialize<NODE, T*>::serialize(vNewNode, pRet);
			tValue.reset(pRet);
		}
	};

	template<class NODE, typename T>
	class ISerialize<NODE, std::unique_ptr<T>>
	{
	public:
		static void serialize(NODE& vNewNode, const std::unique_ptr<T>& tValue)
		{
			ISerialize<NODE, T*>::serialize(vNewNode, tValue.get());
		}
	};

	template<class NODE, typename T>
	class OSerialize<NODE, std::unique_ptr<T>>
	{
	public:
		static void serialize(const NODE& vNewNode, std::unique_ptr<T>& tValue)
		{
			T* pRet = 0;
			OSerialize<NODE, T*>::serialize(vNewNode, pRet);
			tValue.reset(pRet);
		}
	};


	template <size_t N, class NODE, class TUPLET>
	class ISNode
	{
	public:
		static void Push(NODE &vUpNode, const TUPLET& tValue)
		{
			ISNode<N - 1, NODE, TUPLET>::Push(vUpNode, tValue);

			NODE vKeyNode = vUpNode.new_node();
			typedef typename std::tuple_element<N, TUPLET>::type V;
			ISerialize<NODE, V>::serialize(vKeyNode, std::get<N>(tValue));
			vUpNode.push_node(vKeyNode);
		}
	};

	template <class NODE, class TUPLET>
	class ISNode<0, NODE, TUPLET>
	{
	public:
		static void Push(NODE &vUpNode, const TUPLET& tValue)
		{
			NODE vKeyNode = vUpNode.new_node();
			typedef typename std::tuple_element<0, TUPLET>::type V;
			ISerialize<NODE, V>::serialize(vKeyNode, std::get<0>(tValue));
			vUpNode.push_node(vKeyNode);
		}

	};

	template <class NODE, typename... T>
	class ISerialize<NODE, std::tuple<T...> >
	{
	public:
		static void serialize(NODE& vNewNode, const std::tuple<T...>& tValue)
		{
			vNewNode.set_array();

			ISNode<std::tuple_size<std::tuple<T...> >::value - 1, NODE, std::tuple<T...> >::Push(vNewNode, tValue);
		}

	};

	template<size_t N, class NODE, class TUPLET>
	class OSNode
	{
	public:
		static void Pull(typename NODE::ArrIter& itor, TUPLET& tValue)
		{
			OSNode<N - 1, NODE, TUPLET>::Pull(itor, tValue);

			if (itor)
			{
				if (NODE node = *itor)
				{
					typedef typename std::tuple_element<N, TUPLET>::type V;
					OSerialize<NODE, V>::serialize(node, std::get<N>(tValue));
				}

				++itor;
			}
		}
	};

	template<class NODE, class TUPLET>
	class OSNode<0, NODE, TUPLET>
	{
	public:
		static void Pull(typename NODE::ArrIter& itor, TUPLET& tValue)
		{
			if (itor)
			{
				if (NODE node = *itor)
				{
					typedef typename std::tuple_element<0, TUPLET>::type V;
					OSerialize<NODE, V>::serialize(node, std::get<0>(tValue));
				}

				++itor;
			}
		}
	};

	template<class NODE, typename... T>
	class OSerialize<NODE, std::tuple<T...> >
	{
	public:
		static void serialize(const NODE& vNewNode, std::tuple<T...>& tValue)
		{
			typename NODE::ArrIter itor = vNewNode.sub_nodes();
			OSNode<std::tuple_size<std::tuple<T...> >::value - 1, NODE, std::tuple<T...> >::Pull(itor, tValue);
		}
	};

	template<class NODE, class T>
	class ISerialize<NODE, std::unordered_map<std::string, T> >
	{
	public:
		static void serialize(NODE& vNewNode, const std::unordered_map<std::string, T>& tValue)
		{
			vNewNode.set_object();
			for (typename std::unordered_map<std::string, T>::const_iterator citor = tValue.begin();
				citor != tValue.end(); ++citor)
			{
				NODE vNode = vNewNode.new_node();
				ISerialize<NODE, T>::serialize(vNode, citor->second);
				vNewNode.add_member(citor->first.c_str(), vNode);
			}
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, std::unordered_map<std::string, T> >
	{
	public:
		static void serialize(const NODE& vNewNode, std::unordered_map<std::string, T>& tValue)
		{
			std::set<std::string> setKey;

			typename NODE::ObjIter itor = vNewNode.sub_members();
			for (; itor; ++itor)
			{
				if (NODE node = *itor)
				{
					T& t = tValue[itor.key()];
					OSerialize<NODE, T>::serialize(node, t);
					setKey.insert(itor.key());
				}
			}

			if (setKey.size() != tValue.size())
			{
				for (typename std::unordered_map<std::string, T>::iterator itor = tValue.begin();
					itor != tValue.end();)
				{
					if (setKey.find(itor->first) == setKey.end())
					{
						itor = tValue.erase(itor);
					}
					else
					{
						++itor;
					}
				}
			}
		}
	};

	template<class NODE, class K, class T>
	class ISerialize<NODE, std::unordered_map<K, T> >
	{
	public:
		static void serialize(NODE& vNewNode, const std::unordered_map<K, T>& tValue)
		{
			vNewNode.set_array();

			for (typename std::unordered_map<K, T>::const_iterator citor = tValue.begin();
				citor != tValue.end(); ++citor)
			{
				NODE vSubNode = vNewNode.new_node();
				vSubNode.set_array();

				NODE vKeyNode = vSubNode.new_node();
				ISerialize<NODE, K>::serialize(vKeyNode, citor->first);
				vSubNode.push_node(vKeyNode);

				NODE vValueNode = vSubNode.new_node();
				ISerialize<NODE, T>::serialize(vValueNode, citor->second);
				vSubNode.push_node(vValueNode);

				vNewNode.push_node(vSubNode);
			}
		}
	};

	template<class NODE, class K, class T>
	class OSerialize<NODE, std::unordered_map<K, T> >
	{
	public:
		static void serialize(const NODE& vNewNode, std::unordered_map<K, T>& tValue)
		{
			for (typename NODE::ArrIter itor = vNewNode.sub_nodes();
				itor; ++itor)
			{
				if (typename NODE::ArrIter itorSub = (*itor).sub_nodes())
				{
					K k;
					T t;
					OSerialize<NODE, K>::serialize(*itorSub, k);

					if (++itorSub)
					{
						OSerialize<NODE, T>::serialize(*itorSub, t);
					}

					tValue.emplace(std::move(k), std::move(t));
				}
			}

		}
	};

	template<class NODE, class K, class T>
	class ISerialize<NODE, std::unordered_multimap<K, T> >
	{
	public:
		static void serialize(NODE& vNewNode, const std::unordered_multimap<K, T>& tValue)
		{
			vNewNode.set_array();

			for (typename std::unordered_multimap<K, T>::const_iterator citor = tValue.begin();
				citor != tValue.end(); ++citor)
			{
				NODE vSubNode = vNewNode.new_node();
				vSubNode.set_array();

				NODE vKeyNode = vSubNode.new_node();
				ISerialize<NODE, K>::serialize(vKeyNode, citor->first);
				vSubNode.push_node(vKeyNode);

				NODE vValueNode = vSubNode.new_node();
				ISerialize<NODE, T>::serialize(vValueNode, citor->second);
				vSubNode.push_node(vValueNode);

				vNewNode.push_node(vSubNode);
			}
		}
	};

	template<class NODE, class K, class T>
	class OSerialize<NODE, std::unordered_multimap<K, T> >
	{
	public:
		static void serialize(const NODE& vNewNode, std::unordered_multimap<K, T>& tValue)
		{
			for (typename NODE::ArrIter itor = vNewNode.sub_nodes();
				itor; ++itor)
			{
				if (typename NODE::ArrIter itorSub = (*itor).sub_nodes())
				{
					K k;
					T t;
					OSerialize<NODE, K>::serialize(*itorSub, k);

					if (++itorSub)
					{
						OSerialize<NODE, T>::serialize(*itorSub, t);
					}

					tValue.emplace(std::move(k), std::move(t));
				}
			}
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// std::unordered_set<T> 

	template<class NODE, class T>
	class ISerialize<NODE, std::unordered_set<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const std::unordered_set<T>& tValue)
		{
			vNewNode.set_array();
			for (typename std::unordered_set<T>::const_iterator itor = tValue.begin();
				itor != tValue.end(); ++itor)
			{
				NODE vNode = vNewNode.new_node();
				ISerialize<NODE, T>::serialize(vNode, *itor);
				vNewNode.push_node(vNode);
			}
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, std::unordered_set<T> >
	{
	public:
		static void serialize(const NODE& vNewNode, std::unordered_set<T>& tValue)
		{
			typename NODE::ArrIter itor = vNewNode.sub_nodes();
			for (; itor; ++itor)
			{
				T t;
				OSerialize<NODE, T>::serialize(*itor, t);
				tValue.emplace(std::move(t));
			}

		}
	};

	template<class NODE, class T>
	class ISerialize<NODE, std::unordered_multiset<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const std::unordered_multiset<T>& tValue)
		{
			vNewNode.set_array();
			for (typename std::unordered_multiset<T>::const_iterator itor = tValue.begin();
				itor != tValue.end(); ++itor)
			{
				NODE vNode = vNewNode.new_node();
				ISerialize<NODE, T>::serialize(vNode, *itor);
				vNewNode.push_node(vNode);
			}
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, std::unordered_multiset<T> >
	{
	public:
		static void serialize(const NODE& vNewNode, std::unordered_multiset<T>& tValue)
		{
			typename NODE::ArrIter itor = vNewNode.sub_nodes();
			for (; itor; ++itor)
			{
				T t;
				OSerialize<NODE, T>::serialize(*itor, t);
				tValue.emplace(std::move(t));
			}

		}
	};

	////////////////////////////////////////////////////

	template<class NODE, typename T, size_t N>
	class ISerialize<NODE, std::array<T, N>>
	{
	public:
		static void serialize(NODE& vNewNode, const std::array<T, N>& tValue)
		{
			vNewNode.set_array();

			for (size_t ix = 0; ix != N; ++ix)
			{
				NODE vNode = vNewNode.new_node();
				ISerialize<NODE, T>::serialize(vNode, tValue[ix]);
				vNewNode.push_node(vNode);
			}
		}
	};

	template<class NODE, typename T, size_t N>
	class OSerialize<NODE, std::array<T, N>>
	{
	public:
		static void serialize(const NODE& vNewNode, std::array<T, N>& tValue)
		{
			typename NODE::ArrIter itor = vNewNode.sub_nodes();
			for (size_t ix = 0; ix != N && itor; ++itor, ++ix)
			{
				OSerialize<NODE, T>::serialize(*itor, tValue[ix]);
			}
		}

	};

	template<class NODE, size_t N>
	class ISerialize<NODE, std::array<char, N>>
	{
	public:
		static void serialize(NODE& vNewNode, const std::array<char, N>& tValue)
		{
			vNewNode.in_serialize(tValue.data());
		}
	};

	template<class NODE, size_t N>
	class OSerialize<NODE, std::array<char, N>>
	{
	public:
		static void serialize(const NODE& vNewNode, std::array<char, N>& tValue)
		{
			vNewNode.in_serialize(tValue.data(), N);
		}
	};

#endif
}

#endif // MSARCHIVE_STL_H__
