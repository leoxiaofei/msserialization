#ifndef QJSONNODE_H__
#define QJSONNODE_H__

#include "msarchive.hpp"

#include <QJsonValue>
#include <QJsonObject>


namespace MSRPC
{
	class NValQJsonI
	{
	public:
		QJsonValue m_node;

	public:
		template <class T>
		void in_serialize(T& tValue)
		{
			m_node = tValue;
		}

		operator bool() const
		{
			return true;
		}

	};

	class NObjQJsonI
	{
	public:
		QJsonObject m_node;
		
	public:
		NObjQJsonI sub_obj(const char* strName)
		{
			return NObjQJsonI();
		}

		void end_obj(const char* strName, NObjQJsonI& vNode)
		{
			m_node[strName] = vNode.m_node;
		}

		NValQJsonI sub_val(const char* strName)
		{
			return NValQJsonI();
		}

		void end_val(const char* strName, NValQJsonI& vNode)
		{
			m_node[strName] = vNode.m_node;
		}

		operator bool() const
		{
			return true;
		}
	};

	typedef MSRPC::ArchiveHelper<MSRPC::NValQJsonI, MSRPC::NObjQJsonI> JsonIArc;


	class NValQJsonO
	{
	public:
		QJsonValueRef m_node;

		NValQJsonO(const QJsonValueRef& node)
			: m_node(node) {}

	public:
		template <class T>
		void in_serialize(T& tValue)
		{
			tValue = m_node.toVariant().value<T>();
		}

		operator bool() const
		{
			return !m_node.isNull();
		}

	};

	class NObjQJsonO
	{
	public:
		QJsonObject m_node;

		NObjQJsonO(const QJsonObject& node)
			: m_node(node) {}

	public:
		NObjQJsonO sub_obj(const char* strName)
		{
			return NObjQJsonO(m_node[strName].toObject());
		}

		void end_obj(const char* strName, NObjQJsonO& vNode)
		{
		}

		NValQJsonO sub_val(const char* strName)
		{
			return NValQJsonO(m_node[strName]);
		}

		void end_val(const char* strName, NValQJsonO& vNode)
		{
		}

		operator bool() const
		{
			return !m_node.isEmpty();
		}
	};

	typedef MSRPC::ArchiveHelper<MSRPC::NValQJsonO, MSRPC::NObjQJsonO> JsonOArc;

}

#endif // QJSONNODE_H__
