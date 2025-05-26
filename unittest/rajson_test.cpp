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