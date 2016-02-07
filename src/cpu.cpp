/*  src/cpu.cpp CPU - Yann BOUCHER (yann) 01/02/2016
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
#include "cpu.hpp"

namespace i8080
{

CPU::CPU()
{
    reset();
}

void CPU::reset()
{
    m_state.reset();
}

void CPU::loadRom(const std::string& h, const std::string& g, const std::string& f, const std::string& e)
{
    bool sucess = false;
    QFile file_h(QString::fromStdString(h));
    sucess &= file_h.open(QIODevice::ReadOnly);
    QFile file_g(QString::fromStdString(g));
    sucess &= file_g.open(QIODevice::ReadOnly);
    QFile file_f(QString::fromStdString(f));
    sucess &= file_f.open(QIODevice::ReadOnly);
    QFile file_e(QString::fromStdString(e));
    sucess &= file_e.open(QIODevice::ReadOnly);

    if (!sucess)
    {
        emu_error("Cannot open rom files !");
    }
    else
    {
        std::copy(file_h.readAll().cbegin(), file_h.readAll().cend(),
                  m_state.mem.begin());
        std::copy(file_g.readAll().cbegin(), file_h.readAll().cend(),
                  m_state.mem.begin() + 0x800);
        std::copy(file_f.readAll().cbegin(), file_h.readAll().cend(),
                  m_state.mem.begin() + 0x1000);
        std::copy(file_e.readAll().cbegin(), file_h.readAll().cend(),
                  m_state.mem.begin() + 0x1800);
    }
}

unsigned int CPU::step()
{
    byte opcode = fetchOpcode(m_state.pc++);
    bool interrupted = false;

    if (m_state.int_enabled && m_state.interrupt)
    {
        interrupted = true;
        m_state.int_enabled = false;
        m_state.interrupt = false;
        opcode = m_state.interrupt_opcode;
        m_state.interrupt_opcode = 0;
    }

    for (auto opcodeDef : m_opcodes)
    {
        if (opcodeDef.isMatching(opcode))
        {
            auto clockCycles = executeOpcode(opcodeDef, opcode);
            return clockCycles;
        }
    }
    error((QString("Illegal instruction : 0x") + QString::number(opcode, 16) +  " (Address : 0x" +
                  QString::number(m_state.pc - 1, 16) + ") " + (interrupted ? " (interruption)" : "")).toStdString());
}

void CPU::run(unsigned int cycles)
{
    std::thread([this, cycles]{

        if (cycles == 0)
        {
            while(true)
            {
                step();
            }
        }
        else
        {
            unsigned int totalCycles = 0;
            while (totalCycles < cycles)
            {
                totalCycles += step();
            }
        }
    }).detach();
}

void CPU::interrupt(const byte& opcode)
{
    if (m_state.int_enabled)
    {
        m_state.interrupt = true;
        m_state.interrupt_opcode = opcode;
    }
}

void CPU::registerOpcode(const OpcodeDefinition& opcode)
{
    auto result = std::find(m_opcodes.begin(), m_opcodes.end(), opcode);
    if (result == m_opcodes.cend())
    {
        m_opcodes.push_back(opcode);
        std::sort(m_opcodes.begin(), m_opcodes.end(), [](const OpcodeDefinition& a, const OpcodeDefinition& b) -> bool
        {
            return a.wildcardAmount() < b.wildcardAmount();
        });
    }
    else
    {
        *result = opcode;
    }
}

byte CPU::fetchOpcode(word addr) const
{
    return m_state.mem.read(addr);
}

unsigned int CPU::executeOpcode(const OpcodeDefinition& opcodeDef, byte opcode)
{
    auto time1 = std::chrono::high_resolution_clock::now();
    unsigned int clockCycles = opcodeDef.execute(opcode, m_state);
    std::chrono::seconds duration(clockCycles / c_clock_speed);
    auto time2 = std::chrono::high_resolution_clock::now();
    auto delta = time2 - time1;

    if (delta < duration)
    {
        std::this_thread::sleep_for(duration - delta);
    }

    return clockCycles;
}

} // namespace i8080
