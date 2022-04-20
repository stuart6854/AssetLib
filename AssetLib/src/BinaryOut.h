﻿#pragma once

#include <fstream>
#include <string>

namespace Serialisation
{
    class BinaryOut
    {
    public:
        BinaryOut(const std::string& filename);
        ~BinaryOut();

        void Write(const bool& val);
        void Write(const float& val);
        void Write(const int& val);
        void Write(const char& val);
        void Write(const char* val);
        void Write(const std::string& val);

    private:
        template<typename T>
        void Write_Internal(const T& val);

    private:
        std::fstream _file;
    };

    template <typename T>
    void BinaryOut::Write_Internal(const T& val)
    {
        _file.write(reinterpret_cast<const char*>(&val), sizeof(T));
    }
}
