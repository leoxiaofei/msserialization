#ifndef MSARCHIVE_QT_H__
#define MSARCHIVE_QT_H__

#include "msarchive.hpp"
#include <QString>
#include <QTextStream>
#include <QDataStream>
#include <QSharedPointer>
#include <QDateTime>
#include <QMetaType>
#include <QSet>
#include <QLine>
#include <QMargins>

#ifdef QT_GUI_LIB
#include <QPolygon>
#include <QFont>
#include <QPen>
#endif

namespace MSRPC
{
	template<class NODE>
	class ISerialize<NODE, QString>
	{
	public:
		static void serialize(NODE& vNewNode, const QString& tValue)
		{
#ifdef NODE_WITH_QT
			vNewNode.in_serialize(tValue);
#else
			QByteArray baBuffer = tValue.toUtf8();
			vNewNode.in_serialize(baBuffer.data());
#endif
		}
	};

	template<class NODE>
	class OSerialize<NODE, QString>
	{
	public:
		static void serialize(const NODE& vNewNode, QString& tValue)
		{
#ifdef NODE_WITH_QT
			vNewNode.in_serialize(tValue);
#else
			const char* p = 0;
			vNewNode.in_serialize(p);
			tValue = QString::fromUtf8(p);
#endif
		}
	};

	template<class NODE, class T>
	class ISerialize<NODE, QVector<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const QVector<T>& tValue)
		{
			vNewNode.set_array();
			for (int ix = 0; ix != tValue.size(); ++ix)
			{
				NODE vNode = vNewNode.new_node();
				ISerialize<NODE, T>::serialize(vNode, tValue[ix]);
				vNewNode.push_node(vNode);
			}
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, QVector<T> >
	{
	public:
		static void serialize(const NODE& vNewNode, QVector<T>& tValue)
		{
			int ix = 0;
			typename NODE::ArrIter itor = vNewNode.sub_nodes();
			for (; itor; ++itor, ++ix)
			{
				if (NODE node = *itor)
				{
					if (ix >= tValue.size())
					{
						tValue.resize(ix + 1);
					}

					T& t = tValue[ix];
					OSerialize<NODE, T>::serialize(node, t);
				}
			}
			if (ix != tValue.size())
			{
				tValue.resize(ix);
			}
		}
	};

	template<class NODE, class T>
	class ISerialize<NODE, QList<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const QList<T>& tValue)
		{
			vNewNode.set_array();
			for (int ix = 0; ix != tValue.size(); ++ix)
			{
				NODE vNode = vNewNode.new_node();
				ISerialize<NODE, T>::serialize(vNode, tValue[ix]);
				vNewNode.push_node(vNode);
			}
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, QList<T> >
	{
	public:
		static void serialize(const NODE& vNewNode, QList<T>& tValue)
		{
			int ix = 0;
			typename NODE::ArrIter itor = vNewNode.sub_nodes();
			for (; itor; ++itor, ++ix)
			{
				if (NODE node = *itor)
				{
					while (ix >= tValue.size())
					{
						T t;
						tValue.append(t);
					}
					T& t = tValue[ix];
					OSerialize<NODE, T>::serialize(node, t);
				}
			}

			if (ix != tValue.size())
			{
				tValue.erase(tValue.begin() + ix, tValue.end());
			}
		}
	};

	template<class NODE, class T>
	class ISerialize<NODE, QHash<QString, T> >
	{
	public:
		static void serialize(NODE& vNewNode, const QHash<QString, T>& tValue)
		{
			vNewNode.set_object();
			for (typename QHash<QString, T>::const_iterator citor = tValue.constBegin(); 
			citor != tValue.constEnd(); ++citor)
			{
				QByteArray baKey = citor.key().toUtf8();
				NODE vNode = vNewNode.new_node();
				ISerialize<NODE, T>::serialize(vNode, *citor);
				vNewNode.add_member(baKey.data(), vNode);
			}
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, QHash<QString, T> >
	{
	public:
		static void serialize(const NODE& vNewNode, QHash<QString, T>& tValue)
		{
			QSet<QString> setKey;

			typename NODE::ObjIter itor = vNewNode.sub_members();
			for (; itor; ++itor)
			{
				if (NODE node = *itor)
				{
					T& t = tValue[itor.key()];
					OSerialize<NODE, T>::serialize(*itor, t);
					setKey.insert(itor.key());
				}
			}

			if (setKey.size() != tValue.size())
			{
				for (typename QHash<QString, T>::iterator itor = tValue.begin();
					itor != tValue.end();)
				{
					if (!setKey.contains(itor.key()))
					{
						itor = tValue.erase(itor);
					}
					else
					{
						++itor;
					}
				}
			}
		}
	};

	template<class NODE, class K, class T>
	class ISerialize<NODE, QHash<K, T> >
	{
	public:
		static void serialize(NODE& vNewNode, const QHash<K, T>& tValue)
		{
			vNewNode.set_object();

			if (!tValue.isEmpty())
			{
				NODE vKeyNode = vNewNode.new_node();
				vKeyNode.set_array();

				NODE vValueNode = vNewNode.new_node();
				vValueNode.set_array();

				for (typename QHash<K, T>::const_iterator citor = tValue.constBegin();
					 citor != tValue.constEnd(); ++citor)
				{
					NODE vKNode = vKeyNode.new_node();
					ISerialize<NODE, K>::serialize(vKNode, citor.key());
					vKeyNode.push_node(vKNode);

					NODE vVNode = vValueNode.new_node();
					ISerialize<NODE, T>::serialize(vVNode, citor.value());
					vValueNode.push_node(vVNode);
				}

				vNewNode.add_member("key", vKeyNode);
				vNewNode.add_member("value", vValueNode);
			}
		}
	};

	template<class NODE, class K, class T>
	class OSerialize<NODE, QHash<K, T> >
	{
	public:
		static void serialize(const NODE& vNewNode, QHash<K, T>& tValue)
		{
			QSet<K> setKey;

			NODE vKeyNode = vNewNode.sub_member("key");
			NODE vValueNode = vNewNode.sub_member("value");

			if (vKeyNode && vValueNode)
			{
				typename NODE::ArrIter itorKey = vKeyNode.sub_nodes();
				typename NODE::ArrIter itorValue = vValueNode.sub_nodes();
				for (; itorKey && itorValue; ++itorKey, ++itorValue)
				{
					NODE nodeKey = *itorKey;
					NODE nodeVal = *itorValue;
					if (nodeKey && nodeVal)
					{
						K k;
						OSerialize<NODE, K>::serialize(nodeKey, k);
						T& t = tValue[k];
						OSerialize<NODE, T>::serialize(nodeVal, t);
						setKey.insert(k);
					}
				}

				if (setKey.size() != tValue.size())
				{
					for (typename QHash<K, T>::iterator itor = tValue.begin();
						itor != tValue.end();)
					{
						if (!setKey.contains(itor.key()))
						{
							itor = tValue.erase(itor);
						}
						else
						{
							++itor;
						}
					}
				}
			}
		}
	};

	template<class NODE, class T>
	class ISerialize<NODE, QMap<QString, T> >
	{
	public:
		static void serialize(NODE& vNewNode, const QMap<QString, T>& tValue)
		{
			vNewNode.set_object();
			for (typename QMap<QString, T>::const_iterator citor = tValue.constBegin();
				citor != tValue.constEnd(); ++citor)
			{
				QByteArray baKey = citor.key().toUtf8();
				NODE vNode = vNewNode.new_node();
				ISerialize<NODE, T>::serialize(vNode, *citor);
				vNewNode.add_member(baKey.data(), vNode);
			}
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, QMap<QString, T> >
	{
	public:
		static void serialize(const NODE& vNewNode, QMap<QString, T>& tValue)
		{
			QSet<QString> setKey;

			typename NODE::ObjIter itor = vNewNode.sub_members();
			for (; itor; ++itor)
			{
				if (NODE node = *itor)
				{
					T& t = tValue[itor.key()];
					OSerialize<NODE, T>::serialize(node, t);
					setKey.insert(itor.key());
				}
			}

			if (setKey.size() != tValue.size())
			{
				for (typename QMap<QString, T>::iterator itor = tValue.begin();
					itor != tValue.end();)
				{
					if (!setKey.contains(itor.key()))
					{
						itor = tValue.erase(itor);
					}
					else
					{
						++itor;
					}
				}
			}
		}
	};

	template<class NODE, class K, class T>
	class ISerialize<NODE, QMap<K, T> >
	{
	public:
		static void serialize(NODE& vNewNode, const QMap<K, T>& tValue)
		{
			vNewNode.set_object();

			if (!tValue.isEmpty())
			{
				NODE vKeyNode = vNewNode.new_node();
				vKeyNode.set_array();

				NODE vValueNode = vNewNode.new_node();
				vValueNode.set_array();

				for (typename QMap<K, T>::const_iterator citor = tValue.constBegin();
					 citor != tValue.constEnd(); ++citor)
				{
					NODE vKNode = vKeyNode.new_node();
					ISerialize<NODE, K>::serialize(vKNode, citor.key());
					vKeyNode.push_node(vKNode);

					NODE vVNode = vValueNode.new_node();
					ISerialize<NODE, T>::serialize(vVNode, citor.value());
					vValueNode.push_node(vVNode);
				}

				vNewNode.add_member("key", vKeyNode);
				vNewNode.add_member("value", vValueNode);
			}
		}
	};

	template<class NODE, class K, class T>
	class OSerialize<NODE, QMap<K, T> >
	{
	public:
		static void serialize(const NODE& vNewNode, QMap<K, T>& tValue)
		{
			NODE vKeyNode = vNewNode.sub_member("key");
			NODE vValueNode = vNewNode.sub_member("value");

			if (vKeyNode && vValueNode)
			{
				QSet<K> setKey;

				typename NODE::ArrIter itorKey = vKeyNode.sub_nodes();
				typename NODE::ArrIter itorValue = vValueNode.sub_nodes();
				for (; itorKey && itorValue; ++itorKey, ++itorValue)
				{
					NODE nodeKey = *itorKey;
					NODE nodeVal = *itorValue;
					if (nodeKey && nodeVal)
					{
						K k;
						OSerialize<NODE, K>::serialize(nodeKey, k);
						T& t = tValue[k];
						OSerialize<NODE, T>::serialize(nodeVal, t);
						setKey.insert(k);
					}
				}

				if (setKey.size() != tValue.size())
				{
					for (typename QMap<K, T>::iterator itor = tValue.begin();
						itor != tValue.end();)
					{
						if (!setKey.contains(itor.key()))
						{
							itor = tValue.erase(itor);
						}
						else
						{
							++itor;
						}
					}
				}
			}
		}
	};

	template<class NODE, typename T>
	class ISerialize<NODE, QSharedPointer<T>>
	{
	public:
		static void serialize(NODE& vNewNode, const QSharedPointer<T>& tValue)
		{
			ISerialize<NODE, T*>::serialize(vNewNode, tValue.data());
		}
	};

	template<class NODE, typename T>
	class OSerialize<NODE, QSharedPointer<T>>
	{
	public:
		static void serialize(const NODE& vNewNode, QSharedPointer<T>& tValue)
		{
			T* pRet = 0;
			OSerialize<NODE, T*>::serialize(vNewNode, pRet);
			tValue = QSharedPointer<T>(pRet);
		}
	};

	template<class NODE>
	class ISerialize<NODE, QFont>
	{
	public:
		static void serialize(NODE& vNewNode, const QFont& tValue)
		{
			ISerialize<NODE, QString>::serialize(vNewNode, tValue.toString());
		}
	};

	template<class NODE>
	class OSerialize<NODE, QFont>
	{
	public:
		static void serialize(const NODE& vNewNode, QFont& tValue)
		{
			QString strValue;
			OSerialize<NODE, QString>::serialize(vNewNode, strValue);
			tValue.fromString(strValue);
		}
	};

	template<class NODE>
	class ISerialize<NODE, QBrush>
	{
	public:
		static void serialize(NODE& vNewNode, const QBrush& tValue)
		{
			QString strValue = QString("%1,%2").arg(tValue.color().name()).arg(tValue.style());
			ISerialize<NODE, QString>::serialize(vNewNode, strValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, QBrush>
	{
	public:
		static void serialize(const NODE& vNewNode, QBrush& tValue)
		{
			QString strValue;
			OSerialize<NODE, QString>::serialize(vNewNode, strValue);
			QStringList listValue = strValue.split(',');
			if (listValue.size() > 1)
			{
				tValue.setStyle(static_cast<Qt::BrushStyle>(
					listValue[1].toInt()));
			}
			tValue.setColor(QColor(listValue[0]));
		}
	};

	template<class NODE>
	class ISerialize<NODE, QPen>
	{
	public:
		static void serialize(NODE& vNewNode, const QPen& tValue)
		{
			QString strValue = QString("%1,%2,%3,%4,%5").arg(tValue.color().name())
				.arg(tValue.widthF()).arg(tValue.style()).arg(tValue.capStyle()).arg(tValue.joinStyle());
			ISerialize<NODE, QString>::serialize(vNewNode, strValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, QPen>
	{
	public:
		static void serialize(const NODE& vNewNode, QPen& tValue)
		{
			QString strValue;
			OSerialize<NODE, QString>::serialize(vNewNode, strValue);
			QStringList listValue = strValue.split(',');

			int nSize = qMax(listValue.size(), 5);
			switch (nSize - 1)
			{
			case 4:
				tValue.setJoinStyle(static_cast<Qt::PenJoinStyle>(
					listValue[4].toInt()));
			case 3:
				tValue.setCapStyle(static_cast<Qt::PenCapStyle>(
					listValue[3].toInt()));
			case 2:
				tValue.setStyle(static_cast<Qt::PenStyle>(
					listValue[2].toInt()));
			case 1:
				tValue.setWidthF(listValue[1].toDouble());
			case 0:
				tValue.setColor(QColor(listValue[0]));
				break;
			default:
				break;
			}
		}
	};

	template<class NODE>
	class ISerialize<NODE, QPointF>
	{
	public:
		static void serialize(NODE& vNewNode, const QPointF& tValue)
		{
			QString strValue = QString("%1,%2").arg(tValue.x()).arg(tValue.y());
			ISerialize<NODE, QString>::serialize(vNewNode, strValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, QPointF>
	{
	public:
		static void serialize(const NODE& vNewNode, QPointF& tValue)
		{
			QString strValue;
			OSerialize<NODE, QString>::serialize(vNewNode, strValue);
			char ch(0);
			QTextStream ts(const_cast<QString*>(&strValue));
			ts >> tValue.rx() >> ch >> tValue.ry();
		}
	};

	template<class NODE>
	class ISerialize<NODE, QPoint>
	{
	public:
		static void serialize(NODE& vNewNode, const QPoint& tValue)
		{
			QString strValue = QString("%1,%2").arg(tValue.x()).arg(tValue.y());
			ISerialize<NODE, QString>::serialize(vNewNode, strValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, QPoint>
	{
	public:
		static void serialize(const NODE& vNewNode, QPoint& tValue)
		{
			QString strValue;
			OSerialize<NODE, QString>::serialize(vNewNode, strValue);
			char ch(0);
			QTextStream ts(const_cast<QString*>(&strValue));
			ts >> tValue.rx() >> ch >> tValue.ry();
		}
	};

	template<class NODE>
	class ISerialize<NODE, QSizeF>
	{
	public:
		static void serialize(NODE& vNewNode, const QSizeF& tValue)
		{
			QString strValue = QString("%1,%2").arg(tValue.width()).arg(tValue.height());
			ISerialize<NODE, QString>::serialize(vNewNode, strValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, QSizeF>
	{
	public:
		static void serialize(const NODE& vNewNode, QSizeF& tValue)
		{
			QString strValue;
			OSerialize<NODE, QString>::serialize(vNewNode, strValue);
			char ch(0);
			QTextStream ts(const_cast<QString*>(&strValue));
			ts >> tValue.rwidth() >> ch >> tValue.rheight();
		}
	};

	template<class NODE>
	class ISerialize<NODE, QSize>
	{
	public:
		static void serialize(NODE& vNewNode, const QSize& tValue)
		{
			QString strValue = QString("%1,%2").arg(tValue.width()).arg(tValue.height());
			ISerialize<NODE, QString>::serialize(vNewNode, strValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, QSize>
	{
	public:
		static void serialize(const NODE& vNewNode, QSize& tValue)
		{
			QString strValue;
			OSerialize<NODE, QString>::serialize(vNewNode, strValue);
			char ch(0);
			QTextStream ts(const_cast<QString*>(&strValue));
			ts >> tValue.rwidth() >> ch >> tValue.rheight();
		}
	};

	template<class NODE>
	class ISerialize<NODE, QRectF>
	{
	public:
		static void serialize(NODE& vNewNode, const QRectF& tValue)
		{
			QString strValue = QString("%1,%2,%3,%4").arg(tValue.x())
				.arg(tValue.y()).arg(tValue.width()).arg(tValue.height());

			ISerialize<NODE, QString>::serialize(vNewNode, strValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, QRectF>
	{
	public:
		static void serialize(const NODE& vNewNode, QRectF& tValue)
		{
			QString strValue;
			OSerialize<NODE, QString>::serialize(vNewNode, strValue);
			qreal dX(0);
			qreal dY(0);
			qreal dW(0);
			qreal dH(0);
			char ch(0);
			QTextStream ts(const_cast<QString*>(&strValue));
			ts >> dX >> ch >> dY >> ch >> dW >> ch >> dH;
			tValue.setRect(dX, dY, dW, dH);
		}
	};

	template<class NODE>
	class ISerialize<NODE, QRect>
	{
	public:
		static void serialize(NODE& vNewNode, const QRect& tValue)
		{
			QString strValue = QString("%1,%2,%3,%4").arg(tValue.x())
				.arg(tValue.y()).arg(tValue.width()).arg(tValue.height());

			ISerialize<NODE, QString>::serialize(vNewNode, strValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, QRect>
	{
	public:
		static void serialize(const NODE& vNewNode, QRect& tValue)
		{
			QString strValue;
			OSerialize<NODE, QString>::serialize(vNewNode, strValue);
			int nX(0);
			int nY(0);
			int nW(0);
			int nH(0);
			char ch(0);
			QTextStream ts(const_cast<QString*>(&strValue));
			ts >> nX >> ch >> nY >> ch >> nW >> ch >> nH;
			tValue.setRect(nX, nY, nW, nH);
		}
	};

	template<class NODE>
	class ISerialize<NODE, QMarginsF>
	{
	public:
		static void serialize(NODE& vNewNode, const QMarginsF& tValue)
		{
			QString strValue = QString("%1,%2,%3,%4").arg(tValue.left())
				.arg(tValue.top()).arg(tValue.right()).arg(tValue.bottom());

			ISerialize<NODE, QString>::serialize(vNewNode, strValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, QMarginsF>
	{
	public:
		static void serialize(const NODE& vNewNode, QMarginsF& tValue)
		{
			QString strValue;
			OSerialize<NODE, QString>::serialize(vNewNode, strValue);
			qreal dX(0);
			qreal dY(0);
			qreal dW(0);
			qreal dH(0);
			char ch(0);
			QTextStream ts(const_cast<QString*>(&strValue));
			ts >> dX >> ch >> dY >> ch >> dW >> ch >> dH;
			tValue = { dX, dY, dW, dH };
		}
	};

	template<class NODE>
	class ISerialize<NODE, QMargins>
	{
	public:
		static void serialize(NODE& vNewNode, const QMargins& tValue)
		{
			QString strValue = QString("%1,%2,%3,%4").arg(tValue.left())
				.arg(tValue.top()).arg(tValue.right()).arg(tValue.bottom());

			ISerialize<NODE, QString>::serialize(vNewNode, strValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, QMargins>
	{
	public:
		static void serialize(const NODE& vNewNode, QMargins& tValue)
		{
			QString strValue;
			OSerialize<NODE, QString>::serialize(vNewNode, strValue);
			int nX(0);
			int nY(0);
			int nW(0);
			int nH(0);
			char ch(0);
			QTextStream ts(const_cast<QString*>(&strValue));
			ts >> nX >> ch >> nY >> ch >> nW >> ch >> nH;
			tValue = { dX, dY, dW, dH };
		}
	};

	template<class NODE>
	class ISerialize<NODE, QLineF>
	{
	public:
		static void serialize(NODE& vNewNode, const QLineF& tValue)
		{
			QString strValue = QString("%1,%2 %3,%4").arg(tValue.x1())
				.arg(tValue.y1()).arg(tValue.x2()).arg(tValue.y2());

			ISerialize<NODE, QString>::serialize(vNewNode, strValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, QLineF>
	{
	public:
		static void serialize(const NODE& vNewNode, QLineF& tValue)
		{
			QString strValue;
			OSerialize<NODE, QString>::serialize(vNewNode, strValue);
			qreal dX(0);
			qreal dY(0);
			qreal dX2(0);
			qreal dY2(0);
			char ch(0);
			QTextStream ts(const_cast<QString*>(&strValue));
			ts >> dX >> ch >> dY >> ch >> dX2 >> ch >> dY2;
			tValue.setLine(dX, dY, dX2, dY2);
		}
	};

	template<class NODE>
	class ISerialize<NODE, QLine>
	{
	public:
		static void serialize(NODE& vNewNode, const QLine& tValue)
		{
			QString strValue = QString("%1,%2 %3,%4").arg(tValue.x1())
				.arg(tValue.y1()).arg(tValue.x2()).arg(tValue.y2());

			ISerialize<NODE, QString>::serialize(vNewNode, strValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, QLine>
	{
	public:
		static void serialize(const NODE& vNewNode, QLine& tValue)
		{
			QString strValue;
			OSerialize<NODE, QString>::serialize(vNewNode, strValue);
			int nX(0);
			int nY(0);
			int nX2(0);
			int nY2(0);
			char ch(0);
			QTextStream ts(const_cast<QString*>(&strValue));
			ts >> nX >> ch >> nY >> ch >> nX2 >> ch >> nY2;
			tValue.setLine(nX, nY, nX2, nY2);
		}
	};

#ifdef QT_GUI_LIB
	template<class NODE>
	class ISerialize<NODE, QPolygonF>
	{
	public:
		static void serialize(NODE& vNewNode, const QPolygonF& tValue)
		{
			ISerialize<NODE, QVector<QPointF> >::serialize(vNewNode, tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, QPolygonF>
	{
	public:
		static void serialize(const NODE& vNewNode, QPolygonF& tValue)
		{
			OSerialize<NODE, QVector<QPointF> >::serialize(vNewNode, tValue);
		}
	};

	template<class NODE>
	class ISerialize<NODE, QPolygon>
	{
	public:
		static void serialize(NODE& vNewNode, const QPolygon& tValue)
		{
			ISerialize<NODE, QVector<QPoint> >::serialize(vNewNode, tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, QPolygon>
	{
	public:
		static void serialize(const NODE& vNewNode, QPolygon& tValue)
		{
			OSerialize<NODE, QVector<QPoint> >::serialize(vNewNode, tValue);
		}
	};
#endif

	template<class NODE>
	class ISerialize<NODE, QDate>
	{
	public:
		static void serialize(NODE& vNewNode, const QDate& tValue)
		{
			QString strValue = tValue.toString(Qt::ISODate);

			ISerialize<NODE, QString>::serialize(vNewNode, strValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, QDate>
	{
	public:
		static void serialize(const NODE& vNewNode, QDate& tValue)
		{
			QString strValue;
			OSerialize<NODE, QString>::serialize(vNewNode, strValue);
			tValue = QDate::fromString(strValue, Qt::ISODate);
		}
	};

	template<class NODE>
	class ISerialize<NODE, QTime>
	{
	public:
		static void serialize(NODE& vNewNode, const QTime& tValue)
		{
			QString strValue = tValue.toString(Qt::ISODate);

			ISerialize<NODE, QString>::serialize(vNewNode, strValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, QTime>
	{
	public:
		static void serialize(const NODE& vNewNode, QTime& tValue)
		{
			QString strValue;
			OSerialize<NODE, QString>::serialize(vNewNode, strValue);
			tValue = QTime::fromString(strValue, Qt::ISODate);
		}
	};

	template<class NODE>
	class ISerialize<NODE, QDateTime>
	{
	public:
		static void serialize(NODE& vNewNode, const QDateTime& tValue)
		{
			QString strValue = tValue.toString(Qt::ISODate);

			ISerialize<NODE, QString>::serialize(vNewNode, strValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, QDateTime>
	{
	public:
		static void serialize(const NODE& vNewNode, QDateTime& tValue)
		{
			QString strValue;
			OSerialize<NODE, QString>::serialize(vNewNode, strValue);
			tValue = QDateTime::fromString(strValue, Qt::ISODate);
		}
	};

	template<class NODE>
	class ISerialize<NODE, QByteArray>
	{
	public:
		static void serialize(NODE& vNewNode, const QByteArray& tValue)
		{
			QByteArray baBuffer = tValue.toBase64();
			vNewNode.in_serialize(baBuffer.data());
		}
	};

	template<class NODE>
	class OSerialize<NODE, QByteArray>
	{
	public:
		static void serialize(const NODE& vNewNode, QByteArray& tValue)
		{
			QString strBase64;
			vNewNode.in_serialize(strBase64);
			tValue = QByteArray::fromBase64(strBase64.toUtf8());
		}
	};

	template<class NODE>
	class ISerialize<NODE, QStringList>
	{
	public:
		static void serialize(NODE& vNewNode, const QStringList& tValue)
		{
			ISerialize<NODE, QList<QString> >::serialize(vNewNode, tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, QStringList>
	{
	public:
		static void serialize(const NODE& vNewNode, QStringList& tValue)
		{
			OSerialize<NODE, QList<QString> >::serialize(vNewNode, tValue);
		}
	};

	template<class NODE>
	class ISerialize<NODE, QVariant>
	{
	public:
		static void serialize(NODE& vNewNode, const QVariant& tValue)
		{
			vNewNode.set_object();

			///保存QVariant内部数据类型
			NODE vNodeType = vNewNode.new_node();
			ISerialize<NODE, char*>::serialize(vNodeType, tValue.typeName());
			vNewNode.add_member("type", vNodeType);

			NODE vNodeValue = vNewNode.new_node();

			///根据类型保存值
			///这里其实我想用Map，但是这是个纯头文件的库，Map的数据保存在哪里还没想清楚，
			///只好靠C++编译器优化switch了。
			switch (tValue.type())
			{
			case QMetaType::Bool:
				ISerialize<NODE, bool>::serialize(vNodeValue, tValue.toBool());
				break;
			case QMetaType::Int:
				ISerialize<NODE, int>::serialize(vNodeValue, tValue.toInt());
				break;
			case QMetaType::UInt:
				ISerialize<NODE, unsigned int>::serialize(vNodeValue, tValue.toUInt());
				break;
			case QMetaType::LongLong:
				ISerialize<NODE, long long>::serialize(vNodeValue, tValue.toLongLong());
				break;
			case QMetaType::ULongLong:
				ISerialize<NODE, unsigned long long>::serialize(vNodeValue, tValue.toULongLong());
				break;
			case QMetaType::Double:
				ISerialize<NODE, double>::serialize(vNodeValue, tValue.toDouble());
				break;
			case QMetaType::QVariantMap:
				ISerialize<NODE, QVariantMap>::serialize(
					vNodeValue, *static_cast<const QVariantMap*>(tValue.data()));
				break;
			case QMetaType::QVariantList:
				ISerialize<NODE, QVariantList>::serialize(
					vNodeValue, *static_cast<const QVariantList*>(tValue.data()));
				break;
			case QMetaType::QString:
				ISerialize<NODE, QString>::serialize(
					vNodeValue, *static_cast<const QString*>(tValue.data()));
				break;
			case QMetaType::QStringList:
				ISerialize<NODE, QStringList>::serialize(
					vNodeValue, *static_cast<const QStringList*>(tValue.data()));
				break;
			case QMetaType::QByteArray:
				ISerialize<NODE, QByteArray>::serialize(
					vNodeValue, *static_cast<const QByteArray*>(tValue.data()));
				break;
			case QMetaType::QDate:
				ISerialize<NODE, QDate>::serialize(
					vNodeValue, *static_cast<const QDate*>(tValue.data()));
				break;
			case QMetaType::QTime:
				ISerialize<NODE, QTime>::serialize(
					vNodeValue, *static_cast<const QTime*>(tValue.data()));
				break;
			case QMetaType::QDateTime:
				ISerialize<NODE, QDateTime>::serialize(
					vNodeValue, *static_cast<const QDateTime*>(tValue.data()));
				break;
			case QMetaType::QRect:
				ISerialize<NODE, QRect>::serialize(
					vNodeValue, *static_cast<const QRect*>(tValue.data()));
				break;
			case QMetaType::QRectF:
				ISerialize<NODE, QRectF>::serialize(
					vNodeValue, *static_cast<const QRectF*>(tValue.data()));
				break;
			case QMetaType::QSize:
				ISerialize<NODE, QSize>::serialize(
					vNodeValue, *static_cast<const QSize*>(tValue.data()));
				break;
			case QMetaType::QSizeF:
				ISerialize<NODE, QSizeF>::serialize(
					vNodeValue, *static_cast<const QSizeF*>(tValue.data()));
				break;
			case QMetaType::QLine:
				ISerialize<NODE, QLine>::serialize(
					vNodeValue, *static_cast<const QLine*>(tValue.data()));
				break;
			case QMetaType::QLineF:
				ISerialize<NODE, QLineF>::serialize(
					vNodeValue, *static_cast<const QLineF*>(tValue.data()));
				break;
			case QMetaType::QPoint:
				ISerialize<NODE, QPoint>::serialize(
					vNodeValue, *static_cast<const QPoint*>(tValue.data()));
				break;
			case QMetaType::QPointF:
				ISerialize<NODE, QPointF>::serialize(
					vNodeValue, *static_cast<const QPointF*>(tValue.data()));
				break;
			case QMetaType::QVariantHash:
				ISerialize<NODE, QVariantHash>::serialize(
					vNodeValue, *static_cast<const QVariantHash*>(tValue.data()));
				break;
#ifdef QT_GUI_LIB
			case QMetaType::QFont:
				ISerialize<NODE, QFont>::serialize(
					vNodeValue, *static_cast<const QFont*>(tValue.data()));
				break;
			case QMetaType::QPolygon:
				ISerialize<NODE, QPolygon>::serialize(
					vNodeValue, *static_cast<const QPolygon*>(tValue.data()));
				break;
			case QMetaType::QPolygonF:
				ISerialize<NODE, QPolygonF>::serialize(
					vNodeValue, *static_cast<const QPolygonF*>(tValue.data()));
				break;
#endif
			default:
			{
				QByteArray ba;
				QDataStream ds(&ba, QIODevice::WriteOnly);
				ds << tValue;
				ISerialize<NODE, QByteArray>::serialize(vNodeValue, ba);
				break;
			}
			}

			vNewNode.add_member("value", vNodeValue);

		}
	};

	template<class NODE>
	class OSerialize<NODE, QVariant>
	{
	public:
		static void serialize(const NODE& vNewNode, QVariant& tValue)
		{
			NODE vNodeType = vNewNode.sub_member("type");
			NODE vNodeValue = vNewNode.sub_member("value");
			if (vNodeType && vNodeValue)
			{
				QString baData;
				vNodeType.in_serialize(baData);
				int nType = QVariant::nameToType(baData.toUtf8().data());
				///这里其实我想用Map，但是这是个纯头文件的库，Map的数据保存在哪里还没想清楚，
				///只好靠C++编译器优化switch了。
				switch (nType)
				{
				case QMetaType::Bool:
				{
					bool val = false;
					OSerialize<NODE, bool>::serialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::Int:
				{
					int val = 0;
					OSerialize<NODE, int>::serialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::UInt:
				{
					unsigned int val = 0;
					OSerialize<NODE, unsigned int>::serialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::LongLong:
				{
					long long val = 0;
					OSerialize<NODE, long long>::serialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::ULongLong:
				{
					unsigned long long val = 0;
					OSerialize<NODE, unsigned long long>::serialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::Double:
				{
					double val = 0;
					OSerialize<NODE, double>::serialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QVariantMap:
				{
					QVariantMap val;
					OSerialize<NODE, QVariantMap>::serialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QVariantList:
				{
					QVariantList val;
					OSerialize<NODE, QVariantList>::serialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QString:
				{
					QString val;
					OSerialize<NODE, QString>::serialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QStringList:
				{
					QStringList val;
					OSerialize<NODE, QStringList>::serialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QByteArray:
				{
					QByteArray val;
					OSerialize<NODE, QByteArray>::serialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QDate:
				{
					QDate val;
					OSerialize<NODE, QDate>::serialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QTime:
				{
					QTime val;
					OSerialize<NODE, QTime>::serialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QDateTime:
				{
					QDateTime val;
					OSerialize<NODE, QDateTime>::serialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QRect:
				{
					QRect val;
					OSerialize<NODE, QRect>::serialize(vNodeValue, val);
					tValue = val;
					break;
				}
					OSerialize<NODE, QRect>::serialize(
						vNodeValue, *static_cast<QRect*>(tValue.data()));
					break;
				case QMetaType::QRectF:
				{
					QRectF val;
					OSerialize<NODE, QRectF>::serialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QSize:
				{
					QSize val;
					OSerialize<NODE, QSize>::serialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QSizeF:
				{
					QSizeF val;
					OSerialize<NODE, QSizeF>::serialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QLine:
				{
					QLine val;
					OSerialize<NODE, QLine>::serialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QLineF:
				{
					QLineF val;
					OSerialize<NODE, QLineF>::serialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QPoint:
				{
					QPoint val;
					OSerialize<NODE, QPoint>::serialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QPointF:
				{
					QPointF val;
					OSerialize<NODE, QPointF>::serialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QVariantHash:
				{
					QVariantHash val;
					OSerialize<NODE, QVariantHash>::serialize(vNodeValue, val);
					tValue = val;
					break;
				}
#ifdef QT_GUI_LIB
				case QMetaType::QFont:
				{
					QFont val;
					OSerialize<NODE, QFont>::serialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QPolygon:
				{
					QPolygon val;
					OSerialize<NODE, QPolygon>::serialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QPolygonF:
				{
					QPolygonF val;
					OSerialize<NODE, QPolygonF>::serialize(vNodeValue, val);
					tValue = val;
					break;
				}
#endif
				default:
				{
					QByteArray baTemp;
					OSerialize<NODE, QByteArray>::serialize(vNodeValue, baTemp);
					QDataStream ds(&baTemp, QIODevice::ReadOnly);
					ds >> tValue;
					break;
				}
				}
			}
		}
	};
}

#endif // MSARCHIVE_QT_H__
