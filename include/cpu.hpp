/*  include/cpu.hpp CPU - Yann BOUCHER (yann) 01/02/2016
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
#ifndef CPU_HPP
#define CPU_HPP

#include <chrono>
#include <thread>

#include <QFile>
#include <QString>

#include "common.hpp"
#include "cpustate.hpp"
#include "opcodedefinition.hpp"
#include "utility.hpp"

namespace i8080
{

class CPU
{
    public:
        CPU();
    public:
        void reset(); // Resets CPU
        unsigned int step(); // Execute one instruction; returns cycles taken
        void run(unsigned int cycles); // Run for cycles; return last instruction cycles - If cycles == 0; run endless

        void loadRom(const std::string& h, const std::string& g, const std::string& f, const std::string& e);

        const State& state() const
        { return m_state; }

        void registerOpcode(const OpcodeDefinition& opcode);

    public slots:
        void interrupt(const byte& opcode); // Interrupt CPU

    private:
        byte fetchOpcode(word addr) const;
        unsigned int executeOpcode(const OpcodeDefinition &opcodeDef, byte opcode);

    private:
        std::vector<OpcodeDefinition> m_opcodes;
        State m_state;
};

} // namespace i8080

#endif // CPU_HPP
