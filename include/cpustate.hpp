/*  cpustate %{Cpp:License:ClassName} - Yann BOUCHER (yann) 26/01/2016
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
#ifndef CPUSTATE_HPP
#define CPUSTATE_HPP

#include <cmath>

#include <array>
#include <atomic>
#include <functional>
#include <map>
#include <vector>

#include "common.hpp"
#include "iomanager.hpp"
#include "memorymanager.hpp"

namespace i8080
{

class MemoryManager;

struct State
{
        std::array<byte, 8> registers {};
        byte& C { registers[0] };
        byte& B { registers[1] };
        byte& E { registers[2] };
        byte& D { registers[3] };
        byte& L { registers[4] };
        byte& H { registers[5] };
        byte& F { registers[6] };
        byte& A { registers[7] };


        word& BC { reinterpret_cast<word&>(C) };
        word& DE { reinterpret_cast<word&>(E) };
        word& HL { reinterpret_cast<word&>(L) };
        word& AF { reinterpret_cast<word&>(F) };

        word sp { 0 };
        word pc { 0 };

        MemoryManager mem;

        IOManager io {};

        std::atomic_bool int_enabled { true };

        bool interrupt { false };
        byte interrupt_opcode { 0 };

        bool halt_flag { false };

        void reset();
};

typedef std::function<unsigned int(byte, State&)> opcodeCallback;

} // namespace i8080

#endif // CPUSTATE_HPP
