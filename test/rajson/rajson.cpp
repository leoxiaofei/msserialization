#include "msserialize/msarchive.hpp"
#include "msserialize/msnodeapt.hpp"
#include "msserialize/rajsonnode.hpp"
#include "msserialize/msarchive_stl.hpp"

#include "rapidjson/writer.h"

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
	};

	class B
	{
	public:
		std::vector<A> szA;
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
	//赋值
	A a;
	a.i = 123;
	a.d = 3.21;
	a.str = "aaa";
	a.l = 1234567890;
	strcpy(a.szS, "bbb");
	a.eTest = E_T1;

	B b;
	b.szA.push_back(a);
	b.szA.push_back(a);
	b.szN[0] = 1;

	//序列化
	rapidjson::Document doc;
	MSRPC::IJsonArc::Node nObjI(&doc);
	MSRPC::IJsonArc ia(nObjI);
	ia & b;

	//输出json字符串
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	std::cout << buffer.GetString() << std::endl;

	//////////////////////////////////////////////////////////////////////////
	//反序列化
	rapidjson::Document doc2;
	doc2.Parse(buffer.GetString(), buffer.GetLength());

	MSRPC::OJsonArc::Node objO(&doc2);
	MSRPC::OJsonArc oa(objO);

	B b2;
	oa & b2;

	return 0;
}