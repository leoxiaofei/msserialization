#pragma once

#include "basetypedef.hpp"
#include <string>
#include <memory>
#include <array>
#include <deque>
#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <tuple>

class StringType
{
public:
    char sz1[10] = {};
    char sz2[10] = {};
    // const char* sp = nullptr;
    char* sp = nullptr;
    std::string str;

    bool operator == (const StringType& rhs) const
    {
        return std::strcmp(sz1, rhs.sz1) == 0 &&
            std::strcmp(sz2, rhs.sz2) == 0 &&
            std::strcmp(sp ? sp : "", rhs.sp ? rhs.sp : "") == 0 &&
            str == rhs.str;
    }

    ~StringType()
    {
        delete[] sp;
    }
};

SiExSe(StringType, sz1, sz2, sp, str)

inline void InitData1(StringType& data)
{
    std::strcpy(data.sz1, "abcdefgh");
    std::strcpy(data.sz2, "123456789");
    data.sp = new char[10];
    std::strcpy(data.sp, "sp");
    data.str = "str";
}

inline const char* StringTypeJson1()
{
    return R"({"sz1":"abcdefgh","sz2":"123456789","sp":"sp","str":"str"})";
}

inline const char* StringTypeYaml1()
{
    return R"(sz1: "abcdefgh"
sz2: "123456789"
sp: sp
str: str
)";

}

class BaseStringType
{
public:
    char* c = nullptr;
    const char* b = nullptr;
    char s[64];
    char i[32];
    char l[5];
    std::string f;
    std::string d;

    bool operator ==(const BaseStringType &rhs) const
    {
        return
            std::strcmp(c ? c : "", rhs.c ? rhs.c : "") == 0 &&
            std::strcmp(b ? b : "", rhs.b ? rhs.b : "") == 0 &&
            std::strcmp(s, rhs.s) == 0 &&
            std::strcmp(i, rhs.i) == 0 &&
            std::strcmp(l, rhs.l) == 0 &&
            f == rhs.f &&
            d == rhs.d;
    }

    ~BaseStringType()
    {
        delete[] c;
    }
};

SiExSe(BaseStringType, c, b, s, i, l, f, d)

inline void InitData1(BaseStringType& data)
{
    data.c = new char[10];
    strncpy(data.c, "1", sizeof(10));
    data.b = "true";
    strncpy(data.s, "2", sizeof(data.s));
    strncpy(data.i, "3", sizeof(data.i));
    strncpy(data.l, "40000000000", sizeof(data.l));
    data.l[sizeof(data.l)-1] = '\0';
    data.f = std::to_string(5.1);
    data.d = std::to_string(60000000.2);
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

    bool operator == (const PointerType& rhs) const
    {
        return *pI == *(rhs.pI) &&
            *pT1 == *(rhs.pT1) &&
            *pT2 == *(rhs.pT2) &&
            *pT3 == *(rhs.pT3) &&
            pBT1->Equal(rhs.pBT1.get()) &&
            pBT2->Equal(rhs.pBT2.get()) &&
            pBT3->Equal(rhs.pBT3.get());
    }

    ~PointerType()
    {
        delete pT1;
    }

    PointerType() = default;

    PointerType(PointerType&& rhs) noexcept
        : pI(std::move(rhs.pI))
        , pT1(rhs.pT1)
        , pT2(std::move(rhs.pT2))
        , pT3(std::move(rhs.pT3))
        , pBT1(std::move(rhs.pBT1))
        , pBT2(std::move(rhs.pBT2))
        , pBT3(std::move(rhs.pBT3))
    {
        rhs.pT1 = nullptr;
    }

    PointerType& operator = (PointerType&& rhs) noexcept
    {
        if (this != &rhs)
        {
            pI = std::move(rhs.pI);
            pT1 = rhs.pT1;
            pT2 = std::move(rhs.pT2);
            pT3 = std::move(rhs.pT3);
            pBT1 = std::move(rhs.pBT1);
            pBT2 = std::move(rhs.pBT2);
            pBT3 = std::move(rhs.pBT3);

            rhs.pT1 = nullptr;
        }

        return *this;
    }
};

SiExSe(PointerType, pI, pT1, pT2, pT3, pBT1, pBT2, pBT3);

inline void InitData1(PointerType& data)
{
    data.pI = std::make_unique<int>(10);

    data.pT1 = new Type1();
    data.pT2 = std::make_shared<Type2>();
    data.pT3 = std::make_unique<Type3>();

    data.pBT1 = std::make_shared<Type1>();
    data.pBT2 = std::make_shared<Type2>();
    data.pBT3 = std::make_shared<Type3>();

    InitData1(*data.pT1);
    InitData1(*data.pT2);
    InitData1(*data.pT3);

    InitData1(*std::static_pointer_cast<Type1>(data.pBT1));
    InitData1(*std::static_pointer_cast<Type2>(data.pBT2));
    InitData1(*std::static_pointer_cast<Type3>(data.pBT3));
}

inline const char* PointerTypeJson1()
{
    return R"({"pI":10,"pT1":{"n8":-1,"u8":1,"n16":-2,"u16":2},"pT2":{"n8":-3,"u8":3,"n32":-4,"u32":4},"pT3":{"n8":-5,"u8":5,"n64":-6,"u64":6,"ull":60000000000},"pBT1":{"type":1,"n8":-1,"u8":1,"n16":-2,"u16":2},"pBT2":{"type":2,"n8":-3,"u8":3,"n32":-4,"u32":4},"pBT3":{"type":3,"n8":-5,"u8":5,"n64":-6,"u64":6,"ull":60000000000}})";
}

inline const char* PointerTypeYaml1()
{
    return R"(pI: 10
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
  ull: 60000000000
pBT1:
  type: 1
  n8: -1
  u8: 1
  n16: -2
  u16: 2
pBT2:
  type: 2
  n8: -3
  u8: 3
  n32: -4
  u32: 4
pBT3:
  type: 3
  n8: -5
  u8: 5
  n64: -6
  u64: 6
  ull: 60000000000)";
}

class FixContType
{
public:
    std::array<int32_t, 10> arrN32;
    std::pair<std::string, int32_t> pStrN32;
    std::tuple<uint16_t, std::string, float> tU16StrF;

    bool operator == (const FixContType& rhs) const
    {
        return arrN32 == rhs.arrN32 &&
            pStrN32 == rhs.pStrN32 &&
            tU16StrF == rhs.tU16StrF;
    }
};

SiExSe(FixContType, arrN32, pStrN32, tU16StrF);

inline void InitData1(FixContType& data)
{
    for (int i = 0; i < 10; ++i)
    {
        data.arrN32[i] = i;
    }

    data.pStrN32 = {"pStrN32", 32};

    data.tU16StrF = {16, "tU16StrF", 32.0f};
}

inline const char* FixContTypeJson1()
{
    return R"({"arrN32":[0,1,2,3,4,5,6,7,8,9],"pStrN32":["pStrN32",32],"tU16StrF":[16,"tU16StrF",32.0]})";
}

inline const char* FixContTypeYaml1()
{
    return R"(arrN32:
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

    bool operator == (const ContType& rhs) const
    {
        return vecN32 == rhs.vecN32 &&
            lsU16 == rhs.lsU16 &&
            deqT1 == rhs.deqT1 &&
            stF == rhs.stF &&
            ustU32 == rhs.ustU32 &&
            umstStrN8 == rhs.umstStrN8 &&
            mapStrN8 == rhs.mapStrN8 &&
            mapDN64 == rhs.mapDN64 &&
            hsStrT2 == rhs.hsStrT2 &&
            hsDStr == rhs.hsDStr &&
            ummapStrD == rhs.ummapStrD &&
            ummapDT3 == rhs.ummapDT3;
    }
};

SiExSe(ContType, vecN32, lsU16, deqT1, stF, ustU32, umstStrN8, mapStrN8, mapDN64, hsStrT2, hsDStr, ummapStrD, ummapDT3);

inline void InitData1(ContType& data)
{
    data.vecN32 = {1,2,3,4};
    data.lsU16 = {5,6,7};

    for (int i = 0; i < 3; i++)
    {
        Type1 t;
        t.n8 = -i;
        t.n16 = -i * 16;
        t.u8 = i;
        t.u16 = i * 16;
        data.deqT1.emplace_back(std::move(t));
    }
    
    data.stF = {8.1f, 9.2f, 10.3f};
    data.ustU32 = {11,12,13,14};
    data.umstStrN8 = {"15", "16", "16", "17"};

    data.mapStrN8 = {{"aaa", 1}, {"bbb", 2}};
    data.mapDN64 = {{12,22}, {13,33}};

    for (size_t i = 0; i < 5; i++)
    {
        auto& t = data.hsStrT2[std::to_string(i)];
        t.u8 = i;
        t.u32 = i * 32;
        t.n8 = i * -1;
        t.n32 = i * 16;
    }
    
    data.hsDStr = {{21, "21"}, {22, "22"}};

    data.ummapStrD = {{"aaa", 1.1}, {"aaa", 4.4}, {"bbb", 2.2}, {"ccc", 3.3}};

    data.ummapDT3 = {{1.1, {}}, {3.3, {}}, {2.2, {}}, {3.3, {}}};

    for (auto& pair : data.ummapDT3)
    {
        InitData2(pair.second);
    }
}

inline const char* ContTypeJson1()
{
    return R"({"vecN32":[1,2,3,4],"lsU16":[5,6,7],"deqT1":[{"n8":0,"u8":0,"n16":0,"u16":0},{"n8":-1,"u8":1,"n16":-16,"u16":16},{"n8":-2,"u8":2,"n16":-32,"u16":32}],"stF":[8.100000381469727,9.199999809265137,10.300000190734863],"ustU32":[11,12,13,14],"umstStrN8":["15","16","16","17"],"mapStrN8":{"aaa":1,"bbb":2},"mapDN64":[[12.0,22],[13.0,33]],"hsStrT2":{"0":{"n8":0,"u8":0,"n32":0,"u32":0},"1":{"n8":-1,"u8":1,"n32":16,"u32":32},"2":{"n8":-2,"u8":2,"n32":32,"u32":64},"3":{"n8":-3,"u8":3,"n32":48,"u32":96},"4":{"n8":-4,"u8":4,"n32":64,"u32":128}},"hsDStr":[[21.0,"21"],[22.0,"22"]],"ummapStrD":[["aaa",1.1],["aaa",4.4],["bbb",2.2],["ccc",3.3]],"ummapDT3":[[1.1,{"n8":5,"u8":251,"n64":6,"u64":60000000000,"ull":70000000000}],[3.3,{"n8":5,"u8":251,"n64":6,"u64":60000000000,"ull":70000000000}],[3.3,{"n8":5,"u8":251,"n64":6,"u64":60000000000,"ull":70000000000}],[2.2,{"n8":5,"u8":251,"n64":6,"u64":60000000000,"ull":70000000000}]]})";
}

inline const char* ContTypeYaml1()
{
    return R"(vecN32:
  - 1
  - 2
  - 3
  - 4
lsU16:
  - 5
  - 6
  - 7
deqT1:
  - n8: 0
    u8: 0
    n16: 0
    u16: 0
  - n8: -1
    u8: 1
    n16: -16
    u16: 16
  - n8: -2
    u8: 2
    n16: -32
    u16: 32
stF:
  - 8.1000003814697266
  - 9.1999998092651367
  - 10.300000190734863
ustU32:
  - 11
  - 12
  - 13
  - 14
umstStrN8:
  - 15
  - 16
  - 16
  - 17
mapStrN8:
  aaa: 1
  bbb: 2
mapDN64:
  -
    - 12
    - 22
  -
    - 13
    - 33
hsStrT2:
  0:
    n8: 0
    u8: 0
    n32: 0
    u32: 0
  1:
    n8: -1
    u8: 1
    n32: 16
    u32: 32
  2:
    n8: -2
    u8: 2
    n32: 32
    u32: 64
  3:
    n8: -3
    u8: 3
    n32: 48
    u32: 96
  4:
    n8: -4
    u8: 4
    n32: 64
    u32: 128
hsDStr:
  -
    - 21
    - 21
  -
    - 22
    - 22
ummapStrD:
  -
    - aaa
    - 1.1000000000000001
  -
    - aaa
    - 4.4000000000000004
  -
    - bbb
    - 2.2000000000000002
  -
    - ccc
    - 3.2999999999999998
ummapDT3:
  -
    - 1.1000000000000001
    - n8: 5
      u8: 251
      n64: 6
      u64: 60000000000
      ull: 70000000000
  -
    - 3.2999999999999998
    - n8: 5
      u8: 251
      n64: 6
      u64: 60000000000
      ull: 70000000000
  -
    - 3.2999999999999998
    - n8: 5
      u8: 251
      n64: 6
      u64: 60000000000
      ull: 70000000000
  -
    - 2.2000000000000002
    - n8: 5
      u8: 251
      n64: 6
      u64: 60000000000
      ull: 70000000000)";
}