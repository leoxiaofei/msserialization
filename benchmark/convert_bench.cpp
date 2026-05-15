#include "msadapter/nljson.hpp"
#include "msadapter/rajson.hpp"
#include "msadapter/cppyaml.hpp"
#include "stltypedef.hpp"
#include <gtest/gtest.h>
#include <chrono>

TEST(ConvertBench, Performance_Comparison)
{
    std::vector<PointerType> vecData1(1000);
    for (auto& data : vecData1)
    {
        InitData1(data);
    }

    // NlJson serialization
    auto start = std::chrono::high_resolution_clock::now();
    std::string strJson1 = MSRPC::NlJson::ToJsonS(vecData1);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "NlJson Serialize: " << duration1.count() << "ms" << std::endl;

    // RaJson serialization
    start = std::chrono::high_resolution_clock::now();
    std::string strJson2 = MSRPC::RaJson::ToJsonS(vecData1);
    end = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "RaJson Serialize: " << duration2.count() << "ms" << std::endl;

    // CppYaml serialization
    start = std::chrono::high_resolution_clock::now();
    std::string strYaml = MSRPC::CppYaml::ToYamlS(vecData1);
    end = std::chrono::high_resolution_clock::now();
    auto duration3 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "CppYaml Serialize: " << duration3.count() << "ms" << std::endl;

    // Verify all produce equivalent results
    std::vector<PointerType> vecData2, vecData3, vecData4;
    MSRPC::NlJson::FromJsonS(vecData2, strJson1);
    MSRPC::RaJson::FromJsonS(vecData3, strJson2);
    MSRPC::CppYaml::FromYamlS(vecData4, strJson1); // JSON is valid YAML

    EXPECT_EQ(vecData1, vecData2);
    EXPECT_EQ(vecData1, vecData3);
    EXPECT_EQ(vecData1, vecData4);
}