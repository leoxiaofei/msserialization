#ifndef MSARCHIVE_QT_H__
#define MSARCHIVE_QT_H__

#include "msarchive.hpp"
#include "msbasetypeapt_qt.hpp"
#include <QString>
#include <QTextStream>
#include <QDataStream>
#include <QSharedPointer>
#include <QDateTime>
#include <QMetaType>
#include <QSet>
#include <QQueue>
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
	class Serializer<NODE, QString>
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

		static void deserialize(const NODE& vNewNode, QString& tValue)
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
	class Serializer<NODE, QVector<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const QVector<T>& tValue)
		{
			vNewNode.set_array();
			for (int ix = 0; ix != tValue.size(); ++ix)
			{
				NODE vNode = vNewNode.new_node();
				Serializer<NODE, T>::serialize(vNode, tValue[ix]);
				vNewNode.push_node(vNode);
			}
		}

		static void deserialize(const NODE& vNewNode, QVector<T>& tValue)
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
					Serializer<NODE, T>::deserialize(node, t);
				}
			}
			if (ix != tValue.size())
			{
				tValue.resize(ix);
			}
		}
	};

	template<class NODE, class T>
	class Serializer<NODE, QList<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const QList<T>& tValue)
		{
			vNewNode.set_array();
			for (int ix = 0; ix != tValue.size(); ++ix)
			{
				NODE vNode = vNewNode.new_node();
				Serializer<NODE, T>::serialize(vNode, tValue[ix]);
				vNewNode.push_node(vNode);
			}
		}

		static void deserialize(const NODE& vNewNode, QList<T>& tValue)
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
					Serializer<NODE, T>::deserialize(node, t);
				}
			}

			if (ix != tValue.size())
			{
				tValue.erase(tValue.begin() + ix, tValue.end());
			}
		}
	};

	template<class NODE, class T>
	class Serializer<NODE, QQueue<T> > : public Serializer<NODE, QList<T> >
	{
	public:
	};
 
	template<class NODE, class T>
	class Serializer<NODE, QHash<QString, T> >
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
				Serializer<NODE, T>::serialize(vNode, *citor);
				vNewNode.add_member(baKey.data(), vNode);
			}
		}

		static void deserialize(const NODE& vNewNode, QHash<QString, T>& tValue)
		{
			QSet<QString> setKey;

			typename NODE::ObjIter itor = vNewNode.sub_members();
			for (; itor; ++itor)
			{
				if (NODE node = *itor)
				{
					T& t = tValue[itor.key()];
					Serializer<NODE, T>::deserialize(*itor, t);
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
	class Serializer<NODE, QHash<K, T> >
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
					Serializer<NODE, K>::serialize(vKNode, citor.key());
					vKeyNode.push_node(vKNode);

					NODE vVNode = vValueNode.new_node();
					Serializer<NODE, T>::serialize(vVNode, citor.value());
					vValueNode.push_node(vVNode);
				}

				vNewNode.add_member("key", vKeyNode);
				vNewNode.add_member("value", vValueNode);
			}
		}

		static void deserialize(const NODE& vNewNode, QHash<K, T>& tValue)
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
						Serializer<NODE, K>::deserialize(nodeKey, k);
						T& t = tValue[k];
						Serializer<NODE, T>::deserialize(nodeVal, t);
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
	class Serializer<NODE, QMap<QString, T> >
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
				Serializer<NODE, T>::serialize(vNode, *citor);
				vNewNode.add_member(baKey.data(), vNode);
			}
		}

		static void deserialize(const NODE& vNewNode, QMap<QString, T>& tValue)
		{
			QSet<QString> setKey;

			typename NODE::ObjIter itor = vNewNode.sub_members();
			for (; itor; ++itor)
			{
				if (NODE node = *itor)
				{
					T& t = tValue[itor.key()];
					Serializer<NODE, T>::deserialize(node, t);
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
	class Serializer<NODE, QMap<K, T> >
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
					Serializer<NODE, K>::serialize(vKNode, citor.key());
					vKeyNode.push_node(vKNode);

					NODE vVNode = vValueNode.new_node();
					Serializer<NODE, T>::serialize(vVNode, citor.value());
					vValueNode.push_node(vVNode);
				}

				vNewNode.add_member("key", vKeyNode);
				vNewNode.add_member("value", vValueNode);
			}
		}

		static void deserialize(const NODE& vNewNode, QMap<K, T>& tValue)
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
						Serializer<NODE, K>::deserialize(nodeKey, k);
						T& t = tValue[k];
						Serializer<NODE, T>::deserialize(nodeVal, t);
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
	class Serializer<NODE, QSharedPointer<T>>
	{
	public:
		static void serialize(NODE& vNewNode, const QSharedPointer<T>& tValue)
		{
			Serializer<NODE, T*>::serialize(vNewNode, tValue.data());
		}

		static void deserialize(const NODE& vNewNode, QSharedPointer<T>& tValue)
		{
			T* pRet = 0;
			Serializer<NODE, T*>::deserialize(vNewNode, pRet);
			tValue = QSharedPointer<T>(pRet);
		}
	};

#define DefQStringBaseType(TYPE) \
	template<class NODE> \
	class Serializer<NODE, TYPE> : public Serializer<NODE, BaseTypeApt<QString, TYPE>> { }; \


	DefQStringBaseType(QColor)
	DefQStringBaseType(QFont)
	DefQStringBaseType(QBrush)
	DefQStringBaseType(QPen)
	DefQStringBaseType(QPointF)
	DefQStringBaseType(QPoint)
	DefQStringBaseType(QSizeF)
	DefQStringBaseType(QSize)
	DefQStringBaseType(QRectF)
	DefQStringBaseType(QRect)

	DefQStringBaseType(QMarginsF)
	DefQStringBaseType(QMargins)
	DefQStringBaseType(QLineF)
	DefQStringBaseType(QLine)

	DefQStringBaseType(QDate)
	DefQStringBaseType(QTime)
	DefQStringBaseType(QDateTime)
	DefQStringBaseType(QByteArray)

#ifdef QT_GUI_LIB
	template<class NODE>
	class Serializer<NODE, QPolygonF> : public Serializer<NODE, QVector<QPointF> > { };

	template<class NODE>
	class Serializer<NODE, QPolygon> : public Serializer<NODE, QVector<QPoint> > { };

#endif

	template<class NODE>
	class Serializer<NODE, QStringList> : public Serializer<NODE, QList<QString> > { };

	template<class NODE>
	class Serializer<NODE, QVariant>
	{
	public:
		static void serialize(NODE& vNewNode, const QVariant& tValue)
		{
			vNewNode.set_object();

			NODE vNodeType = vNewNode.new_node();
			Serializer<NODE, const char*>::serialize(vNodeType, tValue.typeName());
			vNewNode.add_member("type", vNodeType);

			NODE vNodeValue = vNewNode.new_node();

			switch (tValue.type())
			{
			case QMetaType::Bool:
				Serializer<NODE, bool>::serialize(vNodeValue, tValue.toBool());
				break;
			case QMetaType::Int:
				Serializer<NODE, int>::serialize(vNodeValue, tValue.toInt());
				break;
			case QMetaType::UInt:
				Serializer<NODE, unsigned int>::serialize(vNodeValue, tValue.toUInt());
				break;
			case QMetaType::LongLong:
				Serializer<NODE, long long>::serialize(vNodeValue, tValue.toLongLong());
				break;
			case QMetaType::ULongLong:
				Serializer<NODE, unsigned long long>::serialize(vNodeValue, tValue.toULongLong());
				break;
			case QMetaType::Double:
				Serializer<NODE, double>::serialize(vNodeValue, tValue.toDouble());
				break;
			case QMetaType::QVariantMap:
				Serializer<NODE, QVariantMap>::serialize(
					vNodeValue, *static_cast<const QVariantMap*>(tValue.data()));
				break;
			case QMetaType::QVariantList:
				Serializer<NODE, QVariantList>::serialize(
					vNodeValue, *static_cast<const QVariantList*>(tValue.data()));
				break;
			case QMetaType::QString:
				Serializer<NODE, QString>::serialize(
					vNodeValue, *static_cast<const QString*>(tValue.data()));
				break;
			case QMetaType::QStringList:
				Serializer<NODE, QStringList>::serialize(
					vNodeValue, *static_cast<const QStringList*>(tValue.data()));
				break;
			case QMetaType::QByteArray:
				Serializer<NODE, QByteArray>::serialize(
					vNodeValue, *static_cast<const QByteArray*>(tValue.data()));
				break;
			case QMetaType::QDate:
				Serializer<NODE, QDate>::serialize(
					vNodeValue, *static_cast<const QDate*>(tValue.data()));
				break;
			case QMetaType::QTime:
				Serializer<NODE, QTime>::serialize(
					vNodeValue, *static_cast<const QTime*>(tValue.data()));
				break;
			case QMetaType::QDateTime:
				Serializer<NODE, QDateTime>::serialize(
					vNodeValue, *static_cast<const QDateTime*>(tValue.data()));
				break;
			case QMetaType::QRect:
				Serializer<NODE, QRect>::serialize(
					vNodeValue, *static_cast<const QRect*>(tValue.data()));
				break;
			case QMetaType::QRectF:
				Serializer<NODE, QRectF>::serialize(
					vNodeValue, *static_cast<const QRectF*>(tValue.data()));
				break;
			case QMetaType::QSize:
				Serializer<NODE, QSize>::serialize(
					vNodeValue, *static_cast<const QSize*>(tValue.data()));
				break;
			case QMetaType::QSizeF:
				Serializer<NODE, QSizeF>::serialize(
					vNodeValue, *static_cast<const QSizeF*>(tValue.data()));
				break;
			case QMetaType::QLine:
				Serializer<NODE, QLine>::serialize(
					vNodeValue, *static_cast<const QLine*>(tValue.data()));
				break;
			case QMetaType::QLineF:
				Serializer<NODE, QLineF>::serialize(
					vNodeValue, *static_cast<const QLineF*>(tValue.data()));
				break;
			case QMetaType::QPoint:
				Serializer<NODE, QPoint>::serialize(
					vNodeValue, *static_cast<const QPoint*>(tValue.data()));
				break;
			case QMetaType::QPointF:
				Serializer<NODE, QPointF>::serialize(
					vNodeValue, *static_cast<const QPointF*>(tValue.data()));
				break;
			case QMetaType::QVariantHash:
				Serializer<NODE, QVariantHash>::serialize(
					vNodeValue, *static_cast<const QVariantHash*>(tValue.data()));
				break;
#ifdef QT_GUI_LIB
			case QMetaType::QFont:
				Serializer<NODE, QFont>::serialize(
					vNodeValue, *static_cast<const QFont*>(tValue.data()));
				break;
			case QMetaType::QPolygon:
				Serializer<NODE, QPolygon>::serialize(
					vNodeValue, *static_cast<const QPolygon*>(tValue.data()));
				break;
			case QMetaType::QPolygonF:
				Serializer<NODE, QPolygonF>::serialize(
					vNodeValue, *static_cast<const QPolygonF*>(tValue.data()));
				break;
#endif
			default:
			{
				QByteArray ba;
				QDataStream ds(&ba, QIODevice::WriteOnly);
				ds << tValue;
				Serializer<NODE, QByteArray>::serialize(vNodeValue, ba);
				break;
			}
			}

			vNewNode.add_member("value", vNodeValue);

		}

		static void deserialize(const NODE& vNewNode, QVariant& tValue)
		{
			NODE vNodeType = vNewNode.sub_member("type");
			NODE vNodeValue = vNewNode.sub_member("value");
			if (vNodeType && vNodeValue)
			{
				QString baData;
				vNodeType.in_serialize(baData);
				int nType = QVariant::nameToType(baData.toUtf8().data());

				switch (nType)
				{
				case QMetaType::Bool:
				{
					bool val = false;
					Serializer<NODE, bool>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::Int:
				{
					int val = 0;
					Serializer<NODE, int>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::UInt:
				{
					unsigned int val = 0;
					Serializer<NODE, unsigned int>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::LongLong:
				{
					long long val = 0;
					Serializer<NODE, long long>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::ULongLong:
				{
					unsigned long long val = 0;
					Serializer<NODE, unsigned long long>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::Double:
				{
					double val = 0;
					Serializer<NODE, double>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QVariantMap:
				{
					QVariantMap val;
					Serializer<NODE, QVariantMap>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QVariantList:
				{
					QVariantList val;
					Serializer<NODE, QVariantList>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QString:
				{
					QString val;
					Serializer<NODE, QString>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QStringList:
				{
					QStringList val;
					Serializer<NODE, QStringList>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QByteArray:
				{
					QByteArray val;
					Serializer<NODE, QByteArray>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QDate:
				{
					QDate val;
					Serializer<NODE, QDate>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QTime:
				{
					QTime val;
					Serializer<NODE, QTime>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QDateTime:
				{
					QDateTime val;
					Serializer<NODE, QDateTime>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QRect:
				{
					QRect val;
					Serializer<NODE, QRect>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QRectF:
				{
					QRectF val;
					Serializer<NODE, QRectF>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QSize:
				{
					QSize val;
					Serializer<NODE, QSize>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QSizeF:
				{
					QSizeF val;
					Serializer<NODE, QSizeF>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QLine:
				{
					QLine val;
					Serializer<NODE, QLine>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QLineF:
				{
					QLineF val;
					Serializer<NODE, QLineF>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QPoint:
				{
					QPoint val;
					Serializer<NODE, QPoint>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QPointF:
				{
					QPointF val;
					Serializer<NODE, QPointF>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QVariantHash:
				{
					QVariantHash val;
					Serializer<NODE, QVariantHash>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
#ifdef QT_GUI_LIB
				case QMetaType::QFont:
				{
					QFont val;
					Serializer<NODE, QFont>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QPolygon:
				{
					QPolygon val;
					Serializer<NODE, QPolygon>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QPolygonF:
				{
					QPolygonF val;
					Serializer<NODE, QPolygonF>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
#endif
				default:
				{
					QByteArray baTemp;
					Serializer<NODE, QByteArray>::deserialize(vNodeValue, baTemp);
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
