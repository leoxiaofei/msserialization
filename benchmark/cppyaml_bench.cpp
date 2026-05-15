#include "msadapter/cppyaml.hpp"
#include "stltypedef.hpp"
#include <gtest/gtest.h>
#include <chrono>

TEST(CppYamlBench, FilePerformance)
{
    std::vector<PointerType> vecData1(10000);

    for (auto& data1 : vecData1)
    {
        InitData1(data1);
    }
    auto start = std::chrono::high_resolution_clock::now();
    MSRPC::ToYamlF(vecData1, "pointertypes1.yaml");
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "SaveFilePerformance: " << duration.count() << "ms" << std::endl;

    std::vector<PointerType> vecData2;
    start =  std::chrono::high_resolution_clock::now();
    MSRPC::FromYamlF(vecData2, "pointertypes1.yaml");
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "LoadFilePerformance: " << duration.count() << "ms" << std::endl;

    EXPECT_EQ(vecData1, vecData2);
}

TEST(CppYamlBench, Performance)
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
    std::cout << "SerializePerformance: " << duration.count() << "ms" << std::endl;

    std::vector<PointerType> vecData2;
    start =  std::chrono::high_resolution_clock::now();
    MSRPC::FromYamlS(vecData2, strYaml);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "DeserializePerformance: " << duration.count() << "ms" << std::endl;

    EXPECT_EQ(vecData1, vecData2);
}

TEST(CppYamlBench, Performance2)
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
    std::cout << "SerializePerformance: " << duration.count() << "ms" << std::endl;

    std::vector<BaseType> vecData2;
    start =  std::chrono::high_resolution_clock::now();
    MSRPC::FromYamlS(vecData2, strYaml);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "DeserializePerformance: " << duration.count() << "ms" << std::endl;

    EXPECT_EQ(vecData1, vecData2);
}