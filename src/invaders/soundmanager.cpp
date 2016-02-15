/*  src/invaders/soundmanager.cpp SoundManager - Yann BOUCHER (yann) 13/02/2016
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
#include "invaders/soundmanager.hpp"

SoundManager::SoundManager()
{
    m_UFO.setSource(QUrl::fromLocalFile("data/sfx/ufo.wav"));
    m_Shoot.setSource(QUrl::fromLocalFile("data/sfx/shoot.wav"));
    m_PlyDie.setSource(QUrl::fromLocalFile("data/sfx/explosion.wav"));
    m_AlDie.setSource(QUrl::fromLocalFile("data/sfx/invaderkilled.wav"));
    m_AlMove1.setSource(QUrl::fromLocalFile("data/sfx/fastinvader1.wav"));
    m_AlMove2.setSource(QUrl::fromLocalFile("data/sfx/fastinvader2.wav"));
    m_AlMove3.setSource(QUrl::fromLocalFile("data/sfx/fastinvader3.wav"));
    m_AlMove4.setSource(QUrl::fromLocalFile("data/sfx/fastinvader4.wav"));
    m_UFOHit.setSource(QUrl::fromLocalFile("data/sfx/ufo_hit.wav"));
}

void SoundManager::setEnabled(bool enabled)
{
    m_UFO.setMuted(!enabled);
    m_Shoot.setMuted(!enabled);
    m_PlyDie.setMuted(!enabled);
    m_AlDie.setMuted(!enabled);
    m_AlMove1.setMuted(!enabled);
    m_AlMove2.setMuted(!enabled);
    m_AlMove3.setMuted(!enabled);
    m_AlMove4.setMuted(!enabled);
    m_UFOHit.setMuted(!enabled);
}

void SoundManager::setVolume(qreal volume)
{
    m_UFO.setVolume(volume);
    m_Shoot.setVolume(volume);
    m_PlyDie.setVolume(volume);
    m_AlDie.setVolume(volume);
    m_AlMove1.setVolume(volume);
    m_AlMove2.setVolume(volume);
    m_AlMove3.setVolume(volume);
    m_AlMove4.setVolume(volume);
    m_UFOHit.setVolume(volume);
}

void SoundManager::reset()
{
    m_UFOPlaying = false;
    m_ShootPlaying = false;
    m_PlyDiePlaying = false;
    m_AlDiePlaying = false;
    m_AlMove1Playing = false;
    m_AlMove2Playing = false;
    m_AlMove3Playing = false;
    m_AlMove4Playing = false;
    m_UFOHitPlaying = false;
}

void SoundManager::playUFO(bool val)
{
    if (val)
    {
        if (!m_UFOPlaying)
        {
            m_UFOPlaying = true;
            m_UFO.play();
        }
    }
    else
    {
        m_UFOPlaying = false;
    }
}

void SoundManager::playShoot(bool val)
{
    if (val)
    {
        if (!m_ShootPlaying)
        {
            m_ShootPlaying = true;
            m_Shoot.play();
        }
    }
    else
    {
        m_ShootPlaying = false;
    }
}

void SoundManager::playPlayerDie(bool val)
{
    if (val)
    {
        if (!m_PlyDiePlaying)
        {
            m_PlyDiePlaying = true;
            m_PlyDie.play();
        }
    }
    else
    {
        m_PlyDiePlaying = false;
    }
}

void SoundManager::playAlienDie(bool val)
{
    if (val)
    {
        if (!m_AlDiePlaying)
        {
            m_AlDiePlaying = true;
            m_AlDie.play();
        }
    }
    else
    {
        m_AlDiePlaying = false;
    }
}

void SoundManager::playAlienMove1(bool val)
{
    if (val)
    {
        if (!m_AlMove1Playing)
        {
            m_AlMove1Playing = true;
            m_AlMove1.play();
        }
    }
    else
    {
        m_AlMove1Playing = false;
    }
}

void SoundManager::playAlienMove2(bool val)
{
    if (val)
    {
        if (!m_AlMove2Playing)
        {
            m_AlMove2Playing = true;
            m_AlMove2.play();
        }
    }
    else
    {
        m_AlMove2Playing = false;
    }
}

void SoundManager::playAlienMove3(bool val)
{
    if (val)
    {
        if (!m_AlMove3Playing)
        {
            m_AlMove3Playing = true;
            m_AlMove3.play();
        }
    }
    else
    {
        m_AlMove3Playing = false;
    }
}

void SoundManager::playAlienMove4(bool val)
{
    if (val)
    {
        if (!m_AlMove4Playing)
        {
            m_AlMove4Playing = true;
            m_AlMove4.play();
        }
    }
    else
    {
        m_AlMove4Playing = false;
    }
}

void SoundManager::playUFOHit(bool val)
{
    if (val)
    {
        if (!m_UFOHitPlaying)
        {
            m_UFOHitPlaying = true;
            m_UFOHit.play();
        }
    }
    else
    {
        m_UFOHitPlaying = false;
    }
}
