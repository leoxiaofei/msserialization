
#include "msadapter/cppyaml.hpp"
#include "stltypedef.hpp"
#include <gtest/gtest.h>
#include <chrono>


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

TEST(CppYaml, FilePerformance)
{
    std::vector<PointerType> vecData1(10000);

    for (auto& data1 : vecData1)
    {
        InitData1(data1);
    }
    auto start = std::chrono::high_resolution_clock::now();
    MSRPC::ToYamlF(vecData1, "pointertypes1.json");
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "SaveFilePerformance: " << duration.count() << "ms" << std::endl;

    std::vector<PointerType> vecData2;
    start =  std::chrono::high_resolution_clock::now();
    MSRPC::FromYamlF(vecData2, "pointertypes1.json");
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "LoadFilePerformance: " << duration.count() << "ms" << std::endl;

    EXPECT_EQ(vecData1, vecData2);
}

TEST(CppYaml, Performance)
{
    std::vector<PointerType> vecData1(10000);

    for (auto& data1 : vecData1)
    {
        InitData1(data1);
    }
    auto start = std::chrono::high_resolution_clock::now();
    auto strYaml = MSRPC::ToYamlS(vecData1);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "SaveFilePerformance: " << duration.count() << "ms" << std::endl;

    std::vector<PointerType> vecData2;
    start =  std::chrono::high_resolution_clock::now();
    MSRPC::FromYamlS(vecData2, strYaml);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "LoadFilePerformance: " << duration.count() << "ms" << std::endl;

    EXPECT_EQ(vecData1, vecData2);
}

TEST(CppYaml, Performance2)
{
    std::vector<BaseType> vecData1(100000);

    for (auto& data1 : vecData1)
    {
        InitData1(data1);
    }
    auto start = std::chrono::high_resolution_clock::now();
    auto strYaml = MSRPC::ToYamlS(vecData1);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "SaveFilePerformance: " << duration.count() << "ms" << std::endl;

    std::vector<BaseType> vecData2;
    start =  std::chrono::high_resolution_clock::now();
    MSRPC::FromYamlS(vecData2, strYaml);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "LoadFilePerformance: " << duration.count() << "ms" << std::endl;

    EXPECT_EQ(vecData1, vecData2);
}

