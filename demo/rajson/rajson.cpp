#include <msadapter/rajsonserializer.hpp>
#include <msserialize/msnodeapt.hpp>
#include <iostream>

namespace
{
	enum EnumTest { E_T1, E_T2, E_T3 };

	class A
	{
	public:
		bool b;
		int i;
		double d;
		EnumTest eTest;
		std::string str;
		char szS[10];
		long long l;
		std::shared_ptr<double> spD;
	};

	class B
	{
	public:
		std::vector<A> szA;
		std::tuple<int, std::string, double> tpT;
		int szN[10] = { 0 };
	};

	const char* szEnumTest[] = { "E_T1", "E_T2", "E_T3" };

	template<class Ar>
	void ex_serialize(Ar& ar, A& tValue)
	{
		ar.io("b", tValue.b);
		ar.io("i", tValue.i);
		ar.io("d", tValue.d);
		ar.io("l", tValue.l);
		ar.io("str", tValue.str);
		ar.io("szS", tValue.szS);
		ar.io("test", MSRPC::EnumApt<EnumTest>(tValue.eTest, szEnumTest));
		ar.io("spD", tValue.spD);
	}


	SiExSe(B, szA, tpT, szN)
};

int main()
{
	A a;
	a.i = 123;
	a.d = 3.21;
	a.str = "aaa";
	a.l = 1234567890;
	strcpy(a.szS, "bbb");
	a.eTest = E_T1;
	a.spD = std::make_shared<double>(10.1);

	B b;
	b.szA.push_back(a);
	b.szA.push_back(a);
	b.szN[0] = 1;


	std::string strBuf = MSRPC::ToJsonS(b);
	std::cout << strBuf.c_str() << std::endl;

	//////////////////////////////////////////////////////////////////////////

	B b2;
	MSRPC::FromJsonS(b2, strBuf);

	std::cout << b2.szA.size() << " " << b2.szA[0].str << std::endl;

	return 0;
}