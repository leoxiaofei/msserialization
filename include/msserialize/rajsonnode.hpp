#ifndef QJSONNODE_H__
#define QJSONNODE_H__

#include "msarchive.hpp"

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>


namespace MSRPC
{
	class INodeJson
	{
	private:
		rapidjson::Value m_data;
		rapidjson::Value* m_node;
		rapidjson::Document::AllocatorType* m_allocator;

	public:
		template <class T>
		void in_serialize(const T& tValue)
		{
			*m_node = tValue;
		}

		void in_serialize(const char* tValue)
		{
			m_node->SetString(rapidjson::StringRef(tValue));
		}

		void in_serialize(char* tValue)
		{
			m_node->SetString(tValue, *m_allocator);
		}

		template <typename T>
		void in_serialize(const StrApt<T>& tValue)
		{
			in_serialize(tValue.Get());
		}

		INodeJson new_node()
		{
			return INodeJson(m_allocator);
		}

		void set_object(bool bIO = false)
		{
			if (!m_node->IsObject())
			{
				m_node->SetObject();
			}
		}

		void add_member(const char* strName, INodeJson& vNode)
		{
			m_node->AddMember(rapidjson::StringRef(strName), *vNode.m_node, *m_allocator);
		}

		void set_array()
		{
			if (!m_node->IsArray())
			{
				m_node->SetArray();
			}
		}

		void push_node(INodeJson& vNode)
		{
			m_node->PushBack(*vNode.m_node, *m_allocator);
		}

	public:
		rapidjson::Value& data()
		{
			return *m_node;
		}

		INodeJson(rapidjson::Document::AllocatorType* allocator)
			: m_node(&m_data)
			, m_allocator(allocator)
		{

		}

		INodeJson(rapidjson::Document& doc)
			: m_node(&doc)
			, m_allocator(&doc.GetAllocator())
		{

		}

	};

	class ONodeJson
	{
	public:
		const rapidjson::Value* m_node;

		ONodeJson(const rapidjson::Value& node) 
			: m_node(&node) {}

	public:
		template <class T>
		void in_serialize(T& tValue) const
		{
			tValue = m_node->Get<T>();
		}

		void in_serialize(const char*& tValue) const
		{
			tValue = m_node->GetString();
		}

		template <typename T>
		void in_serialize(const StrApt<T>& tValue) const
		{
			const_cast<StrApt<T>&>(tValue).
				Set(m_node->GetString(), m_node->GetStringLength());
		}

		void in_serialize(char* tValue, size_t nSize) const
		{
			const char* str = m_node->GetString();
			
			if (nSize > strlen(str) + 1)
			{
				nSize = strlen(str) + 1;
			}

			memcpy(tValue, str, nSize);
		}

		ONodeJson sub_member(const char* strName) const
		{
			return ONodeJson((*m_node)[strName]);
		}

		class ONodeArrIter
		{
		public:
			rapidjson::Value::ConstValueIterator citCur;
			rapidjson::Value::ConstValueIterator citEnd;

			ONodeArrIter(const rapidjson::Value* node)
				: citCur(node->Begin())
				, citEnd(node->End())
			{}

		public:
			ONodeJson operator *() const
			{
				return ONodeJson(*citCur);
			}

			operator bool() const
			{
				return citCur != citEnd;
			}

			ONodeArrIter& operator ++ ()
			{
				++citCur;
				return *this;
			}
		};

		typedef ONodeArrIter ArrIter;

		ArrIter sub_nodes() const
		{
			return ArrIter(m_node);
		}

		operator bool() const
		{
			return m_node && !m_node->IsNull();
		}
	};

	typedef MSRPC::OArchiveHelper<MSRPC::ONodeJson> OJsonArc;
	typedef MSRPC::IArchiveHelper<MSRPC::INodeJson> IJsonArc;
}

#endif // QJSONNODE_H__
