#include "stltypedef.hpp"

#include "msadapter/nljsonserializer.hpp"
#include <gtest/gtest.h>
#include <chrono>


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

TEST(NlJson, FilePerformance)
{
    std::vector<PointerType> vecData1(10000);

    for (auto& data1 : vecData1)
    {
        InitData1(data1);
    }
    auto start = std::chrono::high_resolution_clock::now();
    MSRPC::ToJsonF(vecData1, "pointertypes1.json");
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "SaveFilePerformance: " << duration.count() << "ms" << std::endl;

    std::vector<PointerType> vecData2;
    start =  std::chrono::high_resolution_clock::now();
    MSRPC::FromJsonF(vecData2, "pointertypes1.json");
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "LoadFilePerformance: " << duration.count() << "ms" << std::endl;

    EXPECT_EQ(vecData1, vecData2);
}

TEST(NlJson, Performance)
{
    std::vector<PointerType> vecData1(10000);

    for (auto& data1 : vecData1)
    {
        InitData1(data1);
    }
    auto start = std::chrono::high_resolution_clock::now();
    auto strJson = MSRPC::ToJsonS(vecData1);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "SaveFilePerformance: " << duration.count() << "ms" << std::endl;

    std::vector<PointerType> vecData2;
    start =  std::chrono::high_resolution_clock::now();
    MSRPC::FromJsonS(vecData2, strJson);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "LoadFilePerformance: " << duration.count() << "ms" << std::endl;

    EXPECT_EQ(vecData1, vecData2);
}

TEST(NlJson, Performance2)
{
    std::vector<BaseType> vecData1(100000);

    for (auto& data1 : vecData1)
    {
        InitData1(data1);
    }
    auto start = std::chrono::high_resolution_clock::now();
    auto strJson = MSRPC::ToJsonS(vecData1);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "SaveFilePerformance: " << duration.count() << "ms" << std::endl;

    std::vector<BaseType> vecData2;
    start =  std::chrono::high_resolution_clock::now();
    MSRPC::FromJsonS(vecData2, strJson);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "LoadFilePerformance: " << duration.count() << "ms" << std::endl;

    EXPECT_EQ(vecData1, vecData2);
}

