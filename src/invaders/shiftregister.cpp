/*  src/shiftregister.cpp ShiftRegister - Yann BOUCHER (yann) 10/02/2016
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
#include "invaders/shiftregister.hpp"

void ShiftRegister::fill(byte val)
{
    m_data >>= 8;
    m_data |= (val << 8);
}

void ShiftRegister::offset(byte off)
{
    assert(off < 8);
    m_offset = off;
}

byte ShiftRegister::result() const
{
    return (m_data << m_offset) >> 8;
}

void ShiftRegister::reset()
{
    m_data = 0;
    m_offset = 0;
}

