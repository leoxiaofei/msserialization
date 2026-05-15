#include "stltypedef.hpp"
#include "msadapter/qjson.hpp"
#include <msserialize/msarchive_stl.hpp>
#include <gtest/gtest.h>

TEST(QJson, BaseType_Self)
{
    BaseType data1;
    InitData1(data1);

    auto strJson = MSRPC::ToJsonS(data1);

    BaseType data2;
    MSRPC::FromJsonS(data2, strJson);

    EXPECT_EQ(data1, data2);
}

TEST(QJson, BaseType_From)
{
    BaseType data1;
    InitData2(data1);

    BaseType data2;
    QByteArray s = BaseTypeJson2();
    MSRPC::FromJsonS(data2, s);

    EXPECT_EQ(data1, data2);
}

TEST(QJson, StringType_Self)
{
    StringType data1;
    InitData1(data1);

    QByteArray strJson = MSRPC::ToJsonS(data1);

    StringType data2;
    MSRPC::FromJsonS(data2, strJson);

    EXPECT_EQ(data1, data2);
}

TEST(QJson, StringType_From)
{
    StringType data1;
    InitData1(data1);

    StringType data2;
    QByteArray s = StringTypeJson1();
    MSRPC::FromJsonS(data2, s);

    EXPECT_EQ(data1, data2);
}

TEST(QJson, PointerType_Self)
{
    PointerType data1;
    InitData1(data1);

    QByteArray strJson = MSRPC::ToJsonS(data1);

    PointerType data2;
    MSRPC::FromJsonS(data2, strJson);

    EXPECT_EQ(data1, data2);
}

TEST(QJson, PointerType_From)
{
    PointerType data1;
    InitData1(data1);

    PointerType data2;
    QByteArray s = PointerTypeJson1();
    MSRPC::FromJsonS(data2, s);

    EXPECT_EQ(data1, data2);
}

TEST(QJson, ContType_Self)
{
    ContType data1;
    InitData1(data1);

    QByteArray strJson = MSRPC::ToJsonS(data1);

    ContType data2;
    MSRPC::FromJsonS(data2, strJson);

    EXPECT_EQ(data1, data2);
}

TEST(QJson, ContType_From)
{
    ContType data1;
    InitData1(data1);

    ContType data2;
    QByteArray s = ContTypeJson1();
    MSRPC::FromJsonS(data2, s);

    EXPECT_EQ(data1, data2);
}

TEST(QJson, FixContType_Self)
{
    FixContType data1;
    InitData1(data1);

    QByteArray strJson = MSRPC::ToJsonS(data1);

    FixContType data2;
    MSRPC::FromJsonS(data2, strJson);

    EXPECT_EQ(data1, data2);
}

TEST(QJson, FixContType_From)
{
    FixContType data1;
    InitData1(data1);

    FixContType data2;
    QByteArray s = FixContTypeJson1();
    MSRPC::FromJsonS(data2, s);

    EXPECT_EQ(data1, data2);
}

TEST(QJson, File)
{
    BaseType bt1;
    InitData1(bt1);
    MSRPC::ToJsonF(bt1, "basetype1.json");

    BaseType bt2;
    MSRPC::FromJsonF(bt2, "basetype1.json");
    EXPECT_EQ(bt1, bt2);
}
