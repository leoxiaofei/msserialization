#ifndef MSARCHIVE_STL_H__
#define MSARCHIVE_STL_H__

#include "msarchive.hpp"

#include <vector>
#include <string>
#include <list>


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
		static void serialize(const NODE& vNewNode, std::string& tValue)
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
		static void serialize(const NODE& vNewNode, std::vector<T>& tValue)
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

	//////////////////////////////////////////////////////////////////////////
	// std::list<T> 

	template<class NODE, class T>
	class ISerialize<NODE, std::list<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const std::list<T>& tValue)
		{
			vNewNode.set_array();
			for (std::list<T>::const_iterator itor = tValue.begin(); 
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
				tValue.push_back(t);
			}

		}
	};
	
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
					T& t = val[itor.key()];
					OSerialize<NODE, T>::serialize(node, t);
					setKey.insert(itor.key());
				}
			}

			if (setKey.size() != tValue.size())
			{
				for (std::map<std::string, T>::iterator itor = tValue.begin();
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
	class ISerialize<NODE, std::map<K, T> >
	{
	public:
		static void serialize(NODE& vNewNode, const std::map<K, T>& tValue)
		{
			vNewNode.set_object();

			if (!tValue.isEmpty())
			{
				NODE vKeyNode = vNewNode.new_node();
				vKeyNode.set_array();

				NODE vValueNode = vNewNode.new_node();
				vValueNode.set_array();

				for (typename std::map<K, T>::const_iterator citor = tValue.begin();
					 citor != tValue.end(); ++citor)
				{
					NODE vKNode = vKeyNode.new_node();
					ISerialize<NODE, K>::serialize(vKNode, citor->first);
					vKeyNode.push_node(vKNode);

					NODE vVNode = vValueNode.new_node();
					ISerialize<NODE, T>::serialize(vVNode, citor->second);
					vValueNode.push_node(vVNode);
				}

				vNewNode.add_member("key", vKeyNode);
				vNewNode.add_member("value", vValueNode);
			}
		}
	};

	template<class NODE, class K, class T>
	class OSerialize<NODE, std::map<K, T> >
	{
	public:
		static void serialize(const NODE& vNewNode, std::map<K, T>& tValue)
		{
			NODE vKeyNode = vNewNode.sub_member("key");
			NODE vValueNode = vNewNode.sub_member("value");

			if (vKeyNode && vValueNode)
			{
				std::set<K> setKey;

				typename NODE::ArrIter itorKey = vKeyNode.sub_nodes();
				typename NODE::ArrIter itorValue = vValueNode.sub_nodes();
				for (; itorKey && itorValue; ++itorKey, ++itorValue)
				{
					NODE nodeKey = *itorKey;
					NODE nodeVal = *itorValue;
					if (nodeKey && nodeVal)
					{
						K k;
						OSerialize<NODE, K>::serialize(nodeKey, k);
						T& t = tValue[k];
						OSerialize<NODE, T>::serialize(nodeVal, t);
						setKey.insert(k);
					}
				}

				if (setKey.size() != tValue.size())
				{
					for (std::map<K, T>::iterator itor = tValue.begin();
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
		}
	};

	template<class NODE, typename T>
	class ISerialize<NODE, std::shared_ptr<T>>
	{
	public:
		static void serialize(NODE& vNewNode, const std::shared_ptr<T>& tValue)
		{
			if (tValue)
			{
				ISerialize<NODE, T>::serialize(vNewNode, *tValue);
			}
		}
	};

	template<class NODE, typename T>
	class OSerialize<NODE, std::shared_ptr<T>>
	{
	public:
		static void serialize(const NODE& vNewNode, std::shared_ptr<T>& tValue)
		{
			tValue = std::shared_ptr<T>(new T);
			OSerialize<NODE, T>::serialize(vNewNode, *tValue);
		}
	};
}

#endif // MSARCHIVE_STL_H__
