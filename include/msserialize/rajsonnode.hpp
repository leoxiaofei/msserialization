#ifndef RAJSONNODE_H__
#define RAJSONNODE_H__


#include "msarchive.hpp"

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>


namespace MSRPC
{
	#define is_valid_number(x) (!std::isnan(x) && !std::isinf(x))

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

		void in_serialize(const long long& tValue)
		{
			*m_node = static_cast<int64_t>(tValue);
		}

		void in_serialize(const unsigned long long& tValue) 
		{
			*m_node = static_cast<uint64_t>(tValue);
		}

		void in_serialize(const float& tValue)
		{
			if(is_valid_number(tValue))
			{
				*m_node = tValue;
			}
			else
			{
				m_node->SetNull();
			}
		}

		void in_serialize(const double& tValue) 
		{
			if(is_valid_number(tValue))
			{
				*m_node = tValue;
			}
			else
			{
				m_node->SetNull();
			}
		}

		INodeJson new_node()
		{
			return INodeJson(m_allocator);
		}

		void set_object()
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

		void add_member(char* strName, INodeJson& vNode)
		{
			rapidjson::Value vName(strName, *m_allocator);
			m_node->AddMember(vName, *vNode.m_node, *m_allocator);
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

		void finish()
		{
		}

	public:
		rapidjson::Value& data()
		{
			return *m_node;
		}

		operator bool() const
		{
			return m_node && !m_node->IsNull();
		}

		INodeJson(rapidjson::Document::AllocatorType* allocator)
			: m_node(&m_data)
			, m_allocator(allocator)
		{

		}

		INodeJson(rapidjson::Document* doc)
			: m_node(doc)
			, m_allocator(&doc->GetAllocator())
		{

		}

		INodeJson(const INodeJson& other)
			: m_node(other.m_node == &other.m_data ? &m_data : other.m_node)
			, m_allocator(other.m_allocator)
		{

		}

	};

	class ONodeJson
	{
	public:
		const rapidjson::Value* m_node;

		ONodeJson(const rapidjson::Value* node)
			: m_node(node) {}

	public:
		template <class T>
		void in_serialize(T& tValue) const
		{
			if(m_node->Is<T>())
			{
				tValue = m_node->Get<T>();
			}
		}

		void in_serialize(unsigned short& tValue) const
		{
			if(m_node->IsNumber())
			{
				tValue = static_cast<unsigned short>(
					m_node->Get<unsigned int>());
			}
		}

		void in_serialize(long long& tValue) const
		{
			if(m_node->IsNumber())
			{
				tValue = static_cast<long long>(
					m_node->Get<int64_t>());
			}
		}

		void in_serialize(unsigned long long& tValue) const
		{
			if(m_node->IsNumber())
			{
				tValue = static_cast<unsigned long long>(
					m_node->Get<uint64_t>());
			}
		}

		void in_serialize(float& tValue) const
		{
			if(m_node->Is<float>())
			{
				tValue = m_node->Get<float>();
			}
			else if(m_node->IsNull())
			{
				tValue = NAN;
			}
		}

		void in_serialize(double& tValue) const
		{
			if(m_node->Is<double>())
			{
				tValue = m_node->Get<double>();
			}
			else if(m_node->IsNull())
			{
				tValue = NAN;
			}
		}

		void in_serialize(const char*& tValue) const
		{
			if(m_node->IsString())
			{
				tValue = m_node->GetString();
			}
		}

		template <typename T>
		void in_serialize(StrApt<T>& tValue) const
		{
			if(m_node->IsString())
			{
				tValue.Set(m_node->GetString(), m_node->GetStringLength());
			}
		}

		void in_serialize(char* tValue, size_t nSize) const
		{
			if(m_node->IsString())
			{
				const char* str = m_node->GetString();
				
				if (nSize > strlen(str) + 1)
				{
					nSize = strlen(str) + 1;
				}

				memcpy(tValue, str, nSize);
			}
		}

		ONodeJson sub_member(const char* strName) const
		{
			const rapidjson::Value* node = 0;
			do 
			{
				if (!m_node->IsObject())
				{
					break;
				}

				rapidjson::Value::ConstMemberIterator itrFind = m_node->FindMember(strName);
				if (itrFind == m_node->MemberEnd())
				{
					break;
				}

				node = &itrFind->value;

			} while (false);

			return ONodeJson(node);
		}

		class ONodeObjIter
		{
		public:
			rapidjson::Value::ConstMemberIterator citCur;
			rapidjson::Value::ConstMemberIterator citEnd;

			ONodeObjIter(const rapidjson::Value* node)
				: citCur(node ? node->MemberBegin() : rapidjson::Value::ConstMemberIterator())
				, citEnd(node ? node->MemberEnd() : rapidjson::Value::ConstMemberIterator())
			{}

		public:
			ONodeJson operator *() const
			{
				return ONodeJson(&citCur->value);
			}

			const char* key() const
			{
				return citCur->name.GetString();
			}

			operator bool() const
			{
				return citCur != citEnd;
			}

			ONodeObjIter& operator ++ ()
			{
				++citCur;
				return *this;
			}
		};

		typedef ONodeObjIter ObjIter;

		ObjIter sub_members() const
		{
			return ObjIter(m_node && m_node->IsObject() ? m_node : nullptr);
		}

		class ONodeArrIter
		{
		public:
			rapidjson::Value::ConstValueIterator citCur;
			rapidjson::Value::ConstValueIterator citEnd;

			ONodeArrIter(const rapidjson::Value* node)
				: citCur(node ? node->Begin() : rapidjson::Value::ConstValueIterator())
				, citEnd(node ? node->End() : rapidjson::Value::ConstValueIterator())
			{}

		public:
			ONodeJson operator *() const
			{
				return ONodeJson(citCur);
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
			return ArrIter(m_node && m_node->IsArray() ? m_node : nullptr);
		}

		operator bool() const
		{
			return m_node && !m_node->IsNull();
		}
	};

	typedef MSRPC::OArchiveHelper<MSRPC::ONodeJson> OJsonArc;
	typedef MSRPC::IArchiveHelper<MSRPC::INodeJson> IJsonArc;
}

#endif // RAJSONNODE_H__
