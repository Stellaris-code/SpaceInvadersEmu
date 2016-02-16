/*  include/invaders/invadersboard.hpp InvadersBoard - Yann BOUCHER (yann) 11/02/2016
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
#ifndef INVADERSBOARD_HPP
#define INVADERSBOARD_HPP

#include <atomic>
#include <functional>
#include <thread>

#include "common.hpp"

#include "motherboard.hpp"
#include "screen.hpp"
#include "opcodes.hpp"
#include "utility.hpp"

#include "invaders/shiftregister.hpp"
#include "invaders/soundmanager.hpp"


class InvadersBoard : public Motherboard
{

    public:
        InvadersBoard();
        virtual ~InvadersBoard() = default;

    public:
        virtual void registerIOPorts() override;
        virtual void start() override;
        virtual void reset() override;

    public:
        void setupScreen(Screen& screen);
        void setInputCheckCallback(std::function<void(void)> callback)
        { m_inputCallback = callback; }

        word highScore() const
        { return m_highScore; }

        void resetHighScore()
        { m_highScore = 0; }

        void enableSound(bool val)
        { m_sound.setEnabled(val); }
        void setVolume(qreal volume)
        { m_sound.setVolume(volume); }

    public:
        bool credit = false;
        bool p2Start = false;
        bool p1Start = false;
        bool p1Shoot = false;
        bool p1Left = false;
        bool p1Right = false;

        bool dip_shipNumberb0 = 0;
        bool dip_shipNumberb1 = 0;
        bool tilt = false;
        bool dip_extraShipScoreBonus = false;
        bool p2Shoot = false;
        bool p2Left = false;
        bool p2Right = false;
        bool dip_coinInfo = false;

    private:
        byte port1InputToByte() const
        {
            byte result = 0;
            i8080::setBit(result, 0, credit);
            i8080::setBit(result, 1, p2Start);
            i8080::setBit(result, 2, p1Start);
            i8080::setBit(result, 3, 1);
            i8080::setBit(result, 4, p1Shoot);
            i8080::setBit(result, 5, p1Left);
            i8080::setBit(result, 6, p1Right);
            i8080::setBit(result, 7, 0);
            return result;
        }

        byte port2InputToByte() const
        {
            byte result = 0;
            i8080::setBit(result, 0, dip_shipNumberb0);
            i8080::setBit(result, 1, dip_shipNumberb1);
            i8080::setBit(result, 2, tilt);
            i8080::setBit(result, 3, dip_extraShipScoreBonus);
            i8080::setBit(result, 4, p2Shoot);
            i8080::setBit(result, 5, p2Left);
            i8080::setBit(result, 6, p2Right);
            i8080::setBit(result, 7, dip_coinInfo);
            return result;
        }

        void checkHighScore();

    private:
        ShiftRegister m_shrg;
        std::shared_ptr<Screen> m_screenref;
        std::function<void(void)> m_inputCallback;
        std::atomic<unsigned int> m_watchdog_timer { 0 };
        SoundManager m_sound;
        word m_highScore { 0 };
};

#endif // INVADERSBOARD_HPP

