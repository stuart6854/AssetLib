#include "BinaryOut.h"

#include <ios>
#include <iostream>

namespace Serialisation
{

    BinaryOut::BinaryOut(const std::string& filename)
    {
        _file = std::fstream(filename, std::ios::out | std::ios::binary);
        if (!_file)
        {
            std::cerr << "Failed to open file: " << filename << std::endl;
        }
    }

    BinaryOut::~BinaryOut()
    {
        _file.close();
    }

    void BinaryOut::Write(const bool& val)
    {
        Write_Internal(val);
        //_file.write(reinterpret_cast<const char*>(&val), sizeof(val));
    }

    void BinaryOut::Write(const float& val)
    {
        Write_Internal(val);
        //_file.write(reinterpret_cast<const char*>(&val), sizeof(val));
    }

    void BinaryOut::Write(const int& val)
    {
        Write_Internal(val);
        //_file.write(reinterpret_cast<const char*>(&val), sizeof(val));
    }

    void BinaryOut::Write(const char& val)
    {
        Write_Internal(val);
        //_file.write(reinterpret_cast<const char*>(&val), sizeof(val));
    }

    void BinaryOut::Write(const char* val)
    {
        Write_Internal(val);
    }

    void BinaryOut::Write(const std::string& val)
    {
        const size_t len = val.size();
        _file.write(reinterpret_cast<const char*>(&len), sizeof(size_t));
        _file.write(val.c_str(), len);
    }

    void BinaryOut::Write(const uint8_t& val)
    {
        Write_Internal(val);
    }

    void BinaryOut::Write(const uint16_t& val)
    {
        Write_Internal(val);
    }

    void BinaryOut::Write(const uint32_t& val)
    {
        Write_Internal(val);
    }

    void BinaryOut::Write(const uint64_t& val)
    {
        Write_Internal(val);
    }

}  // namespace Serialisation
