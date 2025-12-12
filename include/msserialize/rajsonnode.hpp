#ifndef RAJSONNODE_H__
#define RAJSONNODE_H__


#include "msarchive.hpp"
#include "typeutils.hpp"

#include <fstream>
#include <iostream>
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

		typedef SeNodeRapidJson &ObjApt;

		ObjApt set_object()
		{
			if (!m_node->IsObject())
			{
				m_node->SetObject();
			}

			return *this;
		}

		SeNodeRapidJson add_member(const char* strName)
		{
			rapidjson::Value node;
			m_node->AddMember(rapidjson::StringRef(strName), node, *m_allocator);
			return SeNodeRapidJson(&(m_node->MemberEnd() - 1)->value, m_allocator);
		}

		typedef SeNodeRapidJson &ArrApt;
		ArrApt set_array()
		{
			if (!m_node->IsArray())
			{
				m_node->SetArray();
			}

			return *this;
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

		template <class T>
		void operator<<(const T &tValue)
		{
			Serializer<T>::serialize(*this, tValue);
		}

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
			else if(m_node->IsString()) 
			{
				ToValue(tValue, m_node->GetString());
			}
			else 
			{
				std::cerr << "Error: DeNodeRapidJson::in_serialize(T& tValue) const type: " << m_node->GetType() << std::endl;
			}
		}

		void in_serialize(unsigned short& tValue) const
		{
			if(m_node->IsNumber())
			{
				tValue = static_cast<unsigned short>(
					m_node->Get<unsigned int>());
			}
			else if(m_node->IsString()) 
			{
				ToValue(tValue, m_node->GetString());
			}
			else 
			{
				std::cerr << "Error: DeNodeRapidJson::in_serialize(unsigned short& tValue) const type: " << m_node->GetType() << std::endl;
			}
		}

		void in_serialize(long long& tValue) const
		{
			if(m_node->IsNumber())
			{
				tValue = static_cast<long long>(
					m_node->Get<int64_t>());
			}
			else if(m_node->IsString()) 
			{
				ToValue(tValue, m_node->GetString());
			}
			else 
			{
				std::cerr << "Error: DeNodeRapidJson::in_serialize(unsigned short& tValue) const type: " << m_node->GetType() << std::endl;
			}
		}

		void in_serialize(unsigned long long& tValue) const
		{
			if(m_node->IsNumber())
			{
				tValue = static_cast<unsigned long long>(
					m_node->Get<uint64_t>());
			}
			else if(m_node->IsString()) 
			{
				ToValue(tValue, m_node->GetString());
			}
			else 
			{
				std::cerr << "Error: DeNodeRapidJson::in_serialize(unsigned short& tValue) const type: " << m_node->GetType() << std::endl;
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
			else if(m_node->IsString()) 
			{
				ToValue(tValue, m_node->GetString());
			}
			else 
			{
				std::cerr << "Error: DeNodeRapidJson::in_serialize(unsigned short& tValue) const type: " << m_node->GetType() << std::endl;
			}
		}

		static void NodeToString(
			const rapidjson::Value* node,
			char* strValue,
			size_t sSize)
		{
			if (node->IsInt())
			{
				std::snprintf(strValue, sSize, "%d", node->Get<int32_t>());
			}
			else if (node->IsUint())
			{
				std::snprintf(strValue, sSize, "%u", node->Get<uint32_t>());
			}
			else if (node->IsInt64())
			{
#ifdef _WIN32
				std::snprintf(strValue, sSize, "%lld", node->Get<int64_t>());
#else
				std::snprintf(strValue, sSize, "%ld", node->Get<int64_t>());
#endif
			}
			else if (node->IsUint64())
			{
#ifdef _WIN32
				std::snprintf(strValue, sSize, "%llu", node->Get<uint64_t>());
#else
				std::snprintf(strValue, sSize, "%lu", node->Get<uint64_t>());
#endif
			}
			else if (node->IsDouble())
			{
				std::snprintf(strValue, sSize, "%lf", node->Get<double>());
			}
			else if (node->IsFloat())
			{
				std::snprintf(strValue, sSize, "%f", node->Get<float>());
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
			else if(m_node->IsNumber())
			{
				#define BUFFER_SIZE 32
				tValue = new char[BUFFER_SIZE];
				NodeToString(m_node, tValue, BUFFER_SIZE);
				#undef BUFFER_SIZE
			}
			else if(m_node->IsBool())
			{
				#define BUFFER_SIZE 8 
				tValue = new char[BUFFER_SIZE];
				std::snprintf(tValue, BUFFER_SIZE, "%s", m_node->GetBool() ? "true" : "false");
				#undef BUFFER_SIZE
			}
		}

		void in_serialize(const char*& tValue) const
		{
			if(m_node->IsString())
			{
				tValue = m_node->GetString();
			}
			else if(m_node->IsBool())
			{
				tValue = m_node->GetBool() ? "true" : "false";
			}
			else
			{
				std::cerr << "Error: DeNodeRapidJson::in_serialize(const char*& tValue) const type: " << m_node->GetType() << std::endl;
			}
		}

		template <typename T>
		void in_serialize(StrApt<T>& tValue) const
		{
			if(m_node->IsString())
			{
				tValue.Set(m_node->GetString(), m_node->GetStringLength());
			}
			else if(m_node->IsNumber())
			{
				#define BUFFER_SIZE 32
				auto strValue = new char[BUFFER_SIZE];
				NodeToString(m_node, strValue, BUFFER_SIZE);
				#undef BUFFER_SIZE
				tValue.Set(strValue, strlen(strValue));
				delete[] strValue;
			}
			else if(m_node->IsBool())
			{ 
				const char* str = m_node->GetBool() ? "true" : "false";
				tValue.Set(str, strlen(str));
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
			else if(m_node->IsNumber())
			{
				NodeToString(m_node, tValue, nSize);
			}
			else if(m_node->IsBool())
			{ 
				std::snprintf(tValue, nSize, "%s", m_node->GetBool() ? "true" : "false");
			}
		}

		class DeNodeObjIter
		{
			const rapidjson::Value* self;
			rapidjson::Value::ConstMemberIterator citCur;
			rapidjson::Value::ConstMemberIterator citEnd;
		public:

			DeNodeObjIter(const rapidjson::Value* node)
				: self(node)
				, citCur(node ? node->MemberBegin() : rapidjson::Value::ConstMemberIterator())
				, citEnd(node ? node->MemberEnd() : rapidjson::Value::ConstMemberIterator())
			{}

			DeNodeRapidJson find_member(const char *strName) const
			{
				rapidjson::Value::ConstMemberIterator itrFind = self->FindMember(strName);
				return DeNodeRapidJson(itrFind != self->MemberEnd() ? &itrFind->value : nullptr);
			}

			DeNodeRapidJson operator *() const
			{
				return DeNodeRapidJson(&citCur->value);
			}

			template <typename T>
			T key() const
			{
				T t = citCur->name.GetString();
				return t;
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
			const rapidjson::Value* self;
			rapidjson::Value::ConstValueIterator citCur;
			rapidjson::Value::ConstValueIterator citEnd;

			DeNodeArrIter(const rapidjson::Value* node)
				: self(node)
				, citCur(node ? node->Begin() : rapidjson::Value::ConstValueIterator())
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

			size_t array_size() const
			{
				return self->Size();
			}
		};

		typedef DeNodeArrIter ArrIter;

		ArrIter sub_elements() const
		{
			return ArrIter(m_node && m_node->IsArray() ? m_node : nullptr);
		}

		operator bool() const
		{
			return m_node && !m_node->IsNull();
		}

		template <class T>
		void operator>>(T &tValue)
		{
			Serializer<T>::deserialize(*this, tValue);
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
			RsvBuffer buffer(buf);

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
