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

#include <QWidget>
#include <QSize>

#include "common.hpp"
#include "utility.hpp"

class Screen : public QWidget
{
        Q_OBJECT

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

    protected:
        virtual void paintEvent(QPaintEvent *) override;

    private:
        void render();

    signals:
        void VBlankMiddle();
        void VBlank();

    public slots:
        void setColorOverlay(bool val)
        { m_drawColorMask = val; }

    private:
        QImage m_image;
        QImage m_overlay;
        QImage m_background;
        bool m_drawColorMask { true };
        QImage m_screen;
        std::atomic_bool m_renderingEnabled { false };
        std::atomic_bool m_killRendering { false };
        std::thread m_renderingThread;
};

#endif // SCREEN_HPP
