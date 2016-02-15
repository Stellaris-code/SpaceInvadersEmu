/*  include/screen.hpp Screen - Yann BOUCHER (yann) 24/01/2016
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
#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <array>
#include <atomic>
#include <chrono>
#include <thread>

#include <QImage>
#include <QPainter>
#include <QPaintEvent>
#include <QPropertyAnimation>

#include <QWidget>
#include <QSize>

#include "common.hpp"
#include "utility.hpp"

class Screen : public QWidget
{
        Q_OBJECT
        Q_PROPERTY(qreal pauseOpacity MEMBER m_pauseOpacity)

    public:
        explicit Screen(QWidget *parent = 0);

        ~Screen();

    public:
        void setVRAMAddr(const byte* addr);
        void startRendering()
        {
            m_renderingEnabled = true;
        }
        void stopRendering()
        { m_renderingEnabled = false; }

    signals:
        void VBlankMiddle();
        void VBlank();

    protected:
        virtual void paintEvent(QPaintEvent *) override;

    public slots:
        void setColorOverlay(bool val)
        { m_drawColorMask = val; }
        void setBackgroundImage(bool val)
        { m_drawBackground = val; }
        void setPaused(bool val)
        {
            m_paused = val;
            m_pauseAnim.setDirection(val ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
            m_pauseAnim.start();
        }
        void setScale(qreal scale)
        {
            setFixedSize(size() * scale);
        }


    private:
        void render();

        void setPauseOpacity(qreal val)
        {
            m_pauseOpacity = val;
        }
        qreal pauseOpacity() const
        {
            return m_pauseOpacity;
        }

    private:
        QImage m_image;
        QImage m_overlay;
        QImage m_background;
        QImage m_pauseIcon;
        bool m_drawColorMask { true };
        bool m_drawBackground { false };
        QImage m_screen;
        std::atomic_bool m_renderingEnabled { false };
        std::atomic_bool m_killRendering { false };
        std::atomic_bool m_paused { false };
        std::thread m_renderingThread;
        QPropertyAnimation m_pauseAnim;
        qreal m_pauseOpacity { 0 };
};

#endif // SCREEN_HPP
