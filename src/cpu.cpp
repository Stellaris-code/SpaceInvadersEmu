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
    QFile file_h(QString::fromStdString(h));
    if(!file_h.open(QIODevice::ReadOnly))
    {
        error("Cannot open rom 1 : " + file_h.errorString().toStdString() + " : " +
              std::to_string(file_h.error()));
    }
    QFile file_g(QString::fromStdString(g));
    if(!file_g.open(QIODevice::ReadOnly))
    {
        error("Cannot open rom 2 : " + file_g.errorString().toStdString() + " : " +
              std::to_string(file_h.error()));
    }
    QFile file_f(QString::fromStdString(f));
    if(!file_f.open(QIODevice::ReadOnly))
    {
        error("Cannot open rom 3 : " + file_f.errorString().toStdString() + " : " +
              std::to_string(file_h.error()));
    }
    QFile file_e(QString::fromStdString(e));
    if(!file_e.open(QIODevice::ReadOnly))
    {
        error("Cannot open rom 4 : " + file_e.errorString().toStdString() + " : " +
              std::to_string(file_h.error()));
    }
    QByteArray hContents = file_h.readAll();
    std::copy(hContents.constBegin(), hContents.constEnd(),
              m_state.mem.begin());
    QByteArray gContents = file_g.readAll();
    std::copy(gContents.constBegin(), gContents.constEnd(),
              m_state.mem.begin() + 0x800);
    QByteArray fContents = file_f.readAll();
    std::copy(fContents.constBegin(), fContents.constEnd(),
              m_state.mem.begin() + 0x1000);
    QByteArray eContents = file_e.readAll();
    std::copy(eContents.constBegin(), eContents.constEnd(),
              m_state.mem.begin() + 0x1800);
}

unsigned int CPU::step()
{
    byte opcode = fetchOpcode(m_state.pc++);
    bool interrupted = false;

    if (m_state.int_enabled && m_state.interrupt)
    {
        interrupted = true;
        m_state.halt_flag = false;
        m_state.int_enabled = false;
        m_state.interrupt = false;
        opcode = m_state.interrupt_opcode;
        m_state.interrupt_opcode = 0;
    }

    if (!m_state.halt_flag)
    {

        for (auto opcodeDef : m_opcodes)
        {
            if (opcodeDef.isMatching(opcode))
            {
                auto clockCycles = executeOpcode(opcodeDef, opcode);
                return clockCycles;
            }
        }

    }
    else
    {
        return 1;
    }
    error((QString("Illegal instruction : 0x") + QString::number(opcode, 16) + "/" + QString::number(opcode, 2) +
           (interrupted ? " (interruption)" :  " (Address : 0x" + QString::number(m_state.pc - 1, 16) + ")")).toStdString());
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
