#pragma once

#include <fstream>
#include <string>

namespace Serialisation
{
    class BinaryIn
    {
    public:
        BinaryIn(const std::string& filename);
        ~BinaryIn();

        template<typename T>
        auto Read()->T;

    private:
        std::fstream _file;
    };

    template <typename T>
    auto BinaryIn::Read() -> T
    {
        T val;
        _file.read(reinterpret_cast<char*>(&val), sizeof(T));

        return val;
    }

    template <>
    inline auto BinaryIn::Read<std::string>() -> std::string
    {
        // String length
        size_t len;
        _file.read(reinterpret_cast<char*>(&len), sizeof(size_t));

        std::string str;
        str.resize(len);
        _file.read(&str[0], len);

        return str;
    }
}
