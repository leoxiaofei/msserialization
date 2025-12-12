#pragma once

#include "msserialize/siexse.hpp"

#include <cstdint>

class BaseType
{
public:
    char c = 0;
    bool b = false;
    short s = 0;
    int i = 0;
    long long l = 0;
    float f = 0.0f;
    double d = 0.0f;

    bool operator ==(const BaseType &rhs) const
    {
        return c == rhs.c && b == rhs.b && s == rhs.s && i == rhs.i
            && l == rhs.l && f == rhs.f && d == rhs.d;
    }
};

SiExSe(BaseType, c, b, s, i, l, f, d)

inline void InitData1(BaseType& data)
{
    data.c = 1;
    data.b = true;
    data.s = 2;
    data.i = 3;
    data.l = 40000000000;
    data.f = 5.1f;
    data.d = 60000000.2;
}

inline const char* BaseTypeJson1()
{
    return R"({"c":1,"b":true,"s":2,"i":3,"l":40000000000,"f":5.1,"d":60000000.2})";
}

inline const char* BaseTypeJsonDiffMem1()
{
    return R"({"c":1,"x":true,"s":2,"i":3,"y":40000000000,"z":5.1,"d":60000000.2})";
}

inline const char* BaseTypeJsonDiffType1()
{
    return R"({"c":"1","b":"true","s":"2","i":"3","l":"40000000000","f":"5.1","d":"60000000.2"})";
}

inline const char* BaseTypeYaml1()
{
    return R"()";
}

inline void InitData2(BaseType& data)
{
    data.c = 2;
    data.b = false;
    data.s = 3;
    data.i = 4;
    data.l = 50000000000ll;
    data.f = 6.1f;
    data.d = 70000000.2;
}

inline const char* BaseTypeJson2()
{
    return R"({"c":2,"b":false,"s":3,"i":4,"l":50000000000,"f":6.1,"d":70000000.2})";
}

inline const char* BaseTypeYaml2()
{
    return R"(c: 2
b: false
s: 3
i: 4
l: 50000000000
f: 6.0999999046325684
d: 70000000.200000003)";
}


class TypeBase
{
public:
    virtual ~TypeBase() {}
    virtual int Type() = 0;

    int8_t n8 = 0;
    uint8_t u8 = 0;

    bool operator ==(const TypeBase &rhs) const
    {
        return n8 == rhs.n8 && u8 == rhs.u8;
    }

    virtual bool Equal(const TypeBase* rhs) const = 0;
};

SiExSe(TypeBase, n8, u8);

class Type1 : public TypeBase
{
public:
    int Type() override { return 1; }

    int16_t n16 = 0;
    uint16_t u16 = 0;

    bool operator ==(const Type1 &rhs) const
    {
        return static_cast<const TypeBase&>(*this) == (rhs)
            && n16 == rhs.n16 && u16 == rhs.u16;
    }

    virtual bool Equal(const TypeBase *rhs) const override
    {
        auto other = dynamic_cast<const Type1*>(rhs);
        return other && *this == *other;
    }
};

SiExSeInhe(Type1, TypeBase, n16, u16);

class Type2 : public TypeBase
{
public:
    int Type() override { return 2; }

    int32_t n32 = 0;
    uint32_t u32 = 0;

    bool operator ==(const Type2 &rhs) const
    {
        return static_cast<const TypeBase&>(*this) == (rhs)
            && n32 == rhs.n32 && u32 == rhs.u32;
    }

    virtual bool Equal(const TypeBase *rhs) const override
    {
        auto other = dynamic_cast<const Type2*>(rhs);
        return other && *this == *other;
    }
};

SiExSeInhe(Type2, TypeBase, n32, u32);

class Type3 : public TypeBase
{
public:
    int Type() override { return 3; }

    int64_t n64 = 0;
    uint64_t u64 = 0;
    unsigned long long ull = 0;

    bool operator ==(const Type3 &rhs) const
    {
        return static_cast<const TypeBase&>(*this) == (rhs)
            && n64 == rhs.n64 && u64 == rhs.u64 && ull == rhs.ull;
    }

    virtual bool Equal(const TypeBase *rhs) const override
    {
        auto other = dynamic_cast<const Type3*>(rhs);
        return other && *this == *other;
    }
};

SiExSeInhe(Type3, TypeBase, n64, u64, ull);

BeginBaExSe(TypeBase)
    static std::map<int32_t, TypeBaseConvT> map = {
        {1, TypeBaseConv<Ar, Type1>},
        {2, TypeBaseConv<Ar, Type2>},
        {3, TypeBaseConv<Ar, Type3>},
    };
    int32_t type = tValue ? tValue->Type() : 0;
    ar.io("type", type);
EndBaExSe(map, type)

inline void InitData1(Type1& data)
{ 
    data.n8 = -1;
    data.u8 = 1;
    data.n16 = -2;
    data.u16 = 2;
}
inline const char* Type1Json1()
{
    return R"({"type":1,"n8":-1,"u8":1,"n16":-2,"u16":2})";
}

inline const char* Type1Yaml1()
{
    return R"()";
}

inline void InitData2(Type1& data)
{ 
    data.n8 = 1;
    data.u8 = -1;
    data.n16 = 2;
    data.u16 = -2;
}
inline const char* Type1Json2()
{
    return R"({"type":1,"n8":1,"u8":-1,"n16":2,"u16":-2})";
}

inline const char* Type1Yaml2()
{
    return R"(type: 1
n8: 1
u8: -1
n16: 2
u16: -2)";
}

inline void InitData1(Type2& data)
{ 
    data.n8 = -3;
    data.u8 = 3;
    data.n32 = -4;
    data.u32 = 4;
}

inline void InitData2(Type2& data)
{ 
    data.n8 = 3;
    data.u8 = -3;
    data.n32 = 4;
    data.u32 = -4;
}

inline const char* Type2Json1()
{
    return R"({"type":2,"n8":-3,"u8":3,"n32":-4,"u32":4})";
}

inline const char* Type2Yaml1()
{
    return R"(type: 2
n8: -3
u8: 3
n32: -4
u32: 4)";
}

inline const char* Type2Json2()
{
    return R"({"type":2,"n8":3,"u8":-3,"n32":4,"u32":-4})";
}

inline const char* Type2Yaml2()
{
    return R"(type: 2
n8: 3
u8: -3
n32: 4
u32: -4)";

}

inline void InitData1(Type3& data)
{ 
    data.n8 = -5;
    data.u8 = 5;
    data.n64 = -6;
    data.u64 = 6;
    data.ull = 60000000000ull;
}

inline void InitData2(Type3& data)
{ 
    data.n8 = 5;
    data.u8 = -5;
    data.n64 = 6;
    data.u64 = 60000000000ull;
    data.ull = 70000000000ull;
}

inline const char* Type3Json1()
{
    return R"({"type":3,"n8":-5,"u8":5,"n64":-6,"u64":6,"ull":60000000000})";
}

inline const char* Type3Yaml1()
{
    return R"(type: 3
n8: -5
u8: 5
n64: -6
u64: 6
ull: 60000000000)";
}

inline const char* Type3Json2()
{
    return R"({"type":3,"n8":5,"u8":-5,"n64":6,"u64":60000000000,"ull":70000000000})";
}

inline const char* Type3Yaml2()
{
    return R"(type: 3
n8: 5
u8: -5
n64: 6
u64: 60000000000
ull: 70000000000)";
}