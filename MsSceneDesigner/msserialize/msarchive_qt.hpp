#ifndef MSARCHIVE_QT_H__
#define MSARCHIVE_QT_H__

#include <QString>

namespace MSRPC
{
	template<class NODE>
	class ISerialize<NODE, QString>
	{
	public:
		static void serialize(NODE& vNewNode, QString& tValue)
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
		static void serialize(NODE& vNewNode, QString& tValue)
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
		static void serialize(NODE& vNewNode, QVector<T>& tValue)
		{
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
		static void serialize(NODE& vNewNode, QVector<T>& tValue)
		{
			typename NODE::ITER itor = vNewNode.sub_node();
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
		static void serialize(NODE& vNewNode, QList<T>& tValue)
		{
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
		static void serialize(NODE& vNewNode, QList<T>& tValue)
		{
			typename NODE::ITER itor = vNewNode.sub_node();
			for (; itor; ++itor)
			{
				T t;
				OSerialize<NODE, T>::serialize(*itor, t);
				tValue.append(t);
			}
		}
	};

	template<class NODE, typename T>
	class ISerialize<NODE, QSharedPointer<T>>
	{
	public:
		static void serialize(NODE& vNewNode, QSharedPointer<T>& tValue)
		{
			ISerialize<NODE, T>::serialize(vNewNode, *tValue);
		}
	};

	template<class NODE, typename T>
	class OSerialize<NODE, QSharedPointer<T>>
	{
	public:
		static void serialize(NODE& vNewNode, QSharedPointer<T>& tValue)
		{
			tValue = QSharedPointer<T>(new T);
			OSerialize<NODE, T>::serialize(vNewNode, *tValue);
		}
	};


}

#endif // MSARCHIVE_QT_H__
