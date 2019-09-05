#include "msserialize/msarchive.hpp"
#include "msserialize/msnodeapt.hpp"
#include "msserialize/rajsonnode.hpp"
#include "msserialize/msarchive_stl.hpp"

#include "rapidjson/writer.h"

#include <iostream>

namespace
{
	enum EnumTest { E_T1, E_T2, E_T3 };
	const char* szEnumTest[] = { "E_T1", "E_T2", "E_T3" };

	class A
	{
	public:
		bool b;
		int i;
		double d;
		EnumTest eTest;
		std::string str;
		char szS[10];
		char* pS;
		long long l;
	};

	class B
	{
	public:
		std::vector<A> szA;
		int szN[10];
	};

	template<class Ar>
	void ex_serialize(Ar& ar, A& tValue)
	{
		ar.io("b", tValue.b);
		ar.io("i", tValue.i);
		ar.io("d", tValue.d);
		ar.io("l", tValue.l);
		ar.io("str", tValue.str);
		ar.io("szS", tValue.szS);
		ar.io("pS", tValue.pS);
		ar.io("test", MSRPC::EnumApt<EnumTest>(tValue.eTest, szEnumTest));
	}

	template<class Ar>
	void ex_serialize(Ar& ar, B& tValue)
	{
		ar.io("A", tValue.szA);
		ar.io("N", tValue.szN);
	}

};

int main()
{
	A a;
	a.i = 123;
	a.d = 3.21;
	a.str = "aaa";
	strcpy(a.szS, "bbb");
	a.pS = "ccc";
	a.eTest = E_T1;

	B b;
	b.szA.push_back(a);
	b.szA.push_back(a);

	rapidjson::Document d;
	MSRPC::IJsonArc::Node nObjI(d);
	MSRPC::IJsonArc ia(nObjI);
	ia & b;

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	d.Accept(writer);

	std::cout << buffer.GetString() << std::endl;


	rapidjson::Document d2;
	d2.Parse(buffer.GetString(), buffer.GetLength());

	MSRPC::OJsonArc::Node objO(d2);
	MSRPC::OJsonArc oa(objO);

	B b2;
	oa & b2;


	return 0;
}