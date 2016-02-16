/*  src/invaders/invadersboard.cpp InvadersBoard - Yann BOUCHER (yann) 11/02/2016
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
#include "invaders/invadersboard.hpp"

#include <QDebug>

InvadersBoard::InvadersBoard()
    : Motherboard()
{
    i8080::registerOpcodes(m_cpu);
    m_cpu.loadRom("roms/invaders.h", "roms/invaders.g", "roms/invaders.f", "roms/invaders.e");
    registerIOPorts();
}

void InvadersBoard::setupScreen(Screen& screen)
{
    screen.setVRAMAddr(m_cpu.state().mem.VRAM().data());
    screen.startRendering();

    screen.connect(&screen, &Screen::VBlank, [this]{
        m_cpu.interrupt(0xCF); // RST 8
    });
    screen.connect(&screen, &Screen::VBlankMiddle, [this]{
        m_cpu.interrupt(0xD7); // RST 10
    });
}

void InvadersBoard::checkHighScore()
{
    word val = std::max(std::max((m_cpu.state().mem.read(0x20F4) | (m_cpu.state().mem.read(0x20F5) << 8)),
                                 (m_cpu.state().mem.read(0x20F8) | (m_cpu.state().mem.read(0x20F9) << 8))),
                                  m_cpu.state().mem.read(0x20FC) | (m_cpu.state().mem.read(0x20FD) << 8));
    if (val > m_highScore)
    {
        m_highScore = val;
    }
}

void InvadersBoard::start()
{
    m_cpu.run(0);

/*  std::thread([this]
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // Wait for invaders to init
        while (m_cpu.running())
        {
            std::this_thread::sleep_for(std::chrono::duration<double>(1/m_cpu.clockSpeed));
            if (++m_watchdog_timer == 255)
            {
                std::cerr << "Watchdog reset !\n";
                reset();
                start();
                return;
            }
        }
    }).detach(); // Watchdog thread
*/

}

void InvadersBoard::reset()
{
    m_cpu.reset();
    m_shrg.reset();
    m_sound.reset();
    m_cpu.loadRom("roms/invaders.h", "roms/invaders.g", "roms/invaders.f", "roms/invaders.e");
}

void InvadersBoard::registerIOPorts()
{
    cpuState().io.mapInPort(0, []{
        return 0xE; // Mapped port but unused in code
    });
    cpuState().io.mapInPort(1, [this]{
        if (m_inputCallback)
        {
            m_inputCallback();
        }
        return port1InputToByte();
    });
    cpuState().io.mapInPort(2, [this]{
        if (m_inputCallback)
        {
            m_inputCallback();
        }
        return port2InputToByte();
    });
    cpuState().io.mapInPort(3, [this]{
        return m_shrg.result();
    });



    cpuState().io.mapOutPort(2, [this](byte data)
    {
        m_shrg.offset(data & 0x7);
    });

    cpuState().io.mapOutPort(3, [this](byte data)
    {
        m_sound.playUFO(i8080::getBit(data, 0));
        m_sound.playShoot(i8080::getBit(data, 1));
        m_sound.playPlayerDie(i8080::getBit(data, 2));
        m_sound.playAlienDie(i8080::getBit(data, 3));
        checkHighScore();
    });

    cpuState().io.mapOutPort(4, [this](byte data)
    {
        m_shrg.fill(data);
    });

    cpuState().io.mapOutPort(5, [this](byte data)
    {
        m_sound.playAlienMove1(i8080::getBit(data, 0));
        m_sound.playAlienMove2(i8080::getBit(data, 1));
        m_sound.playAlienMove3(i8080::getBit(data, 2));
        m_sound.playAlienMove4(i8080::getBit(data, 3));
        m_sound.playUFOHit(i8080::getBit(data, 4));
        checkHighScore();
    });

    cpuState().io.mapOutPort(6, [this](byte)
    {
        m_watchdog_timer = 0;
    });
}
