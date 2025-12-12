#pragma once
#include <string>
#include <sstream>

namespace MSRPC
{
    template<typename T>
    inline bool is_valid_float(const T& x)
    {
        return !std::isnan(x) && !std::isinf(x);
    }

    template <class T>
    inline void ToString(std::string &str, const T &var)
    {
        std::ostringstream ss;
        ss << var;
        str = ss.str();
    }

    inline void ToString(std::string &str, const bool &var)
    {
        str = var ? "true" : "false";
    }

    template <class T>
    inline void ToValue(T &val, const std::string& str)
    {
        std::istringstream ss(str);
        ss >> val;
    }

    inline void ToValue(bool &val, const std::string& str)
    {
        val = str != "false" && str != "0";
        // val = memcmp(str, "false", size) != 0 && memcmp(str, "0", size) != 0;
    }
}