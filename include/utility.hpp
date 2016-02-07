/*  utility %{Cpp:License:ClassName} - Yann BOUCHER (yann) 26/01/2016
**
**
**            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
**                    Version 2, December 2004
**
** Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
**
** Everyone is permitted to copy and distribute verbatim or modified
** copies of this license document, and changing it is allowed as long
** as the name is changed.
**
**            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
**   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
**
**  0. You just DO WHAT THE FUCK YOU WANT TO.
*/
#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <cctype>

#include <algorithm>
#include <chrono>
#include <stdexcept>
#include <string>
#include <thread>

#include "common.hpp"

namespace i8080
{

template <typename Function>
inline void executeForDuration(std::chrono::duration<double> time, Function&& func)
{
    auto time1 = std::chrono::high_resolution_clock::now();
    func();
    auto time2 = std::chrono::high_resolution_clock::now();
    auto delta = time2 - time1;

    if (delta < time)
    {
        std::this_thread::sleep_for(time - delta);
    }
}

template <typename T>
inline T wrap(T kX, T kLowerBound, T kUpperBound)
{
    T range_size = kUpperBound - kLowerBound + 1;

    if (kX < kLowerBound)
        kX += range_size * ((kLowerBound - kX) / range_size + 1);

    return kLowerBound + (kX - kLowerBound) % range_size;
}


inline bool getBit(byte input, size_t pos)
{
    return (input >> pos) & 1;
}


inline void unsetBit(byte& input, size_t pos)
{
    input &= ~(1 << pos);
}

inline void setBit(byte& input, size_t pos, bool val = true)
{
    if (val)
    {
        input |= 1 << pos;
    }
    else
    {
        unsetBit(input, pos);
    }
}

inline void toggleBit(byte& input, size_t pos)
{
    input ^= 1 << pos;
}


namespace Opcode
{

inline byte getDdd(byte opcode)
{
    byte result { 0 };
    setBit(result, 0, getBit(opcode, 3));
    setBit(result, 1, getBit(opcode, 4));
    setBit(result, 2, getBit(opcode, 5));
    return result;
}

inline byte getAaa(byte opcode)
{
    return getDdd(opcode);
}

inline byte getSss(byte opcode)
{
    byte result { 0 };
    setBit(result, 0, getBit(opcode, 0));
    setBit(result, 1, getBit(opcode, 1));
    setBit(result, 2, getBit(opcode, 2));
    return result;
}

} // namespace Opcode


struct emu_error : std::runtime_error
{
        using std::runtime_error::runtime_error;
};

[[noreturn]] inline void error(const std::string& why)
{
    throw emu_error(why);
}

inline std::string toUpper(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

inline std::string toLower(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

} // namepsace i8080

#endif // UTILITY_HPP
