/*  src/screen.cpp Screen - Yann BOUCHER (yann) 24/01/2016
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
#include "screen.hpp"

Screen::Screen(QWidget *parent) : QWidget(parent),
    m_overlay("data/invadersOverlay.png"), m_background("data/backdrop_moon_midway.png"),
    m_screen(c_screen_size, QImage::Format_ARGB32_Premultiplied), m_renderingThread(&Screen::render, this)
{
    m_image = QImage(c_screen_size, QImage::Format_Mono);
    m_image.fill(Qt::black);
    m_overlay = m_overlay.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    setFixedSize(m_image.size().transposed());
}

Screen::~Screen()
{
    stopRendering();
    m_killRendering = true;
    m_renderingThread.join();
}


void Screen::setVRAMAddr(const byte* addr)
{
    m_image = QImage(addr, c_screen_size.width(), c_screen_size.height(), 32, QImage::Format_Mono);
}

void Screen::render()
{
    QPainter painter(&m_screen);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::gray);
    while (true)
    {
        if (m_renderingEnabled)
        {
            std::this_thread::sleep_for(std::chrono::duration<double>(1/120.f));
            emit VBlankMiddle();
            std::this_thread::sleep_for(std::chrono::duration<double>(1/120.f));
            painter.drawPixmap(m_screen.rect(), QPixmap::fromImage(m_image));
            emit VBlank();
        }
        if (m_killRendering)
        {
            painter.end();
            return;
        }
    }
}

void Screen::paintEvent(QPaintEvent * event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.fillRect(m_screen.rect(), Qt::white);
    painter.drawImage(event->rect(), m_screen);
    painter.setCompositionMode(QPainter::CompositionMode_Multiply);
    if (m_drawColorMask)
    {
        painter.drawImage(event->rect(), m_overlay);
    }
}
