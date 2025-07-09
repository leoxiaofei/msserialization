#include "msadapter/rajsonserializer.hpp"
#include <gtest/gtest.h>

class BaseType
{
public:
    char c = 1;
    bool b = true;
    short s = 2;
    int i = 3;
    long long l = 40000000000;
    float f = 5.1f;
    double d = 60000000.2;
};

SiExSe(BaseType, c, b, s, i, l, f, d)

TEST(RaJson, bt2json)
{
    BaseType bt;
    std::string strJson = MSRPC::ToJsonS(bt);

    rapidjson::Document doc;
    doc.Parse(strJson.data(), strJson.size());
    EXPECT_EQ(doc.HasParseError(), false);
    EXPECT_EQ(doc.IsObject(), true);

    auto ndItem = &doc["c"];
    EXPECT_EQ(ndItem->IsInt(), true);
    EXPECT_EQ(ndItem->GetInt(), 1);

    ndItem = &doc["b"];
    EXPECT_EQ(ndItem->IsBool(), true);
    EXPECT_EQ(ndItem->GetBool(), true);

    ndItem = &doc["s"];
    EXPECT_EQ(ndItem->IsInt(), true);
    EXPECT_EQ(ndItem->GetInt(), 2);

    ndItem = &doc["i"];
    EXPECT_EQ(ndItem->IsInt(), true);
    EXPECT_EQ(ndItem->GetInt(), 3);

    ndItem = &doc["l"];
    EXPECT_EQ(ndItem->IsInt64(), true);
    EXPECT_EQ(ndItem->GetInt64(), 40000000000ll);

    ndItem = &doc["f"];
    EXPECT_EQ(ndItem->IsFloat(), true);
    EXPECT_EQ(ndItem->GetFloat(), 5.1f);

    ndItem = &doc["d"];
    EXPECT_EQ(ndItem->IsDouble(), true);
    EXPECT_EQ(ndItem->GetDouble(), 60000000.2);
}

TEST(RaJson, json2bt)
{
    const std::string s = R"({"c":2,"b":false,"s":3,"i":4,"l":50000000000,"f":6.1,"d":70000000.2})";
    BaseType bt;
    MSRPC::FromJsonS(bt, s);

    EXPECT_EQ(bt.c, 2);
    EXPECT_EQ(bt.b, false);
    EXPECT_EQ(bt.s, 3);
    EXPECT_EQ(bt.i, 4);
    EXPECT_EQ(bt.l, 50000000000ll);
    EXPECT_EQ(bt.f, 6.1f);
    EXPECT_EQ(bt.d, 70000000.2);
}

class StringType
{
public:
    char sz1[10] = "abcdefgh";
    char sz2[10] = "123456789";
    const char* sp = "sp";
    std::string str = "str";
};

SiExSe(StringType, sz1, sz2, sp, str)

TEST(RaJson, st2json)
{
    StringType st;
    std::string strJson = MSRPC::ToJsonS(st);

    std::string s = R"({"sz1":"abcdefgh","sz2":"123456789","sp":"sp","str":"str"})";
    EXPECT_EQ(strJson, s);
}

TEST(RaJson, json2st)
{
    StringType st;
    std::string s = R"({"sz1":"abcdefgh","sz2":"123456789","sp":"sp","str":"str"})";
    st.sp = nullptr;
    MSRPC::FromJsonS(st, s);

    EXPECT_STREQ(st.sz1, "abcdefgh");
    EXPECT_STREQ(st.sz2, "123456789");
    EXPECT_STREQ(st.sp, "sp");
    EXPECT_EQ(st.str, "str");
}

class TypeBase
{
public:
    virtual ~TypeBase() {}
    virtual int Type() = 0;

    std::string name = "Type";
};

SiExSe(TypeBase, name);

class Type1 : public TypeBase
{
public:
    int Type() override { return 1; }

    char c = 1;
    bool b = true;
};

SiExSeInhe(Type1, TypeBase, c, b);

class Type2 : public TypeBase
{
public:
    int Type() override { return 2; }

    long long l = 40000000000;
    float f = 5.1f;
};

SiExSeInhe(Type2, TypeBase, l, f);

class Type3 : public TypeBase
{
public:
    int Type() override { return 3; }

    double d = 60000000.2;
    std::string str = "str";
};

SiExSeInhe(Type3, TypeBase, d, str);

BaExSe(TypeBase);

template<class Ar>
void ex_serialize(Ar& ar, TypeBase *&tValue)
{
    int32_t type = tValue ? tValue->Type() : 0;
    ar.io("type", type);

	typedef void (*TypeBaseConvT)(Ar &ar, TypeBase *&tValue);
    static std::map<int32_t, TypeBaseConvT> map = {
        {1, TypeBaseConv<Ar, Type1>},
        {2, TypeBaseConv<Ar, Type2>},
        {3, TypeBaseConv<Ar, Type3>},
    };

    auto iFind = map.find(type);
    if (iFind != map.end())
    {
        (iFind->second)(ar, tValue);
    }
}

class PointerType
{ 
public:
    std::unique_ptr<int> pI;
    Type1* pT1 = nullptr;
    std::shared_ptr<Type2> pT2;
    std::unique_ptr<Type3> pT3;
    std::shared_ptr<TypeBase> pBT1;
    std::shared_ptr<TypeBase> pBT2;
    std::shared_ptr<TypeBase> pBT3;
};

SiExSe(PointerType, pI, pT1, pT2, pT3, pBT1, pBT2, pBT3);

TEST(RaJson, ptr2json)
{
    PointerType pt;
    pt.pT1 = new Type1;
    pt.pT2 = std::make_shared<Type2>();
    pt.pT3 = std::make_unique<Type3>();
    pt.pBT1 = std::make_shared<Type1>();
    pt.pBT2 = std::make_shared<Type2>();
    pt.pBT3 = std::make_shared<Type3>();
    pt.pI = std::make_unique<int>(10);

    pt.pT1->name = "pT1";
    pt.pT2->name = "pT2";
    pt.pT3->name = "pT3";

    pt.pBT1->name = "pBT1";
    std::static_pointer_cast<Type1>(pt.pBT1)->c = 20;
    pt.pBT2->name = "pBT2";
    std::static_pointer_cast<Type2>(pt.pBT2)->l = 30;
    pt.pBT3->name = "pBT3";
    std::static_pointer_cast<Type3>(pt.pBT3)->d = 40;

    std::string strJson = MSRPC::ToJsonS(pt);

    std::string s = R"({"pI":10,"pT1":{"name":"pT1","c":1,"b":true},"pT2":{"name":"pT2","l":40000000000,"f":5.099999904632568},"pT3":{"name":"pT3","d":60000000.2,"str":"str"},"pBT1":{"type":1,"name":"pBT1","c":20,"b":true},"pBT2":{"type":2,"name":"pBT2","l":30,"f":5.099999904632568},"pBT3":{"type":3,"name":"pBT3","d":40.0,"str":"str"}})";
    EXPECT_EQ(strJson, s);

    delete pt.pT1;
}

TEST(RaJson, json2ptr)
{
    PointerType pt;
    std::string s = R"({"pI":10,"pT1":{"name":"pT1","c":1,"b":true},"pT2":{"name":"pT2","l":40000000000,"f":5.099999904632568},"pT3":{"name":"pT3","d":60000000.2,"str":"str"},"pBT1":{"type":1,"name":"pBT1","c":20,"b":true},"pBT2":{"type":2,"name":"pBT2","l":30,"f":5.099999904632568},"pBT3":{"type":3,"name":"pBT3","d":40.0,"str":"str"}})";

    MSRPC::FromJsonS(pt, s);

    EXPECT_EQ(*pt.pI, 10);
    EXPECT_EQ(pt.pT1->name, "pT1");
    EXPECT_EQ(pt.pT2->name, "pT2");
    EXPECT_EQ(pt.pT3->name, "pT3");
    EXPECT_EQ(pt.pBT1->name, "pBT1");
    EXPECT_EQ(pt.pBT2->name, "pBT2");
    EXPECT_EQ(pt.pBT3->name, "pBT3");
    EXPECT_EQ(std::static_pointer_cast<Type1>(pt.pBT1)->c, 20);
    EXPECT_EQ(std::static_pointer_cast<Type2>(pt.pBT2)->l, 30);
    EXPECT_EQ(std::static_pointer_cast<Type3>(pt.pBT3)->d, 40);

    delete pt.pT1;
}

class ContType
{
public:
    std::vector<int32_t> vecN32;
    std::list<uint16_t> lsU16;

    std::deque<Type1> deqT1;

    std::set<float> stF;
    std::unordered_set<uint32_t> ustU32;
    std::unordered_multiset<std::string> umstStrN8;

    std::map<std::string, int8_t> mapStrN8;
    std::map<double, uint64_t> mapDN64;

    std::unordered_map<std::string, Type2> hsStrT2;
    std::unordered_map<double, std::string> hsDStr;

    std::unordered_multimap<std::string, double> ummapStrD;
    std::unordered_multimap<double, Type3> ummapDT3;
};

SiExSe(ContType, vecN32, lsU16, deqT1, stF, ustU32, umstStrN8, mapStrN8, mapDN64, hsStrT2, hsDStr, ummapStrD, ummapDT3)


TEST(RaJson, cont2json)
{
    ContType ct;

    ct.vecN32 = {1,2,3,4};
    ct.lsU16 = {5,6,7};

    for (int i = 0; i < 3; i++)
    {
        Type1 t;
        t.name = "name" + std::to_string(i);
        t.c = i;
        ct.deqT1.emplace_back(std::move(t));
    }
    
    ct.stF = {8.1f, 9.2f, 10.3f};
    ct.ustU32 = {11,12,13,14};
    ct.umstStrN8 = {"15", "16", "16", "17"};

    ct.mapStrN8 = {{"aaa", 1}, {"bbb", 2}};
    ct.mapDN64 = {{12,22}, {13,33}};

    for (size_t i = 0; i < 5; i++)
    {
        auto& t = ct.hsStrT2[std::to_string(i)];
        t.f = i * 5;
        t.name = "hsStrT2_" + std::to_string(i);
    }
    
    ct.hsDStr = {{21, "21"}, {22, "22"}};

    ct.ummapStrD = {{"aaa", 1.1}, {"aaa", 4.4}, {"bbb", 2.2}, {"ccc", 3.3}};

    ct.ummapDT3 = {{1.1, {}}, {3.3, {}}, {2.2, {}}, {3.3, {}}};

    std::string strJson = MSRPC::ToJsonS(ct);

    std::string s = R"({"vecN32":[1,2,3,4],"lsU16":[5,6,7],"deqT1":[{"name":"name0","c":0,"b":true},{"name":"name1","c":1,"b":true},{"name":"name2","c":2,"b":true}],"stF":[8.100000381469727,9.199999809265137,10.300000190734863],"ustU32":[11,12,13,14],"umstStrN8":["15","16","16","17"],"mapStrN8":{"aaa":1,"bbb":2},"mapDN64":[[12.0,22],[13.0,33]],"hsStrT2":{"0":{"name":"hsStrT2_0","l":40000000000,"f":0.0},"1":{"name":"hsStrT2_1","l":40000000000,"f":5.0},"2":{"name":"hsStrT2_2","l":40000000000,"f":10.0},"3":{"name":"hsStrT2_3","l":40000000000,"f":15.0},"4":{"name":"hsStrT2_4","l":40000000000,"f":20.0}},"hsDStr":[[21.0,"21"],[22.0,"22"]],"ummapStrD":[["aaa",1.1],["aaa",4.4],["bbb",2.2],["ccc",3.3]],"ummapDT3":[[1.1,{"name":"Type","d":60000000.2,"str":"str"}],[3.3,{"name":"Type","d":60000000.2,"str":"str"}],[3.3,{"name":"Type","d":60000000.2,"str":"str"}],[2.2,{"name":"Type","d":60000000.2,"str":"str"}]]})";
    EXPECT_EQ(strJson, s);
}

// TEST(RaJson, json2cont)
// {
//     ContType pt;
//     std::string s = R"({"pI":10,"pT1":{"name":"pT1","c":1,"b":true},"pT2":{"name":"pT2","l":40000000000,"f":5.099999904632568},"pT3":{"name":"pT3","d":60000000.2,"str":"str"},"pBT1":{"type":1,"name":"pBT1","c":20,"b":true},"pBT2":{"type":2,"name":"pBT2","l":30,"f":5.099999904632568},"pBT3":{"type":3,"name":"pBT3","d":40.0,"str":"str"}})";

//     MSRPC::FromJsonS(pt, s);

//     EXPECT_EQ(*pt.pI, 10);
//     EXPECT_EQ(pt.pT1->name, "pT1");
//     EXPECT_EQ(pt.pT2->name, "pT2");
//     EXPECT_EQ(pt.pT3->name, "pT3");
//     EXPECT_EQ(pt.pBT1->name, "pBT1");
//     EXPECT_EQ(pt.pBT2->name, "pBT2");
//     EXPECT_EQ(pt.pBT3->name, "pBT3");
//     EXPECT_EQ(std::static_pointer_cast<Type1>(pt.pBT1)->c, 20);
//     EXPECT_EQ(std::static_pointer_cast<Type2>(pt.pBT2)->l, 30);
//     EXPECT_EQ(std::static_pointer_cast<Type3>(pt.pBT3)->d, 40);

//     delete pt.pT1;
// }