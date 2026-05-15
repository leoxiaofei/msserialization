
#include "msadapter/cppyaml.hpp"
#include "stltypedef.hpp"
#include <gtest/gtest.h>


TEST(CppYaml, BaseType_Self)
{
    BaseType data1;
    InitData1(data1);

    std::string strYaml = MSRPC::ToYamlS(data1);

    BaseType data2;
    MSRPC::FromYamlS(data2, strYaml);

    EXPECT_EQ(data1, data2);
}

TEST(CppYaml, BaseType_From)
{
    BaseType data1;
    InitData2(data1);

    std::string strYaml = MSRPC::ToYamlS(data1);

    BaseType data2;
    std::string s = BaseTypeYaml2();
    MSRPC::FromYamlS(data2, s);

    EXPECT_EQ(data1, data2);
}

TEST(CppYaml, BaseType_FromDiffMem)
{
    BaseType data1;
    InitData1(data1);

    BaseType data2;
    InitData1(data2);
    data2.c = 0;
    data2.s = 0;
    data2.i = 0.0;
    data2.d = 0.0;

    std::string s = BaseTypeYamlDiffMem1();
    MSRPC::FromYamlS(data2, s);

    EXPECT_EQ(data1, data2);
}

TEST(CppYaml, BaseType_FromDiffTypeString)
{
    BaseType data1;
    InitData1(data1);

    BaseType data2;
    std::string s = BaseTypeYamlDiffType1();
    MSRPC::FromYamlS(data2, s);

    EXPECT_EQ(data1, data2);
}

TEST(CppYaml, BaseStringType_FromDiffType)
{
    BaseStringType data1;
    InitData1(data1);

    BaseStringType data2;
    std::string s = BaseTypeYaml1();
    auto root = YAML::Load(s);
    MSRPC::FromYamlO(data2, root);

    EXPECT_EQ(data1, data2);
}

TEST(CppYaml, StringType_Self)
{
    StringType data1;
    InitData1(data1);

    std::string strYaml = MSRPC::ToYamlS(data1);

    StringType data2;
    MSRPC::FromYamlS(data2, strYaml);

    EXPECT_EQ(data1, data2);
}

TEST(CppYaml, StringType_From)
{
    StringType data1;
    InitData1(data1);

    StringType data2;
    std::string s = StringTypeYaml1();
    MSRPC::FromYamlS(data2, s);

    EXPECT_EQ(data1, data2);
}

TEST(CppYaml, PointerType_Self)
{
    PointerType data1;
    InitData1(data1);

    std::string strYaml = MSRPC::ToYamlS(data1);

    PointerType data2;
    MSRPC::FromYamlS(data2, strYaml);

    EXPECT_EQ(data1, data2);
}

TEST(CppYaml, PointerType_From)
{
    PointerType data1;
    InitData1(data1);

    PointerType data2;
    std::string s = PointerTypeYaml1();
    MSRPC::FromYamlS(data2, s);

    EXPECT_EQ(data1, data2);
}

TEST(CppYaml, ContType_Self)
{
    ContType data1;
    InitData1(data1);

    std::string strYaml = MSRPC::ToYamlS(data1);

    ContType data2;
    MSRPC::FromYamlS(data2, strYaml);

    EXPECT_EQ(data1, data2);
}

TEST(CppYaml, ContType_From)
{
    ContType data1;
    InitData1(data1);

    ContType data2;
    std::string s = ContTypeYaml1();
    MSRPC::FromYamlS(data2, s);

    EXPECT_EQ(data1, data2);
}

TEST(CppYaml, FixContType_Self)
{
    FixContType data1;
    InitData1(data1);

    std::string strYaml = MSRPC::ToYamlS(data1);

    FixContType data2;
    MSRPC::FromYamlS(data2, strYaml);

    EXPECT_EQ(data1, data2);
}

TEST(CppYaml, FixContType_From)
{
    FixContType data1;
    InitData1(data1);

    FixContType data2;
    std::string s = FixContTypeYaml1();
    MSRPC::FromYamlS(data2, s);

    EXPECT_EQ(data1, data2);
}

TEST(CppYaml, File)
{
    BaseType bt1;
    InitData1(bt1);
    MSRPC::ToYamlF(bt1, "basetype1.json");

    BaseType bt2;
    MSRPC::FromYamlF(bt2, "basetype1.json");
    EXPECT_EQ(bt1, bt2);
}

