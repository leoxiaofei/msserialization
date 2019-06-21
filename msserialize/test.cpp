#include "msarchive_qt.hpp"
#include "qjsonnode.hpp"
#include "msnodeapt_qt.hpp"


#include <QSharedPointer>
#include <QGraphicsItem>
#include <QDebug>
#include <QVector>
#include "msnodeapt.hpp"

namespace MSRPC
{

	template<class NODE>
	class ISerialize<NODE, QSharedPointer<QGraphicsItem>>
	{
	public:
		static void serialize(NODE& vNewNode, const QSharedPointer<QGraphicsItem>& tValue)
		{
			if (tValue && tValue->type() == QGraphicsRectItem::Type)
			{
				IArchiveHelper<NODE> ar(vNewNode);
				int nType = tValue->type();
				ar.io("type", nType);

				QGraphicsRectItem* pItem = (QGraphicsRectItem*)(tValue.data());
				ISerialize<NODE, QGraphicsRectItem*>::serialize(vNewNode, pItem);
			}
		}
	};

	template<class NODE>
	class OSerialize<NODE, QSharedPointer<QGraphicsItem> >
	{
	public:
		static void serialize(NODE& vNewNode, QSharedPointer<QGraphicsItem>& tValue)
		{
			if (!tValue)
			{
				int nType;
				OArchiveHelper<NODE> ar(vNewNode);
				ar.io("type", nType);
				if (nType == QGraphicsRectItem::Type)
				{
					QGraphicsRectItem* p = 0;
					OSerialize<NODE, QGraphicsRectItem*>::serialize(vNewNode, p);
					tValue = QSharedPointer<QGraphicsRectItem>(p);
				}
			}
		}
	};
}

namespace
{
	enum EnumTest { E_T1, E_T2, E_T3 };
	const char* szEnumTest[] = { "E_T1", "E_T2", "E_T3" };

	class A
	{
	public:
		int i;
		double d;
		QString str;
		EnumTest eTest;
		QSharedPointer<QGraphicsItem> item;
		long long l;

		A() : item(0) {}
	};

	class B
	{
	public:
		QVector<A> szA;
		int szN[10];
	};

	template<class Ar>
	void ex_serialize(Ar& ar, QGraphicsRectItem& tValue)
	{
		ar.io("pos", MSRPC::QtPosApt<QGraphicsRectItem>(tValue));
		ar.io("rect", MSRPC::QtRectApt<QGraphicsRectItem>(tValue));
	}


	template<class Ar>
	void ex_serialize(Ar& ar, A& tValue)
	{
		ar.io("i", tValue.i);
		ar.io("d", tValue.d);
		ar.io("str", tValue.str);
		ar.io("item", tValue.item);
		ar.io("l", tValue.l);
		ar.io("test", MSRPC::EnumApt<EnumTest>(tValue.eTest, szEnumTest));
	}

	template<class Ar>
	void ex_serialize(Ar& ar, B& tValue)
	{
		ar.io("A", tValue.szA);
		ar.io("N", tValue.szN);
	}

}

#include <QJsonDocument>

class JsonTest
{
public:
	JsonTest()
	{
		A a;
		a.i = 123;
		a.d = 3.21;
		a.str = "aaa";
		a.eTest = E_T1;
		a.item = QSharedPointer<QGraphicsRectItem>(new QGraphicsRectItem);
		a.item->setPos(10, 20);

		B b;
		b.szA.append(a);
		b.szA.append(a);

		MSRPC::IJsonArc::Node nObjI;
		MSRPC::IJsonArc ia(nObjI);
		ia & b;

		QJsonObject obj = nObjI.data().toObject();
		QJsonDocument jd(obj);
		QByteArray ba = jd.toJson();

		B c;

		QJsonDocument od = QJsonDocument::fromJson(ba);
		MSRPC::OJsonArc::Node objO(od.object());
		MSRPC::OJsonArc oa(objO);
		oa & c;

		qDebug() << c.szA.size();
	}
}a;
