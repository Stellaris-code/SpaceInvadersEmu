/*  cpustate %{Cpp:License:ClassName} - Yann BOUCHER (yann) 07/02/2016
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

#include "cpustate.hpp"

namespace i8080
{

void State::reset()
{
    registers.fill(0);
    F = 2; //  00000010
    sp = 0;
    pc = 0;
    mem.reset();
    io.reset();
    int_enabled = true;
    interrupt = false;
    interrupt_opcode = 0;
}

} // namespace i8080
