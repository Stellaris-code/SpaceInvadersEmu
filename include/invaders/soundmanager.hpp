/*  include/invaderssoundmanager.hpp SoundManager - Yann BOUCHER (yann) 13/02/2016
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
#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include <QSoundEffect>
#include <QUrl>


class SoundManager
{
    public:
        SoundManager();

    public:
        void playUFO(bool val);
        void playShoot(bool val);
        void playPlayerDie(bool val);
        void playAlienDie(bool val);
        void playAlienMove1(bool val);
        void playAlienMove2(bool val);
        void playAlienMove3(bool val);
        void playAlienMove4(bool val);
        void playUFOHit(bool val);

        void reset();

    public slots:
        void setEnabled(bool enabled);
        void setVolume(qreal volume);

    private:
        bool m_enabled = true;

        QSoundEffect m_UFO;
        bool m_UFOPlaying { false };
        QSoundEffect m_Shoot;
        bool m_ShootPlaying { false };
        QSoundEffect m_PlyDie;
        bool m_PlyDiePlaying { false };
        QSoundEffect m_AlDie;
        bool m_AlDiePlaying { false };
        QSoundEffect m_AlMove1;
        bool m_AlMove1Playing { false };
        QSoundEffect m_AlMove2;
        bool m_AlMove2Playing { false };
        QSoundEffect m_AlMove3;
        bool m_AlMove3Playing { false };
        QSoundEffect m_AlMove4;
        bool m_AlMove4Playing { false };
        QSoundEffect m_UFOHit;
        bool m_UFOHitPlaying { false };
};

#endif // SOUNDMANAGER_HPP
