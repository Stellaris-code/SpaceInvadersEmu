/*  src/memorymanager.cpp MemoryManager - Yann BOUCHER (yann) 27/01/2016
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
#include "memorymanager.hpp"

namespace i8080
{

byte MemoryManager::read(word addr) const
{
        return m_memory[addr];
}

void MemoryManager::write(word addr, byte data)
{
    if (addr < c_RAMMirrorAddr)
    {
        m_memory[addr] = data;
    }
    else
    {
        write(wrap<word>(addr, 0, c_RAMMirrorAddr - 1), data);
    }
}

void MemoryManager::reset()
{
    m_memory.fill(0);
}

} // namespace i8080
