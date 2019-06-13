#ifndef MSJSONNODEAPT_QT_H__
#define MSJSONNODEAPT_QT_H__


#include "msarchive.hpp"
#include <QTextStream>


namespace MSRPC
{
	template<class T>
	class QtPosApt
	{
	public:
		QtPosApt(T& t) 
			: m_t(t) {}

		operator QString () const
		{
			return QString("%1,%2").arg(m_t.x()).arg(m_t.y());
		}

		void operator = (const QString& strValue)
		{
			double dX(0);
			double dY(0);
			char ch(0);
			QTextStream ts(const_cast<QString*>(&strValue));
			ts >> dX >> ch >> dY;
			m_t.setX(dX);
			m_t.setY(dY);
		}

	private:
		T& m_t;
	};

	template<class NODE, class T>
	class ISerialize<NODE, QtPosApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, QtPosApt<T>& tValue)
		{
			QString strValue = tValue;
			ISerialize<NODE, QString>::serialize(vNewNode, strValue);
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, QtPosApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, QtPosApt<T>& tValue)
		{
			QString strValue;
			OSerialize<NODE, QString>::serialize(vNewNode, strValue);
			tValue = strValue;
		}
	};

}

#endif // MSJSONNODEAPT_QT_H__
