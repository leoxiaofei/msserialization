#include "msadapter/nljson.hpp"
#include "msadapter/rajson.hpp"
#include "msadapter/cppyaml.hpp"
#include "stltypedef.hpp"
#include <gtest/gtest.h>

// ============================================================================
// Test 1: Parse predefined JSON/YAML strings with equivalent values
// ============================================================================

TEST(Convert, Parse_Equivalent_BaseType)
{
    // JSON string
    std::string strJson = R"({"c":1,"b":true,"s":2,"i":3,"l":40000000000,"f":5.1,"d":60000000.2})";

    // Equivalent YAML string
    std::string strYaml = R"(c: 1
b: true
s: 2
i: 3
l: 40000000000
f: 5.1
d: 60000000.2)";

    // Parse with different engines
    BaseType dataJson1, dataJson2;
    BaseType dataYaml;

    MSRPC::NlJson::FromJsonS(dataJson1, strJson);
    MSRPC::RaJson::FromJsonS(dataJson2, strJson);
    MSRPC::CppYaml::FromYamlS(dataYaml, strYaml);

    EXPECT_EQ(dataJson1, dataYaml);
    EXPECT_EQ(dataJson2, dataYaml);
    EXPECT_EQ(dataJson1, dataJson2);
}

TEST(Convert, Parse_Equivalent_PointerType)
{
    std::string strJson = R"({"pI":10,"pT1":{"n8":-1,"u8":1,"n16":-2,"u16":2},"pT2":{"n8":-3,"u8":3,"n32":-4,"u32":4},"pT3":{"n8":-5,"u8":5,"n64":-6,"u64":6,"ull":60000000000}})";

    std::string strYaml = R"(pI: 10
pT1:
  n8: -1
  u8: 1
  n16: -2
  u16: 2
pT2:
  n8: -3
  u8: 3
  n32: -4
  u32: 4
pT3:
  n8: -5
  u8: 5
  n64: -6
  u64: 6
  ull: 60000000000)";

    PointerType dataJson1, dataJson2;
    PointerType dataYaml;

    MSRPC::NlJson::FromJsonS(dataJson1, strJson);
    MSRPC::RaJson::FromJsonS(dataJson2, strJson);
    MSRPC::CppYaml::FromYamlS(dataYaml, strYaml);

    EXPECT_EQ(dataJson1, dataYaml);
    EXPECT_EQ(dataJson2, dataYaml);
    EXPECT_EQ(dataJson1, dataJson2);
}

TEST(Convert, Parse_Equivalent_ContType)
{
    std::string strJson = R"({"vecN32":[1,2,3,4],"lsU16":[5,6,7],"stF":[8.1,9.2,10.3],"mapStrN8":{"aaa":1,"bbb":2}})";

    std::string strYaml = R"(vecN32:
  - 1
  - 2
  - 3
  - 4
lsU16:
  - 5
  - 6
  - 7
stF:
  - 8.1
  - 9.2
  - 10.3
mapStrN8:
  aaa: 1
  bbb: 2)";

    ContType dataJson1, dataJson2;
    ContType dataYaml;

    MSRPC::NlJson::FromJsonS(dataJson1, strJson);
    MSRPC::RaJson::FromJsonS(dataJson2, strJson);
    MSRPC::CppYaml::FromYamlS(dataYaml, strYaml);

    EXPECT_EQ(dataJson1.vecN32, dataYaml.vecN32);
    EXPECT_EQ(dataJson1.lsU16, dataYaml.lsU16);
    EXPECT_EQ(dataJson1.stF, dataYaml.stF);
    EXPECT_EQ(dataJson1.mapStrN8, dataYaml.mapStrN8);

    EXPECT_EQ(dataJson2.vecN32, dataYaml.vecN32);
    EXPECT_EQ(dataJson2.lsU16, dataYaml.lsU16);
    EXPECT_EQ(dataJson2.stF, dataYaml.stF);
    EXPECT_EQ(dataJson2.mapStrN8, dataYaml.mapStrN8);
}

TEST(Convert, Parse_Equivalent_FixContType)
{
    std::string strJson = R"({"arrN32":[0,1,2,3,4,5,6,7,8,9],"pStrN32":["pStrN32",32],"tU16StrF":[16,"tU16StrF",32.0]})";

    std::string strYaml = R"(arrN32:
  - 0
  - 1
  - 2
  - 3
  - 4
  - 5
  - 6
  - 7
  - 8
  - 9
pStrN32:
  - pStrN32
  - 32
tU16StrF:
  - 16
  - tU16StrF
  - 32)";

    FixContType dataJson1, dataJson2;
    FixContType dataYaml;

    MSRPC::NlJson::FromJsonS(dataJson1, strJson);
    MSRPC::RaJson::FromJsonS(dataJson2, strJson);
    MSRPC::CppYaml::FromYamlS(dataYaml, strYaml);

    EXPECT_EQ(dataJson1, dataYaml);
    EXPECT_EQ(dataJson2, dataYaml);
    EXPECT_EQ(dataJson1, dataJson2);
}

// ============================================================================
// Test 2: Serialize same data with different engines, then compare deserialized results
// ============================================================================

TEST(Convert, Serialize_Then_Compare_BaseType)
{
    BaseType dataA, dataB;
    InitData1(dataA);
    InitData1(dataB);

    // Serialize: A -> JSON, B -> YAML
    std::string strJson = MSRPC::NlJson::ToJsonS(dataA);
    std::string strYaml = MSRPC::CppYaml::ToYamlS(dataB);

    // Deserialize: C from JSON, D from YAML
    BaseType dataC, dataD;
    MSRPC::RaJson::FromJsonS(dataC, strJson);
    MSRPC::CppYaml::FromYamlS(dataD, strYaml);

    // Compare C and D (should be equal since A == B)
    EXPECT_EQ(dataC, dataD);
    EXPECT_EQ(dataA, dataC);
    EXPECT_EQ(dataB, dataD);
}

TEST(Convert, Serialize_Then_Compare_PointerType)
{
    PointerType dataA, dataB;
    InitData1(dataA);
    InitData1(dataB);

    std::string strJson = MSRPC::RaJson::ToJsonS(dataA);
    std::string strYaml = MSRPC::CppYaml::ToYamlS(dataB);

    PointerType dataC, dataD;
    MSRPC::NlJson::FromJsonS(dataC, strJson);
    MSRPC::CppYaml::FromYamlS(dataD, strYaml);

    EXPECT_EQ(dataC, dataD);
    EXPECT_EQ(dataA, dataC);
    EXPECT_EQ(dataB, dataD);
}

TEST(Convert, Serialize_Then_Compare_ContType)
{
    ContType dataA, dataB;
    InitData1(dataA);
    InitData1(dataB);

    std::string strJson = MSRPC::NlJson::ToJsonS(dataA);
    std::string strYaml = MSRPC::CppYaml::ToYamlS(dataB);

    ContType dataC, dataD;
    MSRPC::RaJson::FromJsonS(dataC, strJson);
    MSRPC::CppYaml::FromYamlS(dataD, strYaml);

    EXPECT_EQ(dataC, dataD);
    EXPECT_EQ(dataA, dataC);
    EXPECT_EQ(dataB, dataD);
}

TEST(Convert, Serialize_Then_Compare_FixContType)
{
    FixContType dataA, dataB;
    InitData1(dataA);
    InitData1(dataB);

    std::string strJson = MSRPC::RaJson::ToJsonS(dataA);
    std::string strYaml = MSRPC::CppYaml::ToYamlS(dataB);

    FixContType dataC, dataD;
    MSRPC::NlJson::FromJsonS(dataC, strJson);
    MSRPC::CppYaml::FromYamlS(dataD, strYaml);

    EXPECT_EQ(dataC, dataD);
    EXPECT_EQ(dataA, dataC);
    EXPECT_EQ(dataB, dataD);
}

// ============================================================================
// Test 3: Cross-engine round-trip (serialize with one, deserialize with another)
// ============================================================================

TEST(Convert, RoundTrip_NlJson_To_RaJson)
{
    BaseType data1;
    InitData1(data1);

    std::string strJson = MSRPC::NlJson::ToJsonS(data1);
    BaseType data2;
    MSRPC::RaJson::FromJsonS(data2, strJson);

    std::string strJson2 = MSRPC::RaJson::ToJsonS(data2);
    BaseType data3;
    MSRPC::NlJson::FromJsonS(data3, strJson2);

    EXPECT_EQ(data1, data2);
    EXPECT_EQ(data2, data3);
}

TEST(Convert, RoundTrip_RaJson_To_NlJson)
{
    BaseType data1;
    InitData1(data1);

    std::string strJson = MSRPC::RaJson::ToJsonS(data1);
    BaseType data2;
    MSRPC::NlJson::FromJsonS(data2, strJson);

    std::string strJson2 = MSRPC::NlJson::ToJsonS(data2);
    BaseType data3;
    MSRPC::RaJson::FromJsonS(data3, strJson2);

    EXPECT_EQ(data1, data2);
    EXPECT_EQ(data2, data3);
}

TEST(Convert, RoundTrip_NlJson_To_CppYaml_ViaJson)
{
    BaseType data1;
    InitData1(data1);

    // Serialize as JSON (JSON is valid YAML)
    std::string strJson = MSRPC::NlJson::ToJsonS(data1);
    BaseType data2;
    MSRPC::CppYaml::FromYamlS(data2, strJson);

    std::string strYaml = MSRPC::CppYaml::ToYamlS(data2);
    BaseType data3;
    // Note: YAML string may not be valid JSON, so we serialize back to JSON
    std::string strJson2 = MSRPC::NlJson::ToJsonS(data2);
    MSRPC::NlJson::FromJsonS(data3, strJson2);

    EXPECT_EQ(data1, data2);
    EXPECT_EQ(data2, data3);
}

TEST(Convert, RoundTrip_RaJson_To_CppYaml_ViaJson)
{
    BaseType data1;
    InitData1(data1);

    std::string strJson = MSRPC::RaJson::ToJsonS(data1);
    BaseType data2;
    MSRPC::CppYaml::FromYamlS(data2, strJson);

    std::string strJson2 = MSRPC::RaJson::ToJsonS(data2);
    BaseType data3;
    MSRPC::RaJson::FromJsonS(data3, strJson2);

    EXPECT_EQ(data1, data2);
    EXPECT_EQ(data2, data3);
}

// ============================================================================
// Test 4: Multi-engine consistency check
// ============================================================================

TEST(Convert, MultiEngine_Consistency)
{
    BaseType data1;
    InitData1(data1);

    // Serialize with all JSON engines
    std::string strJson1 = MSRPC::NlJson::ToJsonS(data1);
    std::string strJson2 = MSRPC::RaJson::ToJsonS(data1);

    // Deserialize with different engines
    BaseType data2, data3, data4, data5;
    MSRPC::NlJson::FromJsonS(data2, strJson1);
    MSRPC::RaJson::FromJsonS(data3, strJson1);
    MSRPC::NlJson::FromJsonS(data4, strJson2);
    MSRPC::RaJson::FromJsonS(data5, strJson2);

    // All should be equal
    EXPECT_EQ(data1, data2);
    EXPECT_EQ(data1, data3);
    EXPECT_EQ(data1, data4);
    EXPECT_EQ(data1, data5);
    EXPECT_EQ(data2, data3);
    EXPECT_EQ(data3, data4);
    EXPECT_EQ(data4, data5);
}