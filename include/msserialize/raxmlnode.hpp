#ifndef RAXMLNODE_H__
#define RAXMLNODE_H__

#include "msarchive.hpp"
#include <rapidxml/rapidxml.hpp>
#include <sstream>


namespace MSRPC
{
	enum CtrlSign { CS_NODE, CS_ARRAY };

	class INodeXml
	{
	private:
		rapidxml::xml_node<>* m_node;
		rapidxml::xml_document<>* m_doc;
		CtrlSign m_csign;

		template <class T>
		static void ToString(std::string& str, const T& var)
		{
			std::ostringstream ss;
			ss << var;
			str = ss.str();
		}

	public:
		template <class T>
		void in_serialize(const T& tValue)
		{
			std::string strValue;
			ToString(strValue, tValue);
			m_node->value(m_doc->allocate_string(strValue.c_str(),
				strValue.size() + 1), strValue.size());
		}

		void in_serialize(const char* tValue)
		{
			m_node->value(tValue);
		}

		void in_serialize(char* tValue)
		{
			m_node->value(m_doc->allocate_string(tValue));
		}

		template <typename T>
		void in_serialize(const StrApt<T>& tValue)
		{
			in_serialize(tValue.Get());
		}

		INodeXml new_node()
		{
			return INodeXml(m_doc, m_doc->allocate_node(rapidxml::node_element));
		}

		void set_object()
		{
		}

		void add_member(const char* strName, INodeXml& vNode)
		{
			switch (vNode.m_csign)
			{
			case MSRPC::CS_NODE:
				vNode.m_node->name(strName);
				m_node->append_node(vNode.m_node);
				break;
			case MSRPC::CS_ARRAY:
				for(rapidxml::xml_node<char>* node = vNode.m_node->first_node(); 
					node; node = vNode.m_node->first_node())
				{
					vNode.m_node->remove_node(node);
					node->name(strName);
					m_node->append_node(node);
				}
				break;
			default:
				break;
			}
		}

		void set_array()
		{
			m_csign = CS_ARRAY;
		}

		void push_node(INodeXml& vNode)
		{
			m_node->append_node(vNode.m_node);
		}

		void finish()
		{
		}

	public:
		rapidxml::xml_node<>& data()
		{
			return *m_node;
		}

		INodeXml(rapidxml::xml_document<>* doc)
			: m_node(doc)
			, m_doc(doc)
			, m_csign(CS_NODE)
		{

		}

		INodeXml(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
			: m_node(node)
			, m_doc(doc)
			, m_csign(CS_NODE)
		{

		}
	};

	class ONodeXml
	{
	private:
		const rapidxml::xml_node<>* m_node;

		template <class T>
		static void ToValue(T& val, const char* str)
		{
			std::istringstream ss(str);
			ss >> val;
		}

	public:
		ONodeXml(const rapidxml::xml_node<>* node)
			: m_node(node) {}


	public:
		template <class T>
		void in_serialize(T& tValue) const
		{
			ToValue(tValue, m_node->value());
		}

		void in_serialize(const char*& tValue) const
		{
			tValue = m_node->value();
		}

		template <typename T>
		void in_serialize(const StrApt<T>& tValue) const
		{
			const_cast<StrApt<T>&>(tValue).
				Set(m_node->value(), m_node->value_size());
		}

		void in_serialize(char* tValue, size_t nSize) const
		{
			const char* str = m_node->value();
			
			if (nSize > m_node->value_size() + 1)
			{
				nSize = m_node->value_size() + 1;
			}

			memcpy(tValue, str, nSize);
		}

		ONodeXml sub_member(const char* strName) const
		{
			return ONodeXml(m_node->first_node(strName));
		}

		class ONodeArrIter
		{
		public:
			const rapidxml::xml_node<char>* m_curNode;
			const char* m_name;

			ONodeArrIter(const rapidxml::xml_node<char>* node, const char* name)
				: m_curNode(node)
				, m_name(name)
			{}

		public:
			ONodeXml operator *() const
			{
				return ONodeXml(m_curNode);
			}

			operator bool() const
			{
				return m_curNode;
			}

			ONodeArrIter& operator ++ ()
			{
				m_curNode = m_curNode->next_sibling(m_name);
				return *this;
			}
		};

		typedef ONodeArrIter ArrIter;

		ArrIter sub_nodes() const
		{
			return ArrIter(m_node, m_node->name());
		}

		operator bool() const
		{
			return m_node;
		}

	};

	typedef MSRPC::OArchiveHelper<MSRPC::ONodeXml> OXmlArc;
	typedef MSRPC::IArchiveHelper<MSRPC::INodeXml> IXmlArc;
}

#endif // RAXMLNODE_H__
