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
	template<>
	class Serializer<std::string>
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const std::string& tValue)
		{
			vNewNode.in_serialize(StrApt<std::string>(tValue));
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, std::string& tValue)
		{
			StrApt<std::string> apt(tValue);
			vNewNode.in_serialize(apt);
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// std::vector<T> 

	template<class T>
	class Serializer<std::vector<T> >
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const std::vector<T>& tValue)
		{
			vNewNode.set_array();
			for (int ix = 0; ix != tValue.size(); ++ix)
			{
				NODE vNode = vNewNode.new_node();
				Serializer<T>::serialize(vNode, tValue[ix]);
				vNewNode.push_node(vNode);
			}
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, std::vector<T>& tValue)
		{
			typename NODE::ArrIter itor = vNewNode.sub_nodes();
			for (; itor; ++itor)
			{
				T t;
				Serializer<T>::deserialize(*itor, t);
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

	template<class T>
	class Serializer<std::list<T> >
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const std::list<T>& tValue)
		{
			vNewNode.set_array();
			for (typename std::list<T>::const_iterator itor = tValue.begin();
				itor != tValue.end(); ++itor)
			{
				NODE vNode = vNewNode.new_node();
				Serializer<T>::serialize(vNode, *itor);
				vNewNode.push_node(vNode);
			}
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, std::list<T>& tValue)
		{
			typename NODE::ArrIter itor = vNewNode.sub_nodes();
			for (; itor; ++itor)
			{
				T t;
				Serializer<T>::deserialize(*itor, t);
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

	template<class T>
	class Serializer<std::set<T> >
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const std::set<T>& tValue)
		{
			vNewNode.set_array();
			for (typename std::set<T>::const_iterator itor = tValue.begin();
				itor != tValue.end(); ++itor)
			{
				NODE vNode = vNewNode.new_node();
				Serializer<T>::serialize(vNode, *itor);
				vNewNode.push_node(vNode);
			}
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, std::set<T>& tValue)
		{
			typename NODE::ArrIter itor = vNewNode.sub_nodes();
			for (; itor; ++itor)
			{
				T t;
				Serializer<T>::deserialize(*itor, t);
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

	template<class T>
	class Serializer<std::deque<T> >
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const std::deque<T>& tValue)
		{
			vNewNode.set_array();
			for (typename std::deque<T>::const_iterator itor = tValue.begin();
				itor != tValue.end(); ++itor)
			{
				NODE vNode = vNewNode.new_node();
				Serializer<T>::serialize(vNode, *itor);
				vNewNode.push_node(vNode);
			}
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, std::deque<T>& tValue)
		{
			typename NODE::ArrIter itor = vNewNode.sub_nodes();
			for (; itor; ++itor)
			{
				T t;
				Serializer<T>::deserialize(*itor, t);
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
	template<class T>
	class Serializer<std::map<std::string, T> >
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const std::map<std::string, T>& tValue)
		{
			vNewNode.set_object();
			for (typename std::map<std::string, T>::const_iterator citor = tValue.begin();
				citor != tValue.end(); ++citor)
			{
				NODE vNode = vNewNode.new_node();
				Serializer<T>::serialize(vNode, citor->second);
				vNewNode.add_member(citor->first.c_str(), vNode);
			}
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, std::map<std::string, T>& tValue)
		{
			std::set<std::string> setKey;

			typename NODE::ObjIter itor = vNewNode.sub_members();
			for (; itor; ++itor)
			{
				if (NODE node = *itor)
				{
					T& t = tValue[itor.key()];
					Serializer<T>::deserialize(node, t);
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
	template<class K, class T>
	class Serializer<std::pair<K, T> >
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const std::pair<K, T>& tValue)
		{
			vNewNode.set_array();

			NODE vKeyNode = vNewNode.new_node();
			Serializer<K>::serialize(vKeyNode, tValue.first);
			vNewNode.push_node(vKeyNode);

			NODE vValueNode = vNewNode.new_node();
			Serializer<T>::serialize(vValueNode, tValue.second);
			vNewNode.push_node(vValueNode);
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, std::pair<K, T>& tValue)
		{
			if (typename NODE::ArrIter itor = vNewNode.sub_nodes())
			{
				Serializer<K>::deserialize(*itor, tValue.first);

				if (++itor)
				{
					Serializer<T>::deserialize(*itor, tValue.second);
				}
			}
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// std::map<K, T> 
	template<class K, class T>
	class Serializer<std::map<K, T> >
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const std::map<K, T>& tValue)
		{
			vNewNode.set_array();

			for (typename std::map<K, T>::const_iterator citor = tValue.begin();
				citor != tValue.end(); ++citor)
			{
				NODE vSubNode = vNewNode.new_node();
				vSubNode.set_array();

				NODE vKeyNode = vSubNode.new_node();
				Serializer<K>::serialize(vKeyNode, citor->first);
				vSubNode.push_node(vKeyNode);

				NODE vValueNode = vSubNode.new_node();
				Serializer<T>::serialize(vValueNode, citor->second);
				vSubNode.push_node(vValueNode);

				vNewNode.push_node(vSubNode);
			}
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, std::map<K, T>& tValue)
		{
			for (typename NODE::ArrIter itor = vNewNode.sub_nodes();
				itor; ++itor)
			{
				if (typename NODE::ArrIter itorSub = (*itor).sub_nodes())
				{
					K k;
					Serializer<K>::deserialize(*itorSub, k);

					T& v = tValue[k];
					if (++itorSub)
					{
						Serializer<T>::deserialize(*itorSub, v);
					}
				}
			}
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// std::multimap<K, T> 
	template<class K, class T>
	class Serializer<std::multimap<K, T> >
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const std::multimap<K, T>& tValue)
		{
			vNewNode.set_array();

			for (typename std::multimap<K, T>::const_iterator citor = tValue.begin();
				citor != tValue.end(); ++citor)
			{
				NODE vSubNode = vNewNode.new_node();
				vSubNode.set_array();

				NODE vKeyNode = vSubNode.new_node();
				Serializer<K>::serialize(vKeyNode, citor->first);
				vSubNode.push_node(vKeyNode);

				NODE vValueNode = vSubNode.new_node();
				Serializer<T>::serialize(vValueNode, citor->second);
				vSubNode.push_node(vValueNode);

				vNewNode.push_node(vSubNode);
			}
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, std::multimap<K, T>& tValue)
		{
			for (typename NODE::ArrIter itor = vNewNode.sub_nodes();
				itor; ++itor)
			{
				if (typename NODE::ArrIter itorSub = (*itor).sub_nodes())
				{
					K k;
					T t;
					Serializer<K>::deserialize(*itorSub, k);

					if (++itorSub)
					{
						Serializer<T>::deserialize(*itorSub, t);
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

	template<typename T>
	class Serializer<std::shared_ptr<T>>
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const std::shared_ptr<T>& tValue)
		{
			Serializer<T*>::serialize(vNewNode, tValue.get());
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, std::shared_ptr<T>& tValue)
		{
			T* pRet = 0;
			Serializer<T*>::deserialize(vNewNode, pRet);
			tValue.reset(pRet);
		}
	};

	template<typename T>
	class Serializer<std::unique_ptr<T>>
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const std::unique_ptr<T>& tValue)
		{
			Serializer<T*>::serialize(vNewNode, tValue.get());
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, std::unique_ptr<T>& tValue)
		{
			T* pRet = 0;
			Serializer<T*>::deserialize(vNewNode, pRet);
			tValue.reset(pRet);
		}
	};

	template <size_t N, class NODE, class TUPLET>
	class SNode
	{
	public:
		static void Push(NODE &vUpNode, const TUPLET& tValue)
		{
			SNode<N - 1, NODE, TUPLET>::Push(vUpNode, tValue);

			NODE vKeyNode = vUpNode.new_node();
			typedef typename std::tuple_element<N, TUPLET>::type V;
			Serializer<V>::serialize(vKeyNode, std::get<N>(tValue));
			vUpNode.push_node(vKeyNode);
		}

		static void Pull(typename NODE::ArrIter &itor, TUPLET &tValue)
		{
			SNode<N - 1, NODE, TUPLET>::Pull(itor, tValue);

			if (itor)
			{
				if (NODE node = *itor)
				{
					typedef typename std::tuple_element<N, TUPLET>::type V;
					Serializer<V>::deserialize(node, std::get<N>(tValue));
				}

				++itor;
			}
		}
	};

	template <class NODE, class TUPLET>
	class SNode<0, NODE, TUPLET>
	{
	public:
		static void Push(NODE &vUpNode, const TUPLET& tValue)
		{
			NODE vKeyNode = vUpNode.new_node();
			typedef typename std::tuple_element<0, TUPLET>::type V;
			Serializer<V>::serialize(vKeyNode, std::get<0>(tValue));
			vUpNode.push_node(vKeyNode);
		}

		static void Pull(typename NODE::ArrIter& itor, TUPLET& tValue)
		{
			if (itor)
			{
				if (NODE node = *itor)
				{
					typedef typename std::tuple_element<0, TUPLET>::type V;
					Serializer<V>::deserialize(node, std::get<0>(tValue));
				}

				++itor;
			}
		}
	};

	template <typename... T>
	class Serializer<std::tuple<T...> >
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const std::tuple<T...>& tValue)
		{
			vNewNode.set_array();

			SNode<std::tuple_size<std::tuple<T...> >::value - 1, NODE, std::tuple<T...> >::Push(vNewNode, tValue);
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, std::tuple<T...>& tValue)
		{
			typename NODE::ArrIter itor = vNewNode.sub_nodes();
			SNode<std::tuple_size<std::tuple<T...> >::value - 1, NODE, std::tuple<T...> >::Pull(itor, tValue);
		}
	};

	template<class T>
	class Serializer<std::unordered_map<std::string, T> >
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const std::unordered_map<std::string, T>& tValue)
		{
			vNewNode.set_object();
			for (typename std::unordered_map<std::string, T>::const_iterator citor = tValue.begin();
				citor != tValue.end(); ++citor)
			{
				NODE vNode = vNewNode.new_node();
				Serializer<T>::serialize(vNode, citor->second);
				vNewNode.add_member(citor->first.c_str(), vNode);
			}
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, std::unordered_map<std::string, T>& tValue)
		{
			std::set<std::string> setKey;

			typename NODE::ObjIter itor = vNewNode.sub_members();
			for (; itor; ++itor)
			{
				if (NODE node = *itor)
				{
					T& t = tValue[itor.key()];
					Serializer<T>::deserialize(node, t);
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

	template<class K, class T>
	class Serializer<std::unordered_map<K, T> >
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const std::unordered_map<K, T>& tValue)
		{
			vNewNode.set_array();

			for (typename std::unordered_map<K, T>::const_iterator citor = tValue.begin();
				citor != tValue.end(); ++citor)
			{
				NODE vSubNode = vNewNode.new_node();
				vSubNode.set_array();

				NODE vKeyNode = vSubNode.new_node();
				Serializer<K>::serialize(vKeyNode, citor->first);
				vSubNode.push_node(vKeyNode);

				NODE vValueNode = vSubNode.new_node();
				Serializer<T>::serialize(vValueNode, citor->second);
				vSubNode.push_node(vValueNode);

				vNewNode.push_node(vSubNode);
			}
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, std::unordered_map<K, T>& tValue)
		{
			for (typename NODE::ArrIter itor = vNewNode.sub_nodes();
				itor; ++itor)
			{
				if (typename NODE::ArrIter itorSub = (*itor).sub_nodes())
				{
					K k;
					T t;
					Serializer<K>::deserialize(*itorSub, k);

					if (++itorSub)
					{
						Serializer<T>::deserialize(*itorSub, t);
					}

					tValue.emplace(std::move(k), std::move(t));
				}
			}

		}
	};

	template<class K, class T>
	class Serializer<std::unordered_multimap<K, T> >
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const std::unordered_multimap<K, T>& tValue)
		{
			vNewNode.set_array();

			for (typename std::unordered_multimap<K, T>::const_iterator citor = tValue.begin();
				citor != tValue.end(); ++citor)
			{
				NODE vSubNode = vNewNode.new_node();
				vSubNode.set_array();

				NODE vKeyNode = vSubNode.new_node();
				Serializer<K>::serialize(vKeyNode, citor->first);
				vSubNode.push_node(vKeyNode);

				NODE vValueNode = vSubNode.new_node();
				Serializer<T>::serialize(vValueNode, citor->second);
				vSubNode.push_node(vValueNode);

				vNewNode.push_node(vSubNode);
			}
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, std::unordered_multimap<K, T>& tValue)
		{
			for (typename NODE::ArrIter itor = vNewNode.sub_nodes();
				itor; ++itor)
			{
				if (typename NODE::ArrIter itorSub = (*itor).sub_nodes())
				{
					K k;
					T t;
					Serializer<K>::deserialize(*itorSub, k);

					if (++itorSub)
					{
						Serializer<T>::deserialize(*itorSub, t);
					}

					tValue.emplace(std::move(k), std::move(t));
				}
			}
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// std::unordered_set<T> 

	template<class T>
	class Serializer<std::unordered_set<T> >
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const std::unordered_set<T>& tValue)
		{
			vNewNode.set_array();
			for (typename std::unordered_set<T>::const_iterator itor = tValue.begin();
				itor != tValue.end(); ++itor)
			{
				NODE vNode = vNewNode.new_node();
				Serializer<T>::serialize(vNode, *itor);
				vNewNode.push_node(vNode);
			}
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, std::unordered_set<T>& tValue)
		{
			typename NODE::ArrIter itor = vNewNode.sub_nodes();
			for (; itor; ++itor)
			{
				T t;
				Serializer<T>::deserialize(*itor, t);
				tValue.emplace(std::move(t));
			}

		}
	};

	template<class T>
	class Serializer<std::unordered_multiset<T> >
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const std::unordered_multiset<T>& tValue)
		{
			vNewNode.set_array();
			for (typename std::unordered_multiset<T>::const_iterator itor = tValue.begin();
				itor != tValue.end(); ++itor)
			{
				NODE vNode = vNewNode.new_node();
				Serializer<T>::serialize(vNode, *itor);
				vNewNode.push_node(vNode);
			}
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, std::unordered_multiset<T>& tValue)
		{
			typename NODE::ArrIter itor = vNewNode.sub_nodes();
			for (; itor; ++itor)
			{
				T t;
				Serializer<T>::deserialize(*itor, t);
				tValue.emplace(std::move(t));
			}

		}
	};

	////////////////////////////////////////////////////
	template<typename T, size_t N>
	class Serializer<std::array<T, N>>
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const std::array<T, N>& tValue)
		{
			vNewNode.set_array();

			for (size_t ix = 0; ix != N; ++ix)
			{
				NODE vNode = vNewNode.new_node();
				Serializer<T>::serialize(vNode, tValue[ix]);
				vNewNode.push_node(vNode);
			}
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, std::array<T, N>& tValue)
		{
			typename NODE::ArrIter itor = vNewNode.sub_nodes();
			for (size_t ix = 0; ix != N && itor; ++itor, ++ix)
			{
				Serializer<T>::deserialize(*itor, tValue[ix]);
			}
		}

	};

	template<size_t N>
	class Serializer<std::array<char, N>>
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const std::array<char, N>& tValue)
		{
			vNewNode.in_serialize(tValue.data());
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, std::array<char, N>& tValue)
		{
			vNewNode.in_serialize(tValue.data(), N);
		}
	};

#endif
}

#endif // MSARCHIVE_STL_H__
