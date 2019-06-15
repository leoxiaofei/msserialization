#ifndef MSDATANODE_H__
#define MSDATANODE_H__

#include <QHash>
#include <QString>
#include <QVariant>
#include "msarchive.hpp"


namespace MSRPC
{
	/// A: key-value B: item

	class ONodeData
	{
	public:
		QStringRef m_strKey;
		QVariant m_vValue;

		ONodeData() {}

		ONodeData(const QStringRef& strKey, const QVariant& vValue)
			: m_strKey(strKey)
			, m_vValue(vValue)
		{}

	public:
		template <class T>
		void in_serialize(T& tValue) const
		{
			tValue = m_vValue.value<T>();
		}

		void in_serialize(char*& tValue) const
		{
			QByteArray baBuffer = m_vValue.toString().toUtf8();

			tValue = new char[baBuffer.size()];
			memcpy(tValue, baBuffer.data(), baBuffer.size());
		}

		ONodeData sub_member(const char* strName) const
		{
			QLatin1String strNa(strName);
			if (m_strKey.startsWith(strNa) && (strNa.size() == m_strKey.size() ||
				m_strKey[strNa.size()] == '.'))
			{
				return ONodeData(m_strKey.right(m_strKey.size() - strNa.size()), m_vValue);
			}
			else
			{
				return ONodeData();
			}


			//return (!m_listKey.isEmpty() && m_listKey.last() == strName)
			//	? ONodeData(m_listKey.mid(0, m_listKey.size() - 1), m_vValue)
			//	: ONodeData();
		}

		//class ONodeJsonIter
		//{
		//public:
		//	QJsonArray m_node;
		//	int m_idx;

		//	ONodeJsonIter(const QJsonArray& node)
		//		: m_node(node)
		//		, m_idx(0) {}

		//public:
		//	ONodeJson operator *() const
		//	{
		//		return ONodeJson(m_node[m_idx]);
		//	}

		//	operator bool() const
		//	{
		//		return m_idx != m_node.size();
		//	}

		//	ONodeJsonIter& operator ++ ()
		//	{
		//		++m_idx;
		//		return *this;
		//	}
		//};

		//typedef ONodeJsonIter ITER;

		//ITER sub_node() const
		//{
		//	return ITER(m_node.toArray());
		//}

		operator bool() const
		{
			return !m_vValue.isNull();
		}
	};

	
	typedef MSRPC::OArchiveHelper<MSRPC::ONodeData> ODataArc;
}

#endif // MSDATANODE_H__