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

struct State
{
        std::array<byte, 8> registers {};
        byte* const B { &registers[0] };
        byte* const C { &registers[1] };
        byte* const D { &registers[2] };
        byte* const E { &registers[3] };
        byte* const H { &registers[4] };
        byte* const L { &registers[5] };
        byte* const A { &registers[6] };
        byte* const F { &registers[7] };

        word* const BC { reinterpret_cast<word* const>(B) };
        word* const DE { reinterpret_cast<word* const>(D) };
        word* const HL { reinterpret_cast<word* const>(H) };
        word* const AF { reinterpret_cast<word* const>(A) };

        word sp { 0 };
        word pc { 0 };

        MemoryManager mem {};

        IOManager io {};

        std::atomic_bool int_enabled { true };

        bool interrupt { false };
        byte interrupt_opcode { 0 };



        void reset()
        {
            registers.fill(0);
            sp = 0;
            pc = 0;
            mem.reset();
            io.reset();
            int_enabled = true;
            interrupt = false;
            interrupt_opcode = 0;
        }
};

typedef std::function<unsigned int(byte, State&)> opcodeCallback;

} // namespace i8080

#endif // CPUSTATE_HPP
