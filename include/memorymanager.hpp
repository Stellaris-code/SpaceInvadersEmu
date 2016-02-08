/*  include/memorymanager.hpp MemoryManager - Yann BOUCHER (yann) 27/01/2016
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
#ifndef MEMORYMANAGER_HPP
#define MEMORYMANAGER_HPP

#include <array>

#include "common.hpp"

namespace i8080
{

class MemoryManager
{
    public:
        MemoryManager()
        { reset(); }

    public:
        byte read(word addr) const;
        void write(word addr, byte data);
        void reset();
        byte* begin()
        { return m_memory.begin(); }
        byte* end()
        { return m_memory.end(); }
        byte* RAM()
        { return begin() + 0x2000; }
        byte* VRAM()
        { return begin() + 0x2400; }
        byte* RAMMirror()
        { return begin() + 0x4000; }

        const byte* cbegin() const
        { return m_memory.data(); }
        const byte* cend() const
        { return m_memory.cend(); }
        const byte* ROM() const
        { return cbegin(); }
        const byte* RAM() const
        { return cbegin() + 0x2000; }
        const byte* VRAM() const
        { return cbegin() + 0x2400; }
        const byte* RAMMirror() const
        { return cbegin() + 0x4000; }


    private:
        std::array<byte, 1 << 16> m_memory {}; // 64KB
};

} // namespace i8080

#endif // MEMORYMANAGER_HPP
