/*  src/iomanager.cpp IOManager - Yann BOUCHER (yann) 27/01/2016
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
#include "iomanager.hpp"

namespace i8080
{

IOManager::IOManager(const std::function<byte(void)> &inDefaultHandler, const std::function<void(byte)> &outDefaultHandler)
{
    assert(inDefaultHandler && outDefaultHandler);

    m_inports.fill(inDefaultHandler);
    m_outports.fill(outDefaultHandler);
}

byte IOManager::inb(byte port) const
{
    return m_inports[port]();
}

void IOManager::outb(byte port, byte data)
{
    m_outports[port](data);
}

void IOManager::reset()
{

}

} // namespace i8080
