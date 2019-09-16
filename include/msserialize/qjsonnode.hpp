#ifndef QJSONNODE_H__
#define QJSONNODE_H__

#include "msarchive.hpp"

#include <QJsonObject>
#include <QJsonArray>

#define NODE_WITH_QT 1

namespace MSRPC
{
	class INodeJson
	{
	private:
		QJsonValue m_node;

	public:
		template <class T>
		void in_serialize(const T& tValue)
		{
			m_node = tValue;
		}

		void in_serialize(const char* tValue)
		{
			m_node = tValue;
		}

		INodeJson new_node()
		{
			return INodeJson();
		}

		void set_object(bool bIO = false)
		{
		}

		void add_member(const char* strName, INodeJson& vNode)
		{
			QJsonObject obj = m_node.toObject();
			obj[strName] = vNode.m_node;
			m_node = obj;
		}

		void set_array()
		{
		}

		void push_node(INodeJson& vNode)
		{
			QJsonArray arr = m_node.toArray();
			arr.append(vNode.m_node);
			m_node = arr;
		}

	public:
		QJsonValue& data()
		{
			return m_node;
		}

	};

	class ONodeJson
	{
	public:
		QJsonValue m_node;

		ONodeJson(const QJsonValue& node) 
			: m_node(node) {}

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

		ObjIter sub_members()
		{
			return ObjIter(m_node.toObject());
		}

		operator bool() const
		{
			return !m_node.isNull();
		}
	};

	typedef MSRPC::OArchiveHelper<MSRPC::ONodeJson> OJsonArc;
	typedef MSRPC::IArchiveHelper<MSRPC::INodeJson> IJsonArc;
}

#endif // QJSONNODE_H__
