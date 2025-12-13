#ifndef NLJSONNODE_H__
#define NLJSONNODE_H__


#include "msarchive.hpp"
#include "typeutils.hpp"

#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

namespace MSRPC
{
	class SeNodeNlohmannJson
	{
	private:
		nlohmann::json* m_node;

	public:
		template <class T>
		void in_serialize(const T& tValue)
		{
			*m_node = tValue;
		}

		template <typename T>
		void in_serialize(const StrApt<T>& tValue)
		{
			in_serialize(tValue.Get());
		}

		void in_serialize(const float& tValue)
		{
			if(is_valid_float(tValue))
			{
				*m_node = tValue;
			}
			else
			{
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
			}
		}

		typedef SeNodeNlohmannJson& ObjApt;
		ObjApt set_object() 
		{
			if (!m_node->is_object())
			{
				/* code */
				*m_node = nlohmann::json::object();
			}

			return *this;
		}

		SeNodeNlohmannJson add_member(const char* strName)
		{
			return SeNodeNlohmannJson(&*m_node->emplace(strName, nlohmann::json()).first);
		}

		typedef SeNodeNlohmannJson& ArrApt;
		ArrApt set_array() 
		{
			if(!m_node->is_array())
			{
				*m_node = nlohmann::json::array();
			}

			return *this;
		}

		SeNodeNlohmannJson add_element()
		{
			return SeNodeNlohmannJson(&m_node->emplace_back(nlohmann::json()));
		}

	public:
		nlohmann::json& data()
		{
			return *m_node;
		}

		operator bool() const
		{
			return m_node && !m_node->is_null();
		}


		SeNodeNlohmannJson(nlohmann::json* node)
		: m_node(node)
		{

		}

		SeNodeNlohmannJson(const SeNodeNlohmannJson& other)
		: m_node(other.m_node)
		{

		}

		template <class T>
		void operator<<(const T &tValue)
		{
			Serializer<T>::serialize(*this, tValue);
		}
	};

	class DeNodeNlohmannJson
	{
	private:
		const nlohmann::json* m_node;

	public:
		/// is_number_integer
		template <class T>
		void in_serialize(T& tValue) const
		{
			if (m_node->is_number_integer())
			{
				tValue = m_node->get<T>();
			}
			else if(const std::string* val = m_node->get_ptr<const std::string*>())
			{
				ToValue(tValue, *val);
			}
			else
			{
				std::cerr << "Type mismatch in in_serialize\n" << std::endl;
			}
		}

		void in_serialize(bool &tValue) const
		{
			if(const bool* val = m_node->get_ptr<const bool*>())
			{
				tValue = *val;
			}
			else if(const std::string* val = m_node->get_ptr<const std::string*>())
			{
				ToValue(tValue, *val);
			}
		}

		void in_serialize(double &tValue) const
		{
			if(const double* val = m_node->get_ptr<const double*>())
			{
				tValue = *val;
			}
			else if(m_node->is_null())
			{
				tValue = NAN;
			}
			else if(const std::string* val = m_node->get_ptr<const std::string*>())
			{
				ToValue(tValue, *val);
			}
		}

		static void NodeToString(const nlohmann::json *node, char *strValue, size_t sSize)
		{
			if (const int64_t* val = node->get_ptr<const int64_t*>())
			{
#ifdef _WIN32
				std::snprintf(strValue, sSize, "%lld", *val);
#else
				std::snprintf(strValue, sSize, "%ld", *val);
#endif
			}
			else if (const uint64_t* val = node->get_ptr<const uint64_t*>())
			{
#ifdef _WIN32
				std::snprintf(strValue, sSize, "%llu", *val);
#else
				std::snprintf(strValue, sSize, "%lu", *val);
#endif
			}
			else if (const double* val = node->get_ptr<const double*>())
			{
				std::snprintf(strValue, sSize, "%lf", *val);
			}
		}

		void in_serialize(char*& tValue) const
		{
			if(const std::string* val = m_node->get_ptr<const std::string*>())
			{
				size_t nSize = val->size() + 1;
				tValue = new char[nSize];
				std::strncpy(tValue, val->c_str(), nSize);
			}
			else if(m_node->is_number())
			{
				size_t nSize = 32;
				tValue = new char[nSize];
				NodeToString(m_node, tValue, nSize);
			}
			else if(const bool* val = m_node->get_ptr<const bool*>())
			{
				size_t nSize = 8;
				tValue = new char[nSize];
				std::snprintf(tValue, nSize, "%s", *val ? "true" : "false");
			}
		}

		void in_serialize(const char*& tValue) const
		{
			if(const std::string* val = m_node->get_ptr<const std::string*>())
			{
				tValue = val->c_str();
			}
			else if(const bool* val = m_node->get_ptr<const bool*>())
			{
				tValue = *val ? "true" : "false";
			}
		}

		template <typename T>
		void in_serialize(StrApt<T>& tValue) const
		{
			if(const std::string* val = m_node->get_ptr<const std::string*>())
			{
				tValue.Set(val->c_str(), val->size());
			}
			else if (m_node->is_number())
			{
				size_t nSize = 32;
				auto strValue = new char[nSize];
				NodeToString(m_node, strValue, nSize);
				tValue.Set(strValue, strlen(strValue));
				delete[] strValue;
			}
			else if(const bool* val = m_node->get_ptr<const bool*>())
			{ 
				const char* str = *val ? "true" : "false";
				tValue.Set(str, strlen(str));
			}
		}

		void in_serialize(char* tValue, size_t nSize) const
		{
			if(const std::string* val = m_node->get_ptr<const std::string*>())
			{
				if (nSize > val->size() + 1)
				{
					nSize = val->size() + 1;
				}
				else
				{
					tValue[nSize - 1] = '\0';
					nSize = nSize - 1;
				}

				std::strncpy(tValue, val->c_str(), nSize);
			}
			else if(m_node->is_number())
			{
				NodeToString(m_node, tValue, nSize);
			}
			else if(const bool* val = m_node->get_ptr<const bool*>())
			{ 
				std::snprintf(tValue, nSize, "%s", *val ? "true" : "false");
			}
		}

		class DeNodeObjIter
		{
			const nlohmann::json *self;
			nlohmann::json::const_iterator citCur;
			nlohmann::json::const_iterator citEnd;
			
		public:
			DeNodeObjIter(const nlohmann::json *node)
				: self(node)
				, citCur(node ? node->begin() : nlohmann::json::const_iterator())
				, citEnd(node ? node->end() : nlohmann::json::const_iterator())
			{}

			DeNodeNlohmannJson find_member(const char* strName) const
			{
				auto itrFind = self->find(strName);
				return DeNodeNlohmannJson(itrFind != self->end() ? &*itrFind : nullptr);
			}

			DeNodeNlohmannJson operator *() const
			{
				return DeNodeNlohmannJson(&*citCur);
			}

			template <typename T>
			T key() const
			{
				T t = citCur.key();
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
			return ObjIter((m_node && m_node->is_object()) ? m_node : nullptr);
		}

		class DeNodeArrIter
		{
		public:
			const nlohmann::json *self;
			nlohmann::json::const_iterator citCur;
			nlohmann::json::const_iterator citEnd;

			DeNodeArrIter(const nlohmann::json *node)
				: self(node)
				, citCur(node ? node->begin() : nlohmann::json::const_iterator())
				, citEnd(node ? node->end() : nlohmann::json::const_iterator())
			{}

		public:
			DeNodeNlohmannJson operator *() const
			{
				return DeNodeNlohmannJson(&*citCur);
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
				return self ? self->size() : 0;
			}
		};

		typedef DeNodeArrIter ArrIter;

		ArrIter sub_elements() const
		{
			return ArrIter(m_node && m_node->is_array() ? m_node : nullptr);
		}

		DeNodeNlohmannJson(const nlohmann::json *node)
			: m_node(node)
		{}

		operator bool() const
		{
			return m_node && !m_node->is_null();
		}

		template <class T>
		void operator>>(T &tValue)
		{
			Serializer<T>::deserialize(*this, tValue);
		}
	};

	class DeDocNlohmannJson : public DeNodeNlohmannJson
	{
		nlohmann::json m_doc;
	public:
		DeDocNlohmannJson()
			: DeNodeNlohmannJson(&m_doc)
		{ }

		template<class StrBuf>
		bool Parse(StrBuf& strJson)
		{
			bool bRet = false;
			try
			{
				m_doc = nlohmann::json::parse(strJson);
				bRet = true;
			}
			catch (const nlohmann::json::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}

			return bRet;
		}

		bool Load(const char* strFilePath)
		{
			bool bRet = false;
			try
			{
				std::ifstream ifs(strFilePath);
				if(ifs)
				{
					m_doc = nlohmann::json::parse(ifs);
					bRet = true;
				}
			}
			catch (const nlohmann::json::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}

			return bRet;
		}
	};

	class SeDocNlohmannJson : public SeNodeNlohmannJson
	{
		nlohmann::json m_doc;
	public:
		SeDocNlohmannJson()
			: SeNodeNlohmannJson(&m_doc)
		{ }

		template<class StrBuf = std::string>
		StrBuf Stringify(unsigned int indent = 0)
		{
			return m_doc.dump(indent > 0 ? indent : -1);
		}

		bool Save(const char* strFilePath, unsigned int indent = 0)
		{
			bool bRet = false;

			std::ofstream outfile(strFilePath);
			if (outfile)
			{
				outfile << std::setw(indent) << m_doc;
				bRet = true;
			}

			return bRet;
		}
	};


}

#endif // NLJSONNODE_H__
