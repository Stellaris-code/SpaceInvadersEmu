/*  utility %{Cpp:License:ClassName} - Yann BOUCHER (yann) 07/02/2016
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


#include "utility.hpp"

namespace i8080
{

void checkACarry(byte& flags, unsigned int val, const State& state)
{
    setFlagAC(flags, (state.A & 0xF) > (val & 0xF));
}

namespace Opcode
{

byte fetchByte(State& state)
{
    return state.mem.read(state.pc++);
}

word fetchWord(State& state)
{
    return word(fetchByte(state) | fetchByte(state) << 8);
}

byte readReg(unsigned int index, const State& state)
{
    switch(index)
    {
        case 0x00:
            return state.B;
        case 0x01:
            return state.C;
        case 0x02:
            return state.D;
        case 0x03:
            return state.E;
        case 0x04:
            return state.H;
        case 0x05:
            return state.L;
        case 0x06:
            return state.mem.read(*state.HL);
        case 0x07:
            return state.A;
        default:
            return 0;
    }
}

void writeReg(unsigned int index, byte val, State& state)
{
    switch(index)
    {
        case 0x00:
            state.B = val; break;
        case 0x01:
            state.C = val; break;
        case 0x02:
            state.D = val; break;
        case 0x03:
            state.E = val; break;
        case 0x04:
            state.H = val; break;
        case 0x05:
            state.L = val; break;
        case 0x06:
            state.mem.write(*state.HL, val); break;
        case 0x07:
            state.A = val; break;
    }
}

} // namespace i8080

} // namespace i8080
