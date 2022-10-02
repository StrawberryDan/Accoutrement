#pragma once



#include <string>
#include <vector>
#include <cstdint>



namespace Base64
{
    std::string Encode(const std::vector<uint8_t>& bytes);

    std::vector<uint8_t> Decode(std::string encoded);
}