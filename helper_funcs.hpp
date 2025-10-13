// helper_funcs.hpp
#include <string>
#include <map>
#include <chrono>
#include <bitset>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#pragma once

using namespace std;
int randomFiveDigit()
{
    // Seed only once using a static flag
    static bool seeded = false;
    if (!seeded)
    {
        std::srand(std::time(nullptr));
        seeded = true;
    }

    // Always return a 5-digit number
    return 10000 + (std::rand() % 90000);
}

int binary_to_deciaml(string number)
{
    return stoi(number, nullptr, 2);
}

template <size_t size>
string deciaml_to_binary(int number)
{
    bitset<size> binary(number);
    string binary_number = binary.to_string();
    return binary_number;
}
string pad_to_32(const string &input)
{
    if (input.size() >= 32)
        return input.substr(0, 32); // keep leftmost 32 bits

    return string(32 - input.size(), '0') + input;
}

const unsigned char *string_to_bytes(const std::string &data)
{
    return reinterpret_cast<const unsigned char *>(data.data());
}
std::vector<unsigned char> binary_string_to_bytes(const std::string &bitstring)
{
    if (bitstring.size() % 8 != 0)
        throw std::invalid_argument("Binary string length must be a multiple of 8.");

    std::vector<unsigned char> bytes;
    for (size_t i = 0; i < bitstring.size(); i += 8)
    {
        unsigned char byte = 0;
        for (int b = 0; b < 8; ++b)
        {
            if (bitstring[i + b] == '1')
                byte |= (1 << (7 - b));
            else if (bitstring[i + b] != '0')
                throw std::invalid_argument("Binary string must contain only '0' or '1'.");
        }
        bytes.push_back(byte);
    }
    return bytes;
}
unsigned char *binary_string_to_uchar_ptr(const std::string &bitstring)
{
    auto bytes = binary_string_to_bytes(bitstring);
    unsigned char *buffer = (unsigned char *)malloc(bytes.size());
    std::memcpy(buffer, bytes.data(), bytes.size());
    return buffer; // remember to free() later!
}
unsigned char string_to_uchar(const std::string &s)
{
    if (s.empty())
        throw std::invalid_argument("Empty string cannot be converted to unsigned char");

    // Case 1: string is a single character
    if (s.size() == 1)
        return static_cast<unsigned char>(s[0]);

    // Case 2: string represents a number
    int value = std::stoi(s); // converts string to int
    if (value < 0 || value > 255)
        throw std::out_of_range("Value must be between 0 and 255");

    return static_cast<unsigned char>(value);
}
std::string uchar_to_exact_string(unsigned char c)
{
    // Cast unsigned char to char before constructing the string
    return std::string(1, static_cast<char>(c));
}
bool is_ptr_in_block(unsigned char *block_start, size_t block_size, unsigned char *ptr)
{
    return ptr >= block_start && ptr < (block_start + block_size);
}
string print_if_used(bool para)
{
    if (para)
    {
        return "free";
    }
    else if (!para)
    {
        return "not free";
    }
}