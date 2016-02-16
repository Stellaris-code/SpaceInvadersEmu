/*  include/iomanager.hpp IOManager - Yann BOUCHER (yann) 27/01/2016
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
#ifndef IOMANAGER_HPP
#define IOMANAGER_HPP

#include <cassert>

#include <array>
#include <limits>
#include <functional>

#include "common.hpp"

namespace i8080
{

class IOManager
{
    public:
       explicit IOManager(const std::function<byte(void)>& inDefaultHandler = []() -> byte { return 0; },
                  const std::function<void(byte)>& outDefaultHandler = [](byte){});

    public:
        byte inb(byte port) const;
        void outb(byte port, byte data);
        void reset();

        void mapInPort(byte port, std::function<byte(void)> callback);
        void mapOutPort(byte port, std::function<void(byte)> callback);

    private:
        std::array<std::function<byte(void)>, std::numeric_limits<byte>::max()> m_inports {};
        std::array<std::function<void(byte)>, std::numeric_limits<byte>::max()> m_outports {};
};

}

#endif // IOMANAGER_HPP
