#pragma once

#include "msarchive.hpp"
#include "typeutils.hpp"
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <iostream>
#include <cstring>

namespace MSRPC
{
	class SeNodeCppYaml
	{
	protected:
		YAML::Node m_node;

	public:
		template <class T>
		void in_serialize(const T& tValue)
		{
			m_node = tValue;
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
				m_node = tValue;
			}
			else
			{
				
			}
		}

		void in_serialize(const double& tValue) 
		{
			if(is_valid_float(tValue))
			{
				m_node = tValue;
			}
			else
			{
			}
		}

		typedef SeNodeCppYaml& ObjApt;

		ObjApt set_object() 
		{
			if (!m_node.IsMap())
			{
				/* code */
				m_node = YAML::Node(YAML::NodeType::Map);
			}

			return *this;
		}

		SeNodeCppYaml add_member(const char* strName)
		{
			// YAML::Node node;
			// m_node.force_insert(strName, node);
			return SeNodeCppYaml(m_node[strName]);
		}

		typedef SeNodeCppYaml& ArrApt;
		ArrApt set_array() 
		{
			if(!m_node.IsSequence())
			{
				m_node = YAML::Node(YAML::NodeType::Sequence);
			}

			return *this;
		}

		SeNodeCppYaml add_element()
		{
			YAML::Node node;
			m_node.push_back(node);
			return SeNodeCppYaml(node);
		}

	public:
		YAML::Node& data()
		{
			return m_node;
		}

		operator bool() const
		{
			return !!m_node;
		}


		SeNodeCppYaml(const YAML::Node& node)
		: m_node(node)
		{

		}

		SeNodeCppYaml(const SeNodeCppYaml& other)
		: m_node(other.m_node)
		{

		}

		template <class T>
		void operator<<(const T &tValue)
		{
			Serializer<T>::serialize(*this, tValue);
		}
	};

	class DeNodeCppYaml
	{
	protected:
		YAML::Node m_node;

	public:
		/// is_number_integer
		template <class T>
		void in_serialize(T& tValue) const
		{
			if (m_node.IsScalar())
			{
				tValue = m_node.as<T>();
			}
			else
			{
				std::cerr << "Type mismatch in in_serialize\n" << std::endl;
			}
		}

		void in_serialize(char*& tValue) const
		{
			if (m_node.IsScalar())
			{
				std::string val = m_node.as<std::string>();
				tValue = new char[val.size() + 1];
				std::strncpy(tValue, val.c_str(), val.size() + 1);
				tValue[val.size()] = '\0';
			}
		}

		void in_serialize(const char*& tValue) const
		{
			if (m_node.IsScalar())
			{
				std::string val = m_node.as<std::string>();
				tValue = val.c_str();
			}
			else
			{
				std::cerr << "Type mismatch in in_serialize\n" << std::endl;
			}
		}

		template <typename T>
		void in_serialize(StrApt<T>& tValue) const
		{
			if (m_node.IsScalar())
			{
				std::string val = m_node.as<std::string>();
				tValue.Set(val.c_str(), val.size());
			}
		}

		void in_serialize(char* tValue, size_t nSize) const
		{
			if (m_node.IsScalar())
			{
				std::string val = m_node.as<std::string>();
				if (nSize > val.size() + 1)
				{
					nSize = val.size() + 1;
				}

				std::copy(val.c_str(), val.c_str() + nSize, tValue);
			}
		}

		class DeNodeObjIter
		{
			const YAML::Node* self;
			YAML::Node::const_iterator citCur;
			YAML::Node::const_iterator citEnd;
		public:

			DeNodeObjIter(const YAML::Node* node)
				: self(node)
				, citCur(node ? node->begin() : YAML::Node::const_iterator())
				, citEnd(node ? node->end() : YAML::Node::const_iterator())
			{}

		public:
			DeNodeCppYaml operator *() const
			{
				return DeNodeCppYaml(citCur->second);
			}

			DeNodeCppYaml find_member(const char *strName) const
			{
				return DeNodeCppYaml((*self)[strName]);
			}

			template <typename T>
			T key() const
			{
				T t = citCur->first.as<std::string>();
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
			return ObjIter(m_node.IsMap() ? &m_node : nullptr);
		}

		class DeNodeArrIter
		{
		public:
			const YAML::Node* self;
			YAML::Node::const_iterator citCur;
			YAML::Node::const_iterator citEnd;

			DeNodeArrIter(const YAML::Node* node)
				: self(node)
				, citCur(node ? node->begin() : YAML::Node::const_iterator())
				, citEnd(node ? node->end() : YAML::Node::const_iterator())
			{}

		public:
			DeNodeCppYaml operator *() const
			{
				return DeNodeCppYaml(*citCur);
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
			return ArrIter(m_node.IsSequence() ? &m_node : nullptr);
		}

		DeNodeCppYaml(const YAML::Node& node)
			: m_node(node)
		{}

		operator bool() const
		{
			return !!m_node;
		}

		template <class T>
		void operator>>(T &tValue)
		{
			Serializer<T>::deserialize(*this, tValue);
		}
	};

	class DeDocCppYaml : public DeNodeCppYaml
	{
		YAML::Node& m_doc;
	public:
		DeDocCppYaml()
			: DeNodeCppYaml(YAML::Node())
			, m_doc(m_node)
		{ }

		template<class StrBuf>
		bool Parse(StrBuf& strJson)
		{
			bool bRet = false;
			try
			{
				m_doc = YAML::Load(strJson);
				bRet = true;
			}
			catch (const YAML::Exception& e)
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
				m_doc = YAML::LoadFile(strFilePath);
				bRet = true;
			}
			catch (const YAML::Exception& e)
			{
				std::cerr << e.what() << std::endl;
			}

			return bRet;
		}
	};

	class SeDocCppYaml : public SeNodeCppYaml
	{
		YAML::Node& m_doc;
	public:
		SeDocCppYaml()
			: SeNodeCppYaml(YAML::Node())
			, m_doc(m_node)
		{ }

		template<class StrBuf = std::string>
		StrBuf Stringify(unsigned int indent = 0)
		{
			return YAML::Dump(m_doc);
			// YAML::Emitter out;
			// out << node;

			// // 将Emitter的内容转换为字符串
			// return out.c_str();
		}

		bool Save(const char* strFilePath, unsigned int indent = 0)
		{
			bool bRet = false;

			std::ofstream outfile(strFilePath);
			if (outfile)
			{
				outfile << m_doc;
				bRet = true;
			}

			return bRet;
		}


	};


}