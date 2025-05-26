#ifndef QJSONNODE_H__
#define QJSONNODE_H__

#include "msarchive.hpp"
#include "msserialize/msbasetypeapt_qt.hpp"

#include <QScopedPointer>
#include <QVariant>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define NODE_WITH_QT 1

namespace MSRPC
{
	class NJVBase
	{
	protected:
		enum { NJVType = 100 };

	public:
		virtual ~NJVBase() {}
		virtual QJsonValue data() const = 0;
		virtual int type() const = 0;
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
			Q_ASSERT(false);
		}

		QJsonValue& value()
		{
			return m_data;
		}

		enum {Type = NJVType + 1};
		virtual int type() const { return Type; }

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

		enum { Type = NJVType + 2 };
		virtual int type() const { return Type; }

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

		enum { Type = NJVType + 3 };
		virtual int type() const { return Type; }
	};

	class INodeJson
	{
	private:
		QScopedPointer<NJVBase> m_node;

	public:
		NJValue* set_value()
		{
			if (!m_node || m_node->type() != NJValue::Type)
			{
				m_node.reset(new NJValue);
			}
			return (NJValue*)m_node.data();
		}

		template <class T>
		void in_serialize(const T& tValue)
		{
			NJValue* nj = set_value();
			nj->value() = tValue;
		}

		template<class S, class T>
		void in_serialize(const BaseTypeApt<S, T>& tValue)
		{
			in_serialize((S)tValue);
		}

		void in_serialize(const unsigned int& tValue)
		{
			NJValue* nj = set_value();
			nj->value() = (int)tValue;
		}

		void in_serialize(const unsigned long long& tValue)
		{
			NJValue* nj = set_value();
			nj->value() = (qint64)tValue;
		}

		void in_serialize(const long& tValue)
		{
			NJValue* nj = set_value();
			nj->value() = (qint64)tValue;
		}

		void in_serialize(const unsigned long& tValue)
		{
			NJValue* nj = set_value();
			nj->value() = (qint64)tValue;
		}

		void in_serialize(const char* tValue)
		{
			NJValue* nj = set_value();
			nj->value() = tValue;
		}

		template <typename T>
		void in_serialize(const StrApt<T>& tValue)
		{
			in_serialize(tValue.Get());
		}

		INodeJson new_node()
		{
			return INodeJson();
		}

		NJObject* set_object()
		{
			if (!m_node || m_node->type() != NJObject::Type)
			{
				m_node.reset(new NJObject);
			}

			return (NJObject*)m_node.data();
		}

		void add_member(const char* strName, INodeJson& vNode)
		{
			QJsonObject& obj = static_cast<NJObject*>(m_node.data())->value();

			obj[strName] = vNode.data();
		}

		NJArray* set_array()
		{
			if (!m_node || m_node->type() != NJArray::Type)
			{
				m_node.reset(new NJArray);
			}

			return (NJArray*)m_node.data();
		}

		void push_node(INodeJson& vNode)
		{
			QJsonArray& arr = static_cast<NJArray*>(m_node.data())->value();
			arr.append(vNode.data());
		}

		void finish(QJsonDocument* doc)
		{
			m_node->setDoc(doc);
		}

	public:
		QJsonValue data()
		{
			return m_node->data();
		}

		INodeJson()
		{
		}

		INodeJson(const INodeJson& other)
			: m_node(const_cast<INodeJson&>(other).m_node.take())
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

		template<class S, class T>
		void in_serialize(BaseTypeApt<S, T>& tValue) const
		{
			S strValue;
			in_serialize(strValue);
			tValue = strValue;
		}

		void in_serialize(char*& tValue) const
		{
			QByteArray baBuffer = m_node.toString().toUtf8();

			tValue = new char[baBuffer.size()];
			memcpy(tValue, baBuffer.data(), baBuffer.size());
		}

		template <typename T>
		void in_serialize(StrApt<T>& tValue) const
		{
			QByteArray strValue = m_node.toString().toUtf8();
			tValue.Set(strValue.data(), strValue.size());
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
