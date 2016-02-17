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

#include "array_view/array_view.hpp"

#include "common.hpp"
#include "noncopyable.hpp"

namespace i8080
{

class MemoryManager : private NonCopyable
{
    public:
        MemoryManager()
        { reset(); }

    private:
        std::array<byte, 1 << 16> m_memory {}; // 64KB

    public:
        byte read(word addr) const;
        void write(word addr, byte data);
        void reset();

        auto begin()
        { return m_memory.begin(); }
        auto end()
        { return m_memory.end(); }

        auto cbegin() const
        { return m_memory.cbegin(); }
        auto cend() const
        { return m_memory.cend(); }

        arv::array_view<byte> RAM()
        { return arv::array_view<byte>{m_memory.begin() + 0x2000, m_memory.begin() + 0x23FF}; }
        arv::array_view<byte> VRAM()
        { return arv::array_view<byte>{m_memory.begin() + 0x2400, m_memory.begin() + 0x3FFF}; }
        arv::array_view<byte> RAMMirror()
        { return arv::array_view<byte>{m_memory.begin() + 0x4000, m_memory.end()}; }

        const arv::array_view<byte> ROM() const
        { return arv::array_view<byte>{m_memory.cbegin(), m_memory.cbegin() + 0x1FFF}; }
        const arv::array_view<byte> RAM() const
        { return arv::array_view<byte>{m_memory.begin() + 0x2000, m_memory.begin() + 0x23FF}; }
        const arv::array_view<byte> VRAM() const
        { return arv::array_view<byte>{m_memory.begin() + 0x2400, m_memory.begin() + 0x3FFF}; }
        const arv::array_view<byte> RAMMirror() const
        { return arv::array_view<byte>{m_memory.begin() + 0x4000, m_memory.end()}; }


};

} // namespace i8080

#endif // MEMORYMANAGER_HPP
