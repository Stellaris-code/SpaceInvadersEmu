/*  include/motherboard.hpp Motherboard - Yann BOUCHER (yann) 11/02/2016
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
#ifndef MOTHERBOARD_HPP
#define MOTHERBOARD_HPP

#include "common.hpp"

#include "cpu.hpp"
#include "cpustate.hpp"
#include "utility.hpp"
#include "noncopyable.hpp"

class Motherboard : private NonCopyable
{
    public:
        Motherboard();
        virtual ~Motherboard() = default;

    protected:
        virtual void registerIOPorts()
        {}

    public:
        virtual void start()
        {}
        virtual void reset()
        {}

    public:
        void pause()
        { m_cpu.pause(); }
        void resume()
        { m_cpu.resume(); }
        void setPaused(bool val)
        {
            if (val)
            {
                pause();
            }
            else
            {
                resume();
            }
        }
        bool paused() const
        { return m_cpu.paused(); }

        i8080::State& cpuState()
        { return m_cpu.m_state; }

        double ips() const
        { return m_cpu.ips(); }

        virtual unsigned long clockSpeed() const
        { return 2000000; }

    protected:
        i8080::CPU m_cpu;
};

#endif // MOTHERBOARD_HPP
