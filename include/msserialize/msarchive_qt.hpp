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
	template<>
	class Serializer<QString>
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const QString& tValue)
		{
#ifdef NODE_WITH_QT
			vNewNode.in_serialize(tValue);
#else
			QByteArray baBuffer = tValue.toUtf8();
			vNewNode.in_serialize(baBuffer.data());
#endif
		}

		template<class NODE>
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

	template<class T>
	class Serializer<QVector<T> >
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const QVector<T>& tValue)
		{
			vNewNode.set_array();
			for (int ix = 0; ix != tValue.size(); ++ix)
			{
				NODE vNode = vNewNode.new_node();
				Serializer<T>::serialize(vNode, tValue[ix]);
				vNewNode.push_node(vNode);
			}
		}

		template<class NODE>
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
					Serializer<T>::deserialize(node, t);
				}
			}
			if (ix != tValue.size())
			{
				tValue.resize(ix);
			}
		}
	};

	template<class T>
	class Serializer<QList<T> >
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const QList<T>& tValue)
		{
			vNewNode.set_array();
			for (int ix = 0; ix != tValue.size(); ++ix)
			{
				NODE vNode = vNewNode.new_node();
				Serializer<T>::serialize(vNode, tValue[ix]);
				vNewNode.push_node(vNode);
			}
		}

		template<class NODE>
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
					Serializer<T>::deserialize(node, t);
				}
			}

			if (ix != tValue.size())
			{
				tValue.erase(tValue.begin() + ix, tValue.end());
			}
		}
	};

	template<class T>
	class Serializer<QQueue<T> > : public Serializer<QList<T> >
	{
	public:
	};
 
	template<class T>
	class Serializer<QHash<QString, T> >
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const QHash<QString, T>& tValue)
		{
			vNewNode.set_object();
			for (typename QHash<QString, T>::const_iterator citor = tValue.constBegin(); 
			citor != tValue.constEnd(); ++citor)
			{
				QByteArray baKey = citor.key().toUtf8();
				NODE vNode = vNewNode.new_node();
				Serializer<T>::serialize(vNode, *citor);
				vNewNode.add_member(baKey.data(), vNode);
			}
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, QHash<QString, T>& tValue)
		{
			QSet<QString> setKey;

			typename NODE::ObjIter itor = vNewNode.sub_members();
			for (; itor; ++itor)
			{
				if (NODE node = *itor)
				{
					T& t = tValue[itor.key()];
					Serializer<T>::deserialize(*itor, t);
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

	template<class K, class T>
	class Serializer<QHash<K, T> >
	{
	public:
		template<class NODE>
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
					Serializer<K>::serialize(vKNode, citor.key());
					vKeyNode.push_node(vKNode);

					NODE vVNode = vValueNode.new_node();
					Serializer<T>::serialize(vVNode, citor.value());
					vValueNode.push_node(vVNode);
				}

				vNewNode.add_member("key", vKeyNode);
				vNewNode.add_member("value", vValueNode);
			}
		}

		template<class NODE>
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
						Serializer<K>::deserialize(nodeKey, k);
						T& t = tValue[k];
						Serializer<T>::deserialize(nodeVal, t);
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

	template<class T>
	class Serializer<QMap<QString, T> >
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const QMap<QString, T>& tValue)
		{
			vNewNode.set_object();
			for (typename QMap<QString, T>::const_iterator citor = tValue.constBegin();
				citor != tValue.constEnd(); ++citor)
			{
				QByteArray baKey = citor.key().toUtf8();
				NODE vNode = vNewNode.new_node();
				Serializer<T>::serialize(vNode, *citor);
				vNewNode.add_member(baKey.data(), vNode);
			}
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, QMap<QString, T>& tValue)
		{
			QSet<QString> setKey;

			typename NODE::ObjIter itor = vNewNode.sub_members();
			for (; itor; ++itor)
			{
				if (NODE node = *itor)
				{
					T& t = tValue[itor.key()];
					Serializer<T>::deserialize(node, t);
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

	template<class K, class T>
	class Serializer<QMap<K, T> >
	{
	public:
		template<class NODE>
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
					Serializer<K>::serialize(vKNode, citor.key());
					vKeyNode.push_node(vKNode);

					NODE vVNode = vValueNode.new_node();
					Serializer<T>::serialize(vVNode, citor.value());
					vValueNode.push_node(vVNode);
				}

				vNewNode.add_member("key", vKeyNode);
				vNewNode.add_member("value", vValueNode);
			}
		}

		template<class NODE>
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
						Serializer<K>::deserialize(nodeKey, k);
						T& t = tValue[k];
						Serializer<T>::deserialize(nodeVal, t);
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

	template<typename T>
	class Serializer<QSharedPointer<T>>
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const QSharedPointer<T>& tValue)
		{
			Serializer<T*>::serialize(vNewNode, tValue.data());
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, QSharedPointer<T>& tValue)
		{
			T* pRet = 0;
			Serializer<T*>::deserialize(vNewNode, pRet);
			tValue = QSharedPointer<T>(pRet);
		}
	};

#define DefQStringBaseType(TYPE) \
	template<class NODE> \
	class Serializer<TYPE> : public Serializer<BaseTypeApt<QString, TYPE>> { }; \


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
	template<>
	class Serializer<QPolygonF> : public Serializer<QVector<QPointF> > { };

	template<>
	class Serializer<QPolygon> : public Serializer<QVector<QPoint> > { };

#endif

	template<>
	class Serializer<QStringList> : public Serializer<QList<QString> > { };

	template<>
	class Serializer<QVariant>
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const QVariant& tValue)
		{
			vNewNode.set_object();

			NODE vNodeType = vNewNode.new_node();
			Serializer<const char*>::serialize(vNodeType, tValue.typeName());
			vNewNode.add_member("type", vNodeType);

			NODE vNodeValue = vNewNode.new_node();

			switch (tValue.type())
			{
			case QMetaType::Bool:
				Serializer<bool>::serialize(vNodeValue, tValue.toBool());
				break;
			case QMetaType::Int:
				Serializer<int>::serialize(vNodeValue, tValue.toInt());
				break;
			case QMetaType::UInt:
				Serializer<unsigned int>::serialize(vNodeValue, tValue.toUInt());
				break;
			case QMetaType::LongLong:
				Serializer<long long>::serialize(vNodeValue, tValue.toLongLong());
				break;
			case QMetaType::ULongLong:
				Serializer<unsigned long long>::serialize(vNodeValue, tValue.toULongLong());
				break;
			case QMetaType::Double:
				Serializer<double>::serialize(vNodeValue, tValue.toDouble());
				break;
			case QMetaType::QVariantMap:
				Serializer<QVariantMap>::serialize(
					vNodeValue, *static_cast<const QVariantMap*>(tValue.data()));
				break;
			case QMetaType::QVariantList:
				Serializer<QVariantList>::serialize(
					vNodeValue, *static_cast<const QVariantList*>(tValue.data()));
				break;
			case QMetaType::QString:
				Serializer<QString>::serialize(
					vNodeValue, *static_cast<const QString*>(tValue.data()));
				break;
			case QMetaType::QStringList:
				Serializer<QStringList>::serialize(
					vNodeValue, *static_cast<const QStringList*>(tValue.data()));
				break;
			case QMetaType::QByteArray:
				Serializer<QByteArray>::serialize(
					vNodeValue, *static_cast<const QByteArray*>(tValue.data()));
				break;
			case QMetaType::QDate:
				Serializer<QDate>::serialize(
					vNodeValue, *static_cast<const QDate*>(tValue.data()));
				break;
			case QMetaType::QTime:
				Serializer<QTime>::serialize(
					vNodeValue, *static_cast<const QTime*>(tValue.data()));
				break;
			case QMetaType::QDateTime:
				Serializer<QDateTime>::serialize(
					vNodeValue, *static_cast<const QDateTime*>(tValue.data()));
				break;
			case QMetaType::QRect:
				Serializer<QRect>::serialize(
					vNodeValue, *static_cast<const QRect*>(tValue.data()));
				break;
			case QMetaType::QRectF:
				Serializer<QRectF>::serialize(
					vNodeValue, *static_cast<const QRectF*>(tValue.data()));
				break;
			case QMetaType::QSize:
				Serializer<QSize>::serialize(
					vNodeValue, *static_cast<const QSize*>(tValue.data()));
				break;
			case QMetaType::QSizeF:
				Serializer<QSizeF>::serialize(
					vNodeValue, *static_cast<const QSizeF*>(tValue.data()));
				break;
			case QMetaType::QLine:
				Serializer<QLine>::serialize(
					vNodeValue, *static_cast<const QLine*>(tValue.data()));
				break;
			case QMetaType::QLineF:
				Serializer<QLineF>::serialize(
					vNodeValue, *static_cast<const QLineF*>(tValue.data()));
				break;
			case QMetaType::QPoint:
				Serializer<QPoint>::serialize(
					vNodeValue, *static_cast<const QPoint*>(tValue.data()));
				break;
			case QMetaType::QPointF:
				Serializer<QPointF>::serialize(
					vNodeValue, *static_cast<const QPointF*>(tValue.data()));
				break;
			case QMetaType::QVariantHash:
				Serializer<QVariantHash>::serialize(
					vNodeValue, *static_cast<const QVariantHash*>(tValue.data()));
				break;
#ifdef QT_GUI_LIB
			case QMetaType::QFont:
				Serializer<QFont>::serialize(
					vNodeValue, *static_cast<const QFont*>(tValue.data()));
				break;
			case QMetaType::QPolygon:
				Serializer<QPolygon>::serialize(
					vNodeValue, *static_cast<const QPolygon*>(tValue.data()));
				break;
			case QMetaType::QPolygonF:
				Serializer<QPolygonF>::serialize(
					vNodeValue, *static_cast<const QPolygonF*>(tValue.data()));
				break;
#endif
			default:
			{
				QByteArray ba;
				QDataStream ds(&ba, QIODevice::WriteOnly);
				ds << tValue;
				Serializer<QByteArray>::serialize(vNodeValue, ba);
				break;
			}
			}

			vNewNode.add_member("value", vNodeValue);

		}

		template<class NODE>
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
					Serializer<bool>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::Int:
				{
					int val = 0;
					Serializer<int>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::UInt:
				{
					unsigned int val = 0;
					Serializer<unsigned int>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::LongLong:
				{
					long long val = 0;
					Serializer<long long>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::ULongLong:
				{
					unsigned long long val = 0;
					Serializer<unsigned long long>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::Double:
				{
					double val = 0;
					Serializer<double>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QVariantMap:
				{
					QVariantMap val;
					Serializer<QVariantMap>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QVariantList:
				{
					QVariantList val;
					Serializer<QVariantList>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QString:
				{
					QString val;
					Serializer<QString>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QStringList:
				{
					QStringList val;
					Serializer<QStringList>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QByteArray:
				{
					QByteArray val;
					Serializer<QByteArray>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QDate:
				{
					QDate val;
					Serializer<QDate>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QTime:
				{
					QTime val;
					Serializer<QTime>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QDateTime:
				{
					QDateTime val;
					Serializer<QDateTime>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QRect:
				{
					QRect val;
					Serializer<QRect>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QRectF:
				{
					QRectF val;
					Serializer<QRectF>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QSize:
				{
					QSize val;
					Serializer<QSize>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QSizeF:
				{
					QSizeF val;
					Serializer<QSizeF>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QLine:
				{
					QLine val;
					Serializer<QLine>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QLineF:
				{
					QLineF val;
					Serializer<QLineF>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QPoint:
				{
					QPoint val;
					Serializer<QPoint>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QPointF:
				{
					QPointF val;
					Serializer<QPointF>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QVariantHash:
				{
					QVariantHash val;
					Serializer<QVariantHash>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
#ifdef QT_GUI_LIB
				case QMetaType::QFont:
				{
					QFont val;
					Serializer<QFont>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QPolygon:
				{
					QPolygon val;
					Serializer<QPolygon>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
				case QMetaType::QPolygonF:
				{
					QPolygonF val;
					Serializer<QPolygonF>::deserialize(vNodeValue, val);
					tValue = val;
					break;
				}
#endif
				default:
				{
					QByteArray baTemp;
					Serializer<QByteArray>::deserialize(vNodeValue, baTemp);
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
