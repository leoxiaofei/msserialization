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

#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>

namespace MSRPC
{
	class INodeJson
	{
	private:
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
			*m_node = Poco::JSON::Array::Ptr(new Poco::JSON::Array);
		}

		void push_node(INodeJson& vNode)
		{
			Poco::JSON::Array::Ptr ptr = m_node->extract<Poco::JSON::Array::Ptr>();

			ptr->add(*vNode.m_node);
		}

		void finish()
		{
		}

	public:
		Poco::Dynamic::Var& data()
		{
			return *m_node;
		}

		operator bool() const
		{
			return m_node && !m_node->isEmpty();
		}

		INodeJson()
			: m_node(&m_data)
		{

		}

		INodeJson(Poco::Dynamic::Var* doc)
			: m_node(doc)
		{

		}

		INodeJson(const INodeJson& other)
			: m_node(other.m_node == &other.m_data ? &m_data : other.m_node)
		{

		}

	};

	class ONodeJson
	{
	public:
		Poco::Dynamic::Var m_node;

		ONodeJson() 
		{}

		ONodeJson(Poco::Dynamic::Var&& node)
			: m_node(std::move(node))
		{}

		ONodeJson(const Poco::Dynamic::Var* node)
			: m_node(*node)
		{}

	public:
		template <class T>
		void in_serialize(T& tValue) const
		{
			m_node.convert<T>(tValue);
			//if (m_node->Is<T>())
			//{
			//	tValue = m_node->Get<T>();
			//}
			//else if (std::is_arithmetic<T>() && m_node->IsNumber())
			//{
			//	if (m_node->IsInt())
			//	{
			//		tValue = m_node->GetInt();
			//	}
			//	else if (m_node->IsUint())
			//	{
			//		tValue = m_node->GetUint();
			//	}
			//	else if (m_node->IsDouble())
			//	{
			//		tValue = static_cast<T>(m_node->GetDouble());
			//	}
			//}
			//else if (std::is_arithmetic<T>() && m_node->IsString())
			//{
			//	std::stringstream ss;
			//	ss << m_node->GetString();
			//	ss >> tValue;
			//}
			//else
			//{
			//	std::cerr << "in_serialize type is not matched!" << std::endl;
			//}
		}

		//void in_serialize(bool& tValue) const
		//{
		//	if (m_node->Is<bool>())
		//	{
		//		tValue = m_node->Get<bool>();
		//	}
		//	else if (m_node->IsNumber())
		//	{
		//		tValue = m_node->GetInt() != 0;
		//	}
		//	else if (m_node->IsString())
		//	{
		//		std::stringstream ss;
		//		ss << m_node->GetString();
		//		ss >> tValue;
		//	}
		//	else
		//	{
		//		std::cerr << "in_serialize type is not matched!" << std::endl;
		//	}
		//}

		//void in_serialize(double& tValue) const
		//{
		//	if (m_node->IsNumber())
		//	{
		//		tValue = m_node->Get<double>();
		//	}
		//	else if (m_node->IsString())
		//	{
		//		std::stringstream ss;
		//		ss << m_node->GetString();
		//		ss >> tValue;
		//	}
		//	else if (m_node->IsNull())
		//	{
		//		tValue = NAN;
		//	}
		//	else
		//	{
		//		std::cerr << "in_serialize type is not matched!" << std::endl;
		//	}
		//}

		//void in_serialize(float& tValue) const
		//{
		//	if (m_node->IsNumber())
		//	{
		//		tValue = m_node->Get<float>();
		//	}
		//	else if (m_node->IsString())
		//	{
		//		std::stringstream ss;
		//		ss << m_node->GetString();
		//		ss >> tValue;
		//	}
		//	else if (m_node->IsNull())
		//	{
		//		tValue = NAN;
		//	}
		//	else
		//	{
		//		std::cerr << "in_serialize type is not matched!" << std::endl;
		//	}
		//}

		//void in_serialize(unsigned short& tValue) const
		//{
		//	if (m_node->Is<unsigned int>())
		//	{
		//		tValue = static_cast<unsigned short>(
		//			m_node->Get<unsigned int>());
		//	}
		//	else if (m_node->IsString())
		//	{
		//		std::stringstream ss;
		//		ss << m_node->GetString();
		//		ss >> tValue;
		//	}
		//	else
		//	{
		//		std::cerr << "in_serialize type is not matched!" << std::endl;
		//	}

		//}

		//void in_serialize(long long& tValue) const
		//{
		//	if (m_node->Is<int64_t>())
		//	{
		//		tValue = static_cast<long long>(
		//			m_node->Get<int64_t>());
		//	}
		//	else if (m_node->IsString())
		//	{
		//		std::stringstream ss;
		//		ss << m_node->GetString();
		//		ss >> tValue;
		//	}
		//	else
		//	{
		//		std::cerr << "in_serialize type is not matched!" << std::endl;
		//	}
		//}

		//void in_serialize(unsigned long long& tValue) const
		//{
		//	if (m_node->Is<uint64_t>())
		//	{
		//		tValue = static_cast<unsigned long long>(
		//			m_node->Get<uint64_t>());
		//	}
		//	else if (m_node->IsString())
		//	{
		//		std::stringstream ss;
		//		ss << m_node->GetString();
		//		ss >> tValue;
		//	}
		//	else
		//	{
		//		std::cerr << "in_serialize type is not matched!" << std::endl;
		//	}
		//}

		//void in_serialize(const char*& tValue) const
		//{
		//	if (m_node->IsString())
		//	{
		//		tValue = m_node->GetString();
		//	}
		//	else
		//	{
		//		std::cerr << "in_serialize type is not matched!" << std::endl;
		//	}
		//}

		template <typename T>
		void in_serialize(StrApt<T>& tValue) const
		{
			if (m_node.isString())
			{
				const auto& str = m_node.extract<std::string>();
				tValue.Set(str.c_str(), str.size());
			}
			else if (m_node.isNumeric())
			{
				std::string str;
				m_node.convert<std::string>(str);
				tValue.Set(str.data(), str.size());
			}
			else
			{
				std::cerr << "in_serialize type is not matched!" << std::endl;
			}
		}

		void in_serialize(char* tValue, size_t nSize) const
		{
			if (m_node.isString())
			{
				const auto& str = m_node.extract<std::string>();
				nSize = std::min<size_t>(nSize - 1, str.size());
				memcpy(tValue, str.c_str(), nSize);
				tValue[nSize + 1] = '\0';
			}
			else if (m_node.isNumeric())
			{
				std::string str;
				m_node.convert<std::string>(str);
				nSize = std::min<size_t>(nSize - 1, str.size());
				memcpy(tValue, str.c_str(), nSize);
				tValue[nSize + 1] = '\0';
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
				Poco::JSON::Object::Ptr str = m_node.extract<Poco::JSON::Object::Ptr>();

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
			{}

			ONodeObjIter(const Poco::JSON::Object::Ptr& node)
				: citCur(node->begin())
				, citEnd(node->end())
			{}

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
				Poco::JSON::Object::Ptr obj = m_node.extract<Poco::JSON::Object::Ptr>();

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
			{}

			ONodeArrIter(const Poco::JSON::Array::Ptr& node)
				: citCur(node->begin())
				, citEnd(node->end())
			{}

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
				Poco::JSON::Array::Ptr obj = m_node.extract<Poco::JSON::Array::Ptr>();

				return ArrIter(obj);
			}
			catch (...)
			{

			}

			return ArrIter();
		}

		operator bool() const
		{
			return m_node && !m_node.isEmpty();
		}
	};

	typedef MSRPC::OArchiveHelper<MSRPC::ONodeJson> OJsonArc;
	typedef MSRPC::IArchiveHelper<MSRPC::INodeJson> IJsonArc;
}

#endif // POJSONNODE_H__
