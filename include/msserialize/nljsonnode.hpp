#ifndef NLJSONNODE_H__
#define NLJSONNODE_H__


#include "msarchive.hpp"

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

		void set_object() 
		{
			if (!m_node->is_object())
			{
				/* code */
				*m_node = nlohmann::json::object();
			}
		}

		SeNodeNlohmannJson add_member(const char* strName)
		{
			return SeNodeNlohmannJson(&*m_node->emplace(strName, nlohmann::json()).first);
		}

		void set_array() 
		{
			if(!m_node->is_array())
			{
				*m_node = nlohmann::json::array();
			}
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
		}

		void in_serialize(double &tValue) const
		{
			if(const double* val = m_node->get_ptr<const double*>())
			{
				tValue = *val;
			}
		}

		void in_serialize(char*& tValue) const
		{
			if(const std::string* val = m_node->get_ptr<const std::string*>())
			{
				tValue = new char[val->size() + 1];
				std::strncpy(tValue, val->c_str(), val->size() + 1);
				tValue[val->size()] = '\0';
			}
		}

		void in_serialize(const char*& tValue) const
		{
			if(const std::string* val = m_node->get_ptr<const std::string*>())
			{
				tValue = val->c_str();
			}
		}

		template <typename T>
		void in_serialize(StrApt<T>& tValue) const
		{
			if(const std::string* val = m_node->get_ptr<const std::string*>())
			{
				tValue.Set(val->c_str(), val->size());
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

				std::copy(val->c_str(), val->c_str() + nSize, tValue);
			}
		}

		DeNodeNlohmannJson sub_member(const char* strName) const
		{
			const nlohmann::json *node = nullptr;
			do 
			{
				auto itrFind = m_node->find(strName);
				if (itrFind == m_node->end())
				{
					break;
				}

				node = &*itrFind;

			} while (false);

			return DeNodeNlohmannJson(node);
		}

		class DeNodeObjIter
		{
		public:
			nlohmann::json::const_iterator citCur;
			nlohmann::json::const_iterator citEnd;

			DeNodeObjIter(const nlohmann::json *node)
				: citCur(node ? node->begin() : nlohmann::json::const_iterator())
				, citEnd(node ? node->end() : nlohmann::json::const_iterator())
			{}

		public:
			DeNodeNlohmannJson operator *() const
			{
				return DeNodeNlohmannJson(&*citCur);
			}

			const char* key() const
			{
				return citCur.key().c_str();
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
			return ObjIter(m_node);
		}


		class DeNodeArrIter
		{
		public:
			nlohmann::json::const_iterator citCur;
			nlohmann::json::const_iterator citEnd;

			DeNodeArrIter(const nlohmann::json *node)
				: citCur(node ? node->begin() : nlohmann::json::const_iterator())
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
		};

		typedef DeNodeArrIter ArrIter;

		ArrIter sub_elements() const
		{
			return ArrIter(m_node);
		}

		size_t array_size() const
		{
			return m_node ? m_node->size() : 0;
		}

		DeNodeNlohmannJson(const nlohmann::json *node)
			: m_node(node)
		{}

		operator bool() const
		{
			return m_node && !m_node->is_null();
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
