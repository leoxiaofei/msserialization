#ifndef QJSONNODE_H__
#define QJSONNODE_H__

#include "msarchive.hpp"

#include <QJsonObject>
#include <QJsonArray>

#define NODE_WITH_QT 1

namespace MSRPC
{
	class NJVBase
	{
	public:
		virtual ~NJVBase() {}
		virtual QJsonValue data() const = 0;
		virtual void setDoc(QJsonDocument* doc) = 0;
	};

	class NJValue : public NJVBase
	{
	protected:
		QJsonValue m_data;

	public:
		virtual QJsonValue data() const
		{
			return m_data;
		}

		virtual void setDoc(QJsonDocument* doc)
		{
			// qt is not supported. 
			assert(false);
		}

		QJsonValue& value()
		{
			return m_data;
		}

	};

	class NJObject : public NJVBase
	{
	private:
		QJsonObject m_value;

	public:
		virtual QJsonValue data() const
		{
			return m_value;
		}

		virtual void setDoc(QJsonDocument* doc)
		{
			doc->setObject(m_value);
		}

		QJsonObject& value()
		{
			return m_value;
		}
	};

	class NJArray : public NJVBase
	{
	private:
		QJsonArray m_value;

	public:
		virtual QJsonValue data() const
		{
			return m_value;
		}

		virtual void setDoc(QJsonDocument* doc)
		{
			doc->setArray(m_value);
		}

		QJsonArray& value()
		{
			return m_value;
		}
	};

	class INodeJson
	{
	private:
		QScopedPointer<NJVBase> m_node;
		QJsonDocument* m_doc;

	public:

		template <class T>
		void in_serialize(const T& tValue)
		{
			NJValue* nj = new NJValue;
			nj->value() = tValue;
			m_node.reset(nj);
		}

		void in_serialize(const char* tValue)
		{
			NJValue* nj = new NJValue;
			nj->value() = tValue;
			m_node.reset(nj);
		}

		INodeJson new_node()
		{
			return INodeJson(m_doc);
		}

		void set_object()
		{
			m_node.reset(new NJObject);
		}

		void add_member(const char* strName, INodeJson& vNode)
		{
			QJsonObject& obj = static_cast<NJObject*>(m_node.get())->value();

			obj[strName] = vNode.data();
		}

		void set_array()
		{
			m_node.reset(new NJArray);
		}

		void push_node(INodeJson& vNode)
		{
			QJsonArray& arr = static_cast<NJArray*>(m_node.get())->value();
			arr.append(vNode.data());
		}

		void finish()
		{
			m_node->setDoc(m_doc);
		}

	public:
		QJsonValue data()
		{
			return m_node->data();
		}

		INodeJson(QJsonDocument* doc)
			: m_doc(doc)
		{}

		INodeJson(INodeJson& other)
			: m_node(other.m_node.take())
			, m_doc(other.m_doc)
		{}

	};

	class ONodeJson
	{
	private:
		QJsonValue m_node;

	public:
		template <class T>
		void in_serialize(T& tValue) const
		{
			tValue = m_node.toVariant().value<T>();
		}

		void in_serialize(char*& tValue) const
		{
			QByteArray baBuffer = m_node.toString().toUtf8();

			tValue = new char[baBuffer.size()];
			memcpy(tValue, baBuffer.data(), baBuffer.size());
		}

		ONodeJson sub_member(const char* strName) const
		{
			return ONodeJson(m_node.toObject()[strName]);
		}

		class ONodeArrIter
		{
		public:
			QJsonArray m_node;
			int m_idx;

			ONodeArrIter(const QJsonArray& node)
				: m_node(node)
				, m_idx(0) {}

		public:
			ONodeJson operator *() const
			{
				return ONodeJson(m_node[m_idx]);
			}

			operator bool() const
			{
				return m_idx != m_node.size();
			}

			ONodeArrIter& operator ++ ()
			{
				++m_idx;
				return *this;
			}
		};

		typedef ONodeArrIter ArrIter;

		ArrIter sub_nodes() const
		{
			return ArrIter(m_node.toArray());
		}

		class ONodeObjIter
		{
		public:
			QJsonObject m_node;
			QJsonObject::const_iterator m_citCur;
			QJsonObject::const_iterator m_citEnd;

			ONodeObjIter(const QJsonObject& node)
				: m_node(node)
				, m_citCur(m_node.constBegin()) 
				, m_citEnd(m_node.constEnd())
			{}

		public:
			ONodeJson operator *() const
			{
				return ONodeJson(*m_citCur);
			}

			QString key() const
			{
				return m_citCur.key();
			}

			operator bool() const
			{
				return m_citCur != m_citEnd;
			}

			ONodeObjIter& operator ++ ()
			{
				++m_citCur;
				return *this;
			}
		};

		typedef ONodeObjIter ObjIter;

		ObjIter sub_members() const
		{
			return ObjIter(m_node.toObject());
		}

		operator bool() const
		{
			return !m_node.isNull();
		}

	public:
		ONodeJson(const QJsonValue& node)
			: m_node(node) {}

		ONodeJson(const QJsonDocument* node)
			: m_node(node->isObject() ? node->object() 
			: node->isArray() ? node->array() : QJsonValue())
		{
			
		}

	};

	typedef MSRPC::OArchiveHelper<MSRPC::ONodeJson> OJsonArc;
	typedef MSRPC::IArchiveHelper<MSRPC::INodeJson> IJsonArc;
}

#endif // QJSONNODE_H__
