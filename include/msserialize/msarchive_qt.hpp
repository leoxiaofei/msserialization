#ifndef MSARCHIVE_QT_H__
#define MSARCHIVE_QT_H__

#include "msarchive.hpp"
#include <QString>
#include <QTextStream>

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
			char* p = 0;
			vNewNode.in_serialize(p);
			if (p)
			{
				tValue = QString::fromUtf8(p);
				delete p;
			}
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
			typename NODE::ArrIter itor = vNewNode.sub_nodes();
			for (; itor; ++itor)
			{
				T t;
				OSerialize<NODE, T>::serialize(*itor, t);
				tValue.append(t);
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
			typename NODE::ArrIter itor = vNewNode.sub_nodes();
			for (; itor; ++itor)
			{
				T t;
				OSerialize<NODE, T>::serialize(*itor, t);
				tValue.append(t);
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
			typename NODE::ObjIter itor = vNewNode.sub_members();
			for (; itor; ++itor)
			{
				T t;
				OSerialize<NODE, T>::serialize(*itor, t);
				tValue[itor.key()] = t;
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
			NODE vKeyNode = vNewNode.sub_member("key");
			NODE vValueNode = vNewNode.sub_member("value");

			if (vKeyNode && vValueNode)
			{
				typename NODE::ArrIter itorKey = vKeyNode.sub_nodes();
				typename NODE::ArrIter itorValue = vValueNode.sub_nodes();
				for (; itorKey && itorValue; ++itorKey, ++itorValue)
				{
					K k;
					OSerialize<NODE, K>::serialize(*itorKey, k);
					T t;
					OSerialize<NODE, T>::serialize(*itorValue, t);
					tValue[k] = t;
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
				typename NODE::ArrIter itorKey = vKeyNode.sub_nodes();
				typename NODE::ArrIter itorValue = vValueNode.sub_nodes();
				for (; itorKey && itorValue; ++itorKey, ++itorValue)
				{
					K k;
					OSerialize<NODE, K>::serialize(*itorKey, k);
					T t;
					OSerialize<NODE, T>::serialize(*itorValue, t);
					tValue[k] = t;
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
			ISerialize<NODE, T>::serialize(vNewNode, *tValue);
		}
	};

	template<class NODE, typename T>
	class OSerialize<NODE, QSharedPointer<T>>
	{
	public:
		static void serialize(const NODE& vNewNode, QSharedPointer<T>& tValue)
		{
			tValue = QSharedPointer<T>(new T);
			OSerialize<NODE, T>::serialize(vNewNode, *tValue);
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
	class ISerialize<NODE, QPolygonF>
	{
	public:
		static void serialize(NODE& vNewNode, const QPolygonF& tValue)
		{
			QString strValue;
			QTextStream ts(&strValue);

			for (QPolygonF::const_iterator citor = tValue.constBegin();
				citor != tValue.constEnd(); ++citor)
			{
				ts << citor->x() << ',' << citor->y() << ' ';
			}
			
			if (strValue.endsWith(' '))
			{
				strValue.chop(1);
			}

			ISerialize<NODE, QString>::serialize(vNewNode, strValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, QPolygonF>
	{
	public:
		static void serialize(const NODE& vNewNode, QPolygonF& tValue)
		{
			QString strValue;
			OSerialize<NODE, QString>::serialize(vNewNode, strValue);
			qreal dX(0);
			qreal dY(0);
			char ch(0);
			QTextStream ts(const_cast<QString*>(&strValue));
			while (!ts.status())
			{
				ts >> dX >> ch >> dY >> ch;
				tValue.append(QPointF(dX, dY));
			}
		}
	};
}

#endif // MSARCHIVE_QT_H__
