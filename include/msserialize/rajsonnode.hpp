#ifndef RAJSONNODE_H__
#define RAJSONNODE_H__


#include "msarchive.hpp"

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>


namespace MSRPC
{
	#define is_valid_number(x) (!std::isnan(x) && !std::isinf(x))

	class INodeRapidJson
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

		INodeRapidJson new_node()
		{
			return INodeRapidJson(m_allocator);
		}

		void set_object()
		{
			if (!m_node->IsObject())
			{
				m_node->SetObject();
			}
		}

		void add_member(const char* strName, INodeRapidJson& vNode)
		{
			m_node->AddMember(rapidjson::StringRef(strName), *vNode.m_node, *m_allocator);
		}

		void add_member(char* strName, INodeRapidJson& vNode)
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

		void push_node(INodeRapidJson& vNode)
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

		INodeRapidJson(rapidjson::Document::AllocatorType* allocator)
			: m_node(&m_data)
			, m_allocator(allocator)
		{

		}

		INodeRapidJson(rapidjson::Document* doc)
			: m_node(doc)
			, m_allocator(&doc->GetAllocator())
		{

		}

		INodeRapidJson(const INodeRapidJson& other)
			: m_node(other.m_node == &other.m_data ? &m_data : other.m_node)
			, m_allocator(other.m_allocator)
		{

		}

	};

	class ONodeRapidJson
	{
	public:
		const rapidjson::Value* m_node;

		ONodeRapidJson(const rapidjson::Value* node)
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

		ONodeRapidJson sub_member(const char* strName) const
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

			return ONodeRapidJson(node);
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
			ONodeRapidJson operator *() const
			{
				return ONodeRapidJson(&citCur->value);
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
			ONodeRapidJson operator *() const
			{
				return ONodeRapidJson(citCur);
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

	class ODocRapidJson : public ONodeRapidJson
	{
	public:
		template<class StrBuf>
		bool Parse(StrBuf& strJson)
		{
			bool bRet = false;
			try
			{
				// Poco::JSON::Parser parser;
				// BufferApt<StrBuf> apt(strJson);
				// parser.parse(apt).swap(m_data);
				// bRet = !m_data.isEmpty();
			}
			catch (const Poco::Exception& e)
			{
				std::cerr << e.displayText() << std::endl;
			}

			return bRet;
		}

		bool Load(const char* strFilePath)
		{
			bool bRet = false;
			try
			{
				std::ifstream ifs(strFilePath);
				if (ifs)
				{
					// Poco::JSON::Parser parser;
					// parser.parse(ifs).swap(m_data);
					// bRet = !m_data.isEmpty();
				}
			}
			catch (const std::exception&)
			{

			}

			return bRet;
		}
	};

	class IDocRapidJson : public INodeRapidJson
	{
		rapidjson::Document m_doc;
	public:
		IDocRapidJson()
			: INodeRapidJson(&m_doc)
		{ }

		template<class RsvBuffer>
		bool BuffDocWrite(RsvBuffer& buffer, unsigned int indent)
		{
			bool bRet = false;

			if (indent)
			{
				rapidjson::PrettyWriter<RsvBuffer> writer(buffer);
				writer.SetIndent(' ', indent);
				bRet = doc.Accept(writer);
			}
			else
			{
				rapidjson::Writer<RsvBuffer> writer(buffer);
				bRet = doc.Accept(writer);
			}

			return bRet;
		}

		template<class StrBuf = std::string>
		StrBuf Stringify(unsigned int indent = 0)
		{
			StrBuf strRet;
			typedef MSRPC::TBufferAdapter<StrBuf> RsvBuffer;
			RsvBuffer buffer(strRet);

			BuffDocWrite(doc, buffer, indent);

			return strRet;
		}

		template<class StrBuf>
		void Stringify(StrBuf& buf, unsigned int indent = 0)
		{
			// Poco::JSON::Stringifier::stringify(m_data, buf, indent, -1, Poco::JSON_WRAP_STRINGS);
		}

		bool Save(const char* strFilePath, unsigned int indent = 0)
		{
			bool bRet = false;

			std::ofstream outfile(strFilePath);
			if (outfile)
			{
				// Poco::JSON::Stringifier::stringify(m_data, outfile, indent, 1, Poco::JSON_WRAP_STRINGS);
				bRet = true;
			}

			return bRet;
		}
	};

	typedef MSRPC::OArchiveHelper<MSRPC::ONodeRapidJson> OJsonArc;
	typedef MSRPC::IArchiveHelper<MSRPC::INodeRapidJson> IJsonArc;
	typedef MSRPC::IDocRapidJson IDoc;
}

#endif // RAJSONNODE_H__
