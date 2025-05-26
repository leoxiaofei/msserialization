#ifndef POJSONNODE_H__
#define POJSONNODE_H__


#include "msarchive.hpp"

#include <Poco/JSON/Parser.h>
#include <Poco/JSON/ParseHandler.h>
#include <Poco/JSON/JSONException.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Query.h>
#include <Poco/JSON/PrintHandler.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <algorithm>

namespace MSRPC
{
	class INodeJson
	{
	protected:
		Poco::Dynamic::Var m_data;
		Poco::Dynamic::Var* m_node;

	public:
		template <class T>
		void in_serialize(const T& tValue)
		{
			*m_node = tValue;
		}

		void in_serialize(const char* tValue)
		{
			*m_node = tValue;
		}

		// void in_serialize(char* tValue)
		// {
		// 	m_node->SetString(tValue, *m_allocator);
		// }

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

		void in_serialize(const double& tValue)
		{
			if (!std::isnan(tValue))
			{
				*m_node = tValue;
			}
		}

		void in_serialize(const float& tValue)
		{
			if (!std::isnan(tValue))
			{
				*m_node = tValue;
			}
		}

		INodeJson new_node()
		{
			return INodeJson();
		}

		void set_object()
		{
			*m_node = Poco::JSON::Object::Ptr(new Poco::JSON::Object);
		}

		void add_member(const char* strName, INodeJson& vNode)
		{
			Poco::JSON::Object::Ptr ptr = m_node->extract<Poco::JSON::Object::Ptr>();
			ptr->set(strName, *vNode.m_node);
		}

		void set_array()
		{
			*m_node = Poco::makeShared<Poco::JSON::Array>();
		}

		void push_node(INodeJson& vNode)
		{
			Poco::JSON::Array::Ptr ptr = m_node->extract<Poco::JSON::Array::Ptr>();

			ptr->add(*vNode.m_node);
		}

	public:
		Poco::Dynamic::Var& data()
		{
			return *m_node;
		}

		operator bool() const
		{
			return !m_node->isEmpty();
		}

		INodeJson()
			: m_node(&m_data)
		{

		}

		INodeJson(Poco::Dynamic::Var* doc)
			: m_node(doc)
		{

		}

		INodeJson(INodeJson&& other)
			: m_node(&m_data)
		{
			if (other.m_node == &other.m_data)
			{
				m_data.swap(other.m_data);
			}
			else
			{
				m_node = other.m_node;
			}
		}
	};

	class ONodeJson
	{
	protected:
		Poco::Dynamic::Var m_data;
		const Poco::Dynamic::Var* m_node;

	public:
		ONodeJson()
			: m_node(&m_data)
		{
		}

		ONodeJson(Poco::Dynamic::Var&& node)
			: m_node(&m_data)
		{
			m_data.swap(node);
		}

		ONodeJson(const Poco::Dynamic::Var* node)
			: m_node(node)
		{
		}

	public:
		template <class T>
		void in_serialize(T& tValue) const
		{
			m_node->convert<T>(tValue);
		}

		template <typename T>
		void in_serialize(StrApt<T>& tValue) const
		{
			if (m_node->isString())
			{
				const auto& str = m_node->extract<std::string>();
				tValue.Set(str.c_str(), str.size());
			}
			else if (m_node->isNumeric())
			{
				std::string str;
				m_node->convert<std::string>(str);
				tValue.Set(str.data(), str.size());
			}
			else
			{
				std::cerr << "in_serialize type is not matched!" << std::endl;
			}
		}

		void in_serialize(char* tValue, size_t nSize) const
		{
			if (m_node->isString())
			{
				const auto& str = m_node->extract<std::string>();
				nSize = std::min<size_t>(nSize - 1, str.size());
				memcpy(tValue, str.c_str(), nSize);
				tValue[nSize] = '\0';
			}
			else if (m_node->isNumeric())
			{
				std::string str;
				m_node->convert<std::string>(str);
				nSize = std::min<size_t>(nSize - 1, str.size());
				memcpy(tValue, str.c_str(), nSize);
				tValue[nSize] = '\0';
			}
			else
			{
				std::cerr << "in_serialize type is not matched!" << std::endl;
			}
		}

		ONodeJson sub_member(const char* strName) const
		{
			try
			{
				Poco::JSON::Object::Ptr str = m_node->extract<Poco::JSON::Object::Ptr>();

				return ONodeJson(str->get(strName));
			}
			catch (...)
			{

			}

			return ONodeJson();
		}

		class ONodeObjIter
		{
		public:
			Poco::JSON::Object::ConstIterator citCur;
			Poco::JSON::Object::ConstIterator citEnd;

			ONodeObjIter()
			{
			}

			ONodeObjIter(const Poco::JSON::Object::Ptr& node)
				: citCur(node->begin())
				, citEnd(node->end())
			{
			}

		public:
			ONodeJson operator *() const
			{
				return ONodeJson(&citCur->second);
			}

			const char* key() const
			{
				return citCur->first.c_str();
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
			try
			{
				Poco::JSON::Object::Ptr obj = m_node->extract<Poco::JSON::Object::Ptr>();

				return ObjIter(obj);
			}
			catch (...)
			{

			}

			return ObjIter();
		}

		class ONodeArrIter
		{
		public:
			Poco::JSON::Array::ConstIterator citCur;
			Poco::JSON::Array::ConstIterator citEnd;

			ONodeArrIter()
			{
			}

			ONodeArrIter(const Poco::JSON::Array::Ptr& node)
				: citCur(node->begin())
				, citEnd(node->end())
			{
			}

		public:
			ONodeJson operator *() const
			{
				return ONodeJson(&*citCur);
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
			try
			{
				Poco::JSON::Array::Ptr obj = m_node->extract<Poco::JSON::Array::Ptr>();

				return ArrIter(obj);
			}
			catch (const std::exception& e)
			{
				std::cerr << "extract<Poco::JSON::Array::Ptr>: " << e.what() << std::endl;
			}
			catch (...)
			{
				std::cerr << "extract<Poco::JSON::Array::Ptr>" << std::endl;
			}

			return ArrIter();
		}

		operator bool() const
		{
			return !m_node->isEmpty();
		}
	};

	typedef OArchiveHelper<ONodeJson> OJsonArc;
	typedef IArchiveHelper<INodeJson> IJsonArc;

	class MemBuf : public std::streambuf
	{
	public:
		MemBuf(const char* data, size_t size) {
			char* p = const_cast<char*>(data);
			setg(p, p, p + size);
		}

	protected:
		int_type underflow() override {
			if (gptr() >= egptr()) {
				return traits_type::eof();
			}
			return traits_type::to_int_type(*gptr());
		}
	};

	template<class StrBuf>
	class BufferApt
	{
		MemBuf memBuf;
		std::istream ist;
	public:
		BufferApt(const StrBuf& t)
		: memBuf(t.begin(), t.size())
		, ist(&memBuf)
		{}
		
		operator std::istream& ()
		{
			return ist;
		}
	};

	template<>
	class BufferApt<std::string>
	{
		std::string& str;
	public:
		BufferApt(std::string& s)
			: str(s)
		{}
		operator std::string& ()
		{
			return str;
		}
	};

	class ONodeDoc : public ONodeJson
	{
	public:
		template<class StrBuf>
		bool Parse(StrBuf& strJson)
		{
			bool bRet = false;
			try
			{
				Poco::JSON::Parser parser;
				BufferApt<StrBuf> apt(strJson);
				parser.parse(apt).swap(m_data);
				bRet = !m_data.isEmpty();
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
					Poco::JSON::Parser parser;
					parser.parse(ifs).swap(m_data);
					bRet = !m_data.isEmpty();
				}
			}
			catch (const std::exception&)
			{

			}

			return bRet;
		}
	};

	class INodeDoc : public INodeJson
	{
	public:
		std::string Stringify(unsigned int indent = 0)
		{
			std::ostringstream os;

			Poco::JSON::Stringifier::stringify(m_data, os, indent, -1, Poco::JSON_WRAP_STRINGS);

			return os.str();
		}

		template<class StrBuf>
		void Stringify(StrBuf& buf, unsigned int indent = 0)
		{
			Poco::JSON::Stringifier::stringify(m_data, buf, indent, -1, Poco::JSON_WRAP_STRINGS);
		}

		bool Save(const char* strFilePath, unsigned int indent = 0)
		{
			bool bRet = false;

			std::ofstream outfile(strFilePath);
			if (outfile)
			{
				Poco::JSON::Stringifier::stringify(m_data, outfile, indent, 1, Poco::JSON_WRAP_STRINGS);
				bRet = true;
			}

			return bRet;
		}
	};

}

#endif // POJSONNODE_H__
