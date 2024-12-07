#ifndef MSARCHIVE_QT_H__
#define MSARCHIVE_QT_H__

#include "msarchive.hpp"
#include "msbasetypeapt_qt.hpp"
#include <QString>
#include <QDataStream>
#include <QSharedPointer>
#include <QMetaType>
#include <QSet>
#include <QQueue>

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
	class ISerialize<NODE, QQueue<T> > : public ISerialize<NODE, QList<T> >
	{
	public:
	};
 
	template<class NODE, class T>
	class OSerialize<NODE, QQueue<T> > : public OSerialize<NODE, QList<T> >
	{
	public:
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

#define DefQStringBaseType(TYPE) \
	template<class NODE> \
	class ISerialize<NODE, TYPE> : public ISerialize<NODE, BaseTypeApt<QString, TYPE>> { }; \
	template<class NODE> \
	class OSerialize<NODE, TYPE> : public OSerialize<NODE, BaseTypeApt<QString, TYPE>> { };


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
	class ISerialize<NODE, QPolygonF> : public ISerialize<NODE, QVector<QPointF> > { };

	template<class NODE>
	class OSerialize<NODE, QPolygonF> : public OSerialize<NODE, QVector<QPointF> > { };

	template<class NODE>
	class ISerialize<NODE, QPolygon> : public ISerialize<NODE, QVector<QPoint> > { };

	template<class NODE>
	class OSerialize<NODE, QPolygon> : public OSerialize<NODE, QVector<QPoint> > { };
	
#endif


	template<class NODE>
	class ISerialize<NODE, QStringList> : public ISerialize<NODE, QList<QString> > { };

	template<class NODE>
	class OSerialize<NODE, QStringList> : public OSerialize<NODE, QList<QString> > { };

	template<class NODE>
	class ISerialize<NODE, QVariant>
	{
	public:
		static void serialize(NODE& vNewNode, const QVariant& tValue)
		{
			vNewNode.set_object();

			///保存QVariant内部数据类型
			NODE vNodeType = vNewNode.new_node();
			ISerialize<NODE, const char*>::serialize(vNodeType, tValue.typeName());
			vNewNode.add_member("type", vNodeType);

			NODE vNodeValue = vNewNode.new_node();

			///根据类型保存值
			///这里其实我想用Map，但是这是个纯头文件的库，Map的数据保存在哪里还没想清楚，
			///只好靠C++编译器优化switch了。
			switch (static_cast<QMetaType::Type>(tValue.type()))
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
