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

		void add_member(const char* strName, INodeJson& vNode)
		{
			QJsonObject obj = m_node.toObject();
			obj[strName] = vNode.m_node;
			m_node = obj;
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

		class ONodeJsonIter
		{
		public:
			QJsonArray m_node;
			int m_idx;

			ONodeJsonIter(const QJsonArray& node)
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

			ONodeJsonIter& operator ++ ()
			{
				++m_idx;
				return *this;
			}
		};

		typedef ONodeJsonIter ITER;

		ITER sub_node() const
		{
			return ITER(m_node.toArray());
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
