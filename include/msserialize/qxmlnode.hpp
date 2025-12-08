#ifndef QXMLNODE_H__
#define QXMLNODE_H__

#include "msarchive.hpp"
#include <sstream>
#include <QDomDocument>

#define NODE_WITH_QT 1

namespace MSRPC
{
	enum CtrlSign { CS_NODE, CS_ARRAY };

	class SeNodeXml
	{
	private:
		QDomElement m_node;
		QDomDocument* m_doc;
		CtrlSign m_csign;

	public:
		template <class T>
		void in_serialize(const T& tValue)
		{
			m_node.appendChild(m_doc->createTextNode(QString::number(tValue)));
		}

		void in_serialize(const QString& tValue)
		{
			m_node.appendChild(m_doc->createTextNode(tValue));
		}

		void in_serialize(const char* tValue)
		{
			m_node.appendChild(m_doc->createTextNode(QString::fromUtf8(tValue)));
		}

		template <typename T>
		void in_serialize(const StrApt<T>& tValue)
		{
			in_serialize(tValue.Get());
		}

		SeNodeXml add_element()
		{
			return SeNodeXml(m_doc, m_doc->createElement("A"));
		}

		void set_object()
		{
			m_csign = CS_NODE;
		}

		void add_member(const char* strName, SeNodeXml& vNode)
		{
			switch (vNode.m_csign)
			{
			case MSRPC::CS_NODE:
				vNode.m_node.setTagName(QString::fromUtf8(strName));
				m_node.appendChild(vNode.m_node);
				break;
			case MSRPC::CS_ARRAY:
			{
				while (vNode.m_node.hasChildNodes())
				{
					QDomElement subNode = vNode.m_node.firstChild().toElement();
					subNode.setTagName(QString::fromUtf8(strName));
					m_node.appendChild(subNode);
				}
				break;
			}
			default:
				break;
			}
		}

		void set_array()
		{
			m_csign = CS_ARRAY;
		}

		void push_node(SeNodeXml& vNode)
		{
			m_node.appendChild(vNode.m_node);
		}

		void finish()
		{
			m_doc->appendChild(m_node);
		}

	public:
		QDomElement& data()
		{
			return m_node;
		}

		SeNodeXml(QDomDocument* doc, const QString& strRootName)
			: m_node(doc->createElement(strRootName))
			, m_doc(doc)
			, m_csign(CS_NODE)
		{
		}

		SeNodeXml(QDomDocument* doc, const QDomElement& node)
			: m_node(node)
			, m_doc(doc)
			, m_csign(CS_NODE)
		{

		}

	};

	class DeNodeXml
	{
	private:
		QDomElement m_node;

	public:
		DeNodeXml(const QDomElement& node)
			: m_node(node) {}

		DeNodeXml(const QDomDocument* doc)
			: m_node(doc->firstChildElement()) {}


	public:
		template <class T>
		void in_serialize(T& tValue) const
		{
			tValue = QVariant(m_node.text()).value<T>();
		}

		//void in_serialize(const char*& tValue) const
		//{
		//	m_node.text()
		//	tValue = m_node->value();
		//}

		void in_serialize(QString& tValue) const
		{
			tValue = m_node.text();
		}

		template <typename T>
		void in_serialize(const StrApt<T>& tValue) const
		{
			QByteArray baText = m_node.text().toUtf8();
			const_cast<StrApt<T>&>(tValue).Set(baText.data(), baText.size());
		}

		void in_serialize(char* tValue, size_t nSize) const
		{
			QByteArray baText = m_node.text().toUtf8();

			const char* str = baText.data();
			
			if (nSize > baText.size() + 1)
			{
				nSize = baText.size() + 1;
			}

			memcpy(tValue, str, nSize);
		}

		DeNodeXml sub_member(const char* strName) const
		{
			QDomElement elem = m_node.firstChildElement(
				QString::fromUtf8(strName));
			return DeNodeXml(elem);
		}

		class DeNodeArrIter
		{
		public:
			QDomElement m_curNode;
			QString m_name;

			DeNodeArrIter(const QDomElement& node, const QString& name)
				: m_curNode(node)
				, m_name(name)
			{}

		public:
			DeNodeXml operator *() const
			{
				return DeNodeXml(m_curNode);
			}

			operator bool() const
			{
				return !m_curNode.isNull();
			}

			DeNodeArrIter& operator ++ ()
			{
				m_curNode = m_curNode.nextSiblingElement(m_name);
				return *this;
			}
		};

		typedef DeNodeArrIter ArrIter;

		ArrIter sub_elements() const
		{
			return ArrIter(m_node, m_node.tagName());
		}

		operator bool() const
		{
			return !m_node.isNull();
		}

	};

	typedef MSRPC::OArchiveHelper<MSRPC::DeNodeXml> OXmlArc;
	typedef MSRPC::IArchiveHelper<MSRPC::SeNodeXml> IXmlArc;
}

#endif // QXMLNODE_H__
