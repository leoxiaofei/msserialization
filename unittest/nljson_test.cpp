#include "msadapter/nljson.hpp"
#include "stltypedef.hpp"
#include <gtest/gtest.h>

TEST(NlJson, FromJsonO)
{
    BaseType data1;
    InitData1(data1);

    std::string strJson = BaseTypeJson1();
    auto m_doc = nlohmann::json::parse(strJson);
    BaseType data2;
    MSRPC::FromJsonO(data2, m_doc);

    EXPECT_EQ(data1, data2);
}

TEST(NlJson, BaseType_Self)
{
    BaseType data1;
    InitData1(data1);

    std::string strJson = MSRPC::ToJsonS(data1);

    BaseType data2;
    MSRPC::FromJsonS(data2, strJson);

    EXPECT_EQ(data1, data2);
}

TEST(NlJson, BaseType_From)
{
    BaseType data1;
    InitData2(data1);

    BaseType data2;
    std::string s = BaseTypeJson2();
    MSRPC::FromJsonS(data2, s);

    EXPECT_EQ(data1, data2);
}

TEST(NlJson, BaseType_FromDiffMem)
{
    BaseType data1;
    InitData1(data1);

    BaseType data2;
    InitData1(data2);
    data2.c = 0;
    data2.s = 0;
    data2.i = 0.0;
    data2.d = 0.0;

    std::string s = BaseTypeJsonDiffMem1();
    MSRPC::FromJsonS(data2, s);

    EXPECT_EQ(data1, data2);
}

TEST(NlJson, BaseType_FromDiffTypeString)
{
    BaseType data1;
    InitData1(data1);

    BaseType data2;
    std::string s = BaseTypeJsonDiffType1();
    MSRPC::FromJsonS(data2, s);

    EXPECT_EQ(data1, data2);
}

TEST(NlJson, BaseStringType_FromDiffType)
{
    BaseStringType data1;
    InitData1(data1);

    BaseStringType data2;
    std::string s = BaseTypeJson1();
    auto doc = nlohmann::json::parse(s);
    MSRPC::FromJsonO(data2, doc);

    EXPECT_EQ(data1, data2);
}
TEST(NlJson, StringType_Self)
{
    StringType data1;
    InitData1(data1);

    std::string strJson = MSRPC::ToJsonS(data1);

    StringType data2;
    MSRPC::FromJsonS(data2, strJson);

    EXPECT_EQ(data1, data2);
}

TEST(NlJson, StringType_From)
{
    StringType data1;
    InitData1(data1);

    StringType data2;
    std::string s = StringTypeJson1();
    MSRPC::FromJsonS(data2, s);

    EXPECT_EQ(data1, data2);
}

TEST(NlJson, PointerType_Self)
{
    PointerType data1;
    InitData1(data1);

    std::string strJson = MSRPC::ToJsonS(data1);

    PointerType data2;
    MSRPC::FromJsonS(data2, strJson);

    EXPECT_EQ(data1, data2);
}

TEST(NlJson, PointerType_From)
{
    PointerType data1;
    InitData1(data1);

    PointerType data2;
    std::string s = PointerTypeJson1();
    MSRPC::FromJsonS(data2, s);

    EXPECT_EQ(data1, data2);
}

TEST(NlJson, ContType_Self)
{
    ContType data1;
    InitData1(data1);

    std::string strJson = MSRPC::ToJsonS(data1);

    ContType data2;
    MSRPC::FromJsonS(data2, strJson);

    EXPECT_EQ(data1, data2);
}

TEST(NlJson, ContType_From)
{
    ContType data1;
    InitData1(data1);

    ContType data2;
    std::string s = ContTypeJson1();
    MSRPC::FromJsonS(data2, s);

    EXPECT_EQ(data1, data2);
}

TEST(NlJson, FixContType_Self)
{
    FixContType data1;
    InitData1(data1);

    std::string strJson = MSRPC::ToJsonS(data1);

    FixContType data2;
    MSRPC::FromJsonS(data2, strJson);

    EXPECT_EQ(data1, data2);
}

TEST(NlJson, FixContType_From)
{
    FixContType data1;
    InitData1(data1);

    FixContType data2;
    std::string s = FixContTypeJson1();
    MSRPC::FromJsonS(data2, s);

    EXPECT_EQ(data1, data2);
}

TEST(NlJson, File)
{
    BaseType bt1;
    InitData1(bt1);
    MSRPC::ToJsonF(bt1, "basetype1.json");

    BaseType bt2;
    MSRPC::FromJsonF(bt2, "basetype1.json");
    EXPECT_EQ(bt1, bt2);
}

