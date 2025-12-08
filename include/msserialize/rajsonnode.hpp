#ifndef RAJSONNODE_H__
#define RAJSONNODE_H__


#include "msarchive.hpp"

#include <fstream>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/writer.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>

namespace MSRPC
{
	class SeNodeRapidJson
	{
	protected:
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
			if(is_valid_float(tValue))
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
			if(is_valid_float(tValue))
			{
				*m_node = tValue;
			}
			else
			{
				m_node->SetNull();
			}
		}

		void set_object()
		{
			if (!m_node->IsObject())
			{
				m_node->SetObject();
			}
		}

		SeNodeRapidJson add_member(const char* strName)
		{
			rapidjson::Value node;
			m_node->AddMember(rapidjson::StringRef(strName), node, *m_allocator);
			return SeNodeRapidJson(&(m_node->MemberEnd() - 1)->value, m_allocator);
		}

		SeNodeRapidJson add_member(char* strName, SeNodeRapidJson& vNode)
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

		SeNodeRapidJson add_element()
		{
			rapidjson::Value node;
			m_node->PushBack(node, *m_allocator);
			return SeNodeRapidJson((m_node->End() - 1), m_allocator);
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

		SeNodeRapidJson(rapidjson::Value* node, rapidjson::Document::AllocatorType* allocator)
			: m_node(node)
			, m_allocator(allocator)
		{

		}

		// SeNodeRapidJson(const SeNodeRapidJson& other)
		// 	: m_node(other.m_node == &other.m_data ? &m_data : other.m_node)
		// 	, m_allocator(other.m_allocator)
		// {

		// }

	};

	class DeNodeRapidJson
	{
	public:
		const rapidjson::Value* m_node;

		DeNodeRapidJson(const rapidjson::Value* node)
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

		void in_serialize(char *&tValue) const
		{
			if(m_node->IsString())
			{
				tValue = new char[m_node->GetStringLength() + 1];
				std::strncpy(tValue, m_node->GetString(), m_node->GetStringLength() + 1);
				tValue[m_node->GetStringLength()] = '\0';
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

		DeNodeRapidJson sub_member(const char* strName) const
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

			return DeNodeRapidJson(node);
		}

		class DeNodeObjIter
		{
		public:
			rapidjson::Value::ConstMemberIterator citCur;
			rapidjson::Value::ConstMemberIterator citEnd;

			DeNodeObjIter(const rapidjson::Value* node)
				: citCur(node ? node->MemberBegin() : rapidjson::Value::ConstMemberIterator())
				, citEnd(node ? node->MemberEnd() : rapidjson::Value::ConstMemberIterator())
			{}

		public:
			DeNodeRapidJson operator *() const
			{
				return DeNodeRapidJson(&citCur->value);
			}

			const char* key() const
			{
				return citCur->name.GetString();
			}

			operator bool() const
			{
				return citCur != citEnd;
			}

			DeNodeObjIter& operator ++ ()
			{
				++citCur;
				return *this;
			}
		};

		typedef DeNodeObjIter ObjIter;

		ObjIter sub_members() const
		{
			return ObjIter(m_node && m_node->IsObject() ? m_node : nullptr);
		}

		class DeNodeArrIter
		{
		public:
			rapidjson::Value::ConstValueIterator citCur;
			rapidjson::Value::ConstValueIterator citEnd;

			DeNodeArrIter(const rapidjson::Value* node)
				: citCur(node ? node->Begin() : rapidjson::Value::ConstValueIterator())
				, citEnd(node ? node->End() : rapidjson::Value::ConstValueIterator())
			{}

		public:
			DeNodeRapidJson operator *() const
			{
				return DeNodeRapidJson(citCur);
			}

			operator bool() const
			{
				return citCur != citEnd;
			}

			DeNodeArrIter& operator ++ ()
			{
				++citCur;
				return *this;
			}
		};

		typedef DeNodeArrIter ArrIter;

		ArrIter sub_elements() const
		{
			return ArrIter(m_node && m_node->IsArray() ? m_node : nullptr);
		}

		size_t array_size() const
		{
			return m_node->Size();
		}

		operator bool() const
		{
			return m_node && !m_node->IsNull();
		}
	};

	class DeDocRapidJson : public DeNodeRapidJson
	{
		rapidjson::Document m_doc;

	public:
		DeDocRapidJson()
			: DeNodeRapidJson(&m_doc)
		{
		}

		template<class StrBuf>
		bool Parse(StrBuf& strJson)
		{
			if (std::is_const_v<StrBuf>)
			{
				m_doc.Parse(strJson.data(), strJson.size());
			}
			else
			{
				m_doc.ParseInsitu((char *)strJson.data());
			}

			return !m_doc.HasParseError();
		}

		bool Load(const char* strFilePath)
		{
			bool bRet = false;
			std::ifstream ifs(strFilePath);
			if (ifs)
			{
				rapidjson::IStreamWrapper isw(ifs);
				m_doc.ParseStream(isw);

				bRet = !m_doc.HasParseError();
			}

			return bRet;
		}
	};

	class SeDocRapidJson : public SeNodeRapidJson
	{
		rapidjson::Document m_doc;
	public:
		SeDocRapidJson()
			: SeNodeRapidJson(nullptr, nullptr)
		{
			m_node = &m_doc;
			m_allocator = &m_doc.GetAllocator();
		}

		template<class RsvBuffer>
		bool BuffDocWrite(RsvBuffer& buffer, unsigned int indent)
		{
			bool bRet = false;

			if (indent)
			{
				rapidjson::PrettyWriter<RsvBuffer> writer(buffer);
				writer.SetIndent(' ', indent);
				bRet = m_doc.Accept(writer);
			}
			else
			{
				rapidjson::Writer<RsvBuffer> writer(buffer);
				bRet = m_doc.Accept(writer);
			}

			return bRet;
		}

		template<class StrBuf = std::string>
		StrBuf Stringify(unsigned int indent = 0)
		{
			StrBuf strRet;
			typedef MSRPC::TBufferAdapter<StrBuf> RsvBuffer;
			RsvBuffer buffer(strRet);

			BuffDocWrite(buffer, indent);

			return strRet;
		}

		template<class StrBuf>
		void Stringify(StrBuf& buf, unsigned int indent = 0)
		{
			typedef MSRPC::TBufferAdapter<StrBuf> RsvBuffer;
			RsvBuffer buffer(strRet);

			BuffDocWrite(buffer, indent);
		}

		bool Save(const char* strFilePath, unsigned int indent = 0)
		{
			bool bRet = false;

			std::ofstream outfile(strFilePath);

			if (outfile)
			{
				rapidjson::OStreamWrapper osw(outfile);

				bRet = BuffDocWrite(osw, indent);
			}

			return bRet;
		}
	};
	
}

#endif // RAJSONNODE_H__
