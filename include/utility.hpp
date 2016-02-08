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
#include <climits>

#include <algorithm>
#include <chrono>
#include <stdexcept>
#include <string>
#include <thread>

#include "common.hpp"
#include "cpustate.hpp"

namespace i8080
{

struct State;

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

inline bool parity(byte input)
{
    input ^= input >> 4;
    input &= 0xF;
    return (0x6996 >> input) & 1;
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

inline bool getFlagS(byte flags)
{
    return getBit(flags, 7);
}
inline void setFlagS(byte& flags, bool val)
{
    setBit(flags, 7, val);
}

inline bool getFlagZ(byte flags)
{
    return getBit(flags, 6);
}
inline void setFlagZ(byte& flags, bool val)
{
    setBit(flags, 6, val);
}

inline bool getFlagAC(byte flags)
{
    return getBit(flags, 4);
}
inline void setFlagAC(byte& flags, bool val)
{
    setBit(flags, 4, val);
}

inline bool getFlagP(byte flags)
{
    return getBit(flags, 2);
}
inline void setFlagP(byte& flags, bool val)
{
    setBit(flags, 2, val);
}

inline bool getFlagC(byte flags)
{
    return getBit(flags, 0);
}
inline void setFlagC(byte& flags, bool val)
{
    setBit(flags, 0, val);
}

inline bool sign(byte val)
{
    return getBit(val, 7);
}


void checkACarry(byte& flags, unsigned int val, const State& state);

inline void checkSign(byte& flags, byte val)
{
    setFlagS(flags, sign(val));
}
inline void checkZero(byte& flags, unsigned int val)
{
    setFlagZ(flags, val == 0);
}
inline void checkCarry(byte& flags, unsigned int val)
{
    setFlagC(flags, (val & 0x100) != 0);
}
inline void checkParity(byte& flags, byte val)
{
    setFlagAC(flags, parity(val));
}

inline byte rotl(byte x, unsigned int n)
{
  const decltype(n) mask = (CHAR_BIT*sizeof(x)-1);
  assert (n<=mask && "rotate by more than type width");
  n &= mask;  // avoid undef behaviour with NDEBUG.  0 overhead for most types / compilers
  return (x<<n) | (x>>( (-n)&mask ));
}

inline byte rotr(byte x, unsigned int n)
{
  const decltype(n) mask = (CHAR_BIT*sizeof(x)-1);
  assert (n<=mask && "rotate by more than type width");
  n &= mask;  // avoid undef behaviour with NDEBUG.  0 overhead for most types / compilers
  return (x>>n) | (x<<( (-n)&mask ));
}


namespace Opcode
{

byte fetchByte(State& state);
word fetchWord(State& state);

byte readReg(unsigned int index, const State& state);

void writeReg(unsigned int index, byte val, State& state);

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
