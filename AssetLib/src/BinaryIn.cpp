#include "BinaryIn.h"

#include <iostream>

namespace Serialisation
{
    BinaryIn::BinaryIn(const std::string& filename)
    {
        _file = std::fstream(filename, std::ios::in | std::ios::binary);
        if (!_file)
        {
            std::cerr << "Failed to open file: " << filename << std::endl;
        }
    }

    BinaryIn::~BinaryIn()
    {
        _file.close();
    }
}
