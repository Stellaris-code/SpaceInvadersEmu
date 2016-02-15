/*  include/shiftregister.hpp ShiftRegister - Yann BOUCHER (yann) 10/02/2016
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
#ifndef SHIFTREGISTER_HPP
#define SHIFTREGISTER_HPP

#include <cassert>

#include "common.hpp"

class ShiftRegister
{
    public:
        ShiftRegister() = default;

    public:
        void fill(byte val);
        void offset(byte off);

        void reset();

        byte result() const;

    private:
        word m_data { 0 };
        byte m_offset { 0 };
};

#endif // SHIFTREGISTER_HPP
