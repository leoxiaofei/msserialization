#include <msadapter/qjsonserializer.hpp>
#include <msserialize/mssceneelem.hpp>
#include <msserialize/msnodeapt.hpp>

#include <QSharedPointer>
#include <QDebug>
#include <QVector>


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
		unsigned long long ull;
		unsigned long ul;
		long l;
		short s;
		unsigned short us;

		A() : item(0) {}

		bool operator == (const A& other) const
		{
			return i == other.i
				&& d == other.d
				&& str == other.str
				&& ull == other.ull
				&& ul == ul
				&& l == l
				&& s == s
				&& us == us;
		}
	};

	class B
	{
	public:
		QVector<A> szA;
		int szN[10];

		bool operator == (const B& other) const
		{
			return szA == other.szA && memcmp(szN, other.szN, sizeof(szN)) == 0;
		}
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
		ar.io("ull", tValue.ull);
		ar.io("ul", tValue.ul);
		ar.io("l", tValue.l);
		ar.io("s", tValue.s);
		ar.io("us", tValue.us);
		ar.io("test", MSRPC::EnumApt<EnumTest>(tValue.eTest, szEnumTest));
	}

	template<class Ar>
	void ex_serialize(Ar& ar, B& tValue)
	{
		ar.io("A", tValue.szA);
		ar.io("N", tValue.szN);
	}

}

int main()
{
	A a;
	a.i = 123;
	a.d = 3.21;
	a.ull = 1800000000000000000;
	a.str = "aaa";
	a.eTest = E_T1;
	a.item = QSharedPointer<QGraphicsRectItem>(new QGraphicsRectItem);
	a.item->setPos(10, 20);

	B b;
	b.szA.append(a);
	b.szA.append(a);

	/// 序列化
	QByteArray ba = MSRPC::ToJsonS(b);
	qDebug() << ba.data();


	/// 反序列化
	B b2;
	MSRPC::FromJsonS(b2, ba);

	Q_ASSERT(b == b2);
	qDebug() << b2.szA.size() << " " << b2.szA[0].str;

	return 0;
}
