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

class Motherboard;

namespace i8080
{

class CPU
{
        friend class ::Motherboard;

    public:
        CPU();
        ~CPU();
    public:
        void reset(); // Resets CPU
        unsigned int step(); // Execute one instruction; returns cycles taken
        void run(unsigned long long steps); // Run for steps; If steps == 0; run endless

        void loadRom(const std::string& h, const std::string& g, const std::string& f, const std::string& e);

        const State& state() const
        { return m_state; }

        void registerOpcode(const OpcodeDefinition& opcode);

        unsigned long long totalSteps() const
        { return m_totalSteps; }

        double ips() const
        {
            if (paused())
            {
                return 0;
            }
            else
            {
                return 1.f / (std::chrono::duration_cast<std::chrono::milliseconds>(
                                  std::chrono::steady_clock::now() - m_runStartPoint).count()
                              / 1000.f / m_totalSteps);
            }
        }

    public slots:
        void interrupt(byte opcode); // Interrupt CPU
        void pause()
        { m_paused = true; }
        void resume()
        {
            m_paused = false;
            m_runStartPoint = std::chrono::steady_clock::now();
            m_totalSteps = 0;
        }
        bool paused() const
        { return m_paused; }

        bool running() const
        { return m_running; }

    public:
        unsigned long long clockSpeed { 2000000 };

    private:
        byte fetchOpcode(word addr) const;
        unsigned int executeOpcode(const OpcodeDefinition &opcodeDef, byte opcode);

    private:
        std::vector<OpcodeDefinition> m_opcodes;
        State m_state;
        std::atomic_ullong m_totalSteps { 0 };
        std::atomic_bool m_paused { false };
        std::atomic_bool m_resetting { false };
        std::atomic_bool m_running { false };
        std::thread m_runThread;
        std::chrono::steady_clock::time_point m_runStartPoint;
};

} // namespace i8080

#endif // CPU_HPP
