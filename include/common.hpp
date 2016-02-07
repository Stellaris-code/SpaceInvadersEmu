/*  common %{Cpp:License:ClassName} - Yann BOUCHER (yann) 24/01/2016
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
#ifndef COMMON_HPP
#define COMMON_HPP

#include <cstdint>

#include <QSize>

typedef uint8_t byte;
typedef uint16_t word;

constexpr word c_ROMAddr = 0x0000;
constexpr word c_RAMAddr = 0x2000;
constexpr word c_VRAMAddr = 0x2400;
constexpr word c_RAMMirrorAddr = 0x4000;

constexpr unsigned long c_clock_speed = 2000000; // 2 MHz

constexpr QSize c_screen_size = QSize(256, 224);


#endif // COMMON_HPP
