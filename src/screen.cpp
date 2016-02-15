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
    m_overlay("data/invadersOverlay.png"), m_background("data/invaders.png"), m_pauseIcon("data/pause.png"),
    m_screen(c_screen_size.transposed(), QImage::Format_ARGB32_Premultiplied), m_renderingThread(&Screen::render, this),
    m_pauseAnim(this, "pauseOpacity")
{
    m_image = QImage(c_screen_size, QImage::Format_MonoLSB);
    m_image.fill(Qt::black);
    m_overlay = m_overlay.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    m_background = m_background.scaled(c_screen_size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation)
                   .convertToFormat(QImage::Format_ARGB32_Premultiplied);
    setFixedSize(m_image.size().transposed());

    m_pauseAnim.setDuration(150);
    m_pauseAnim.setStartValue(qreal(0));
    m_pauseAnim.setEndValue(qreal(1));
}

Screen::~Screen()
{
    stopRendering();
    m_killRendering = true;
    m_renderingThread.join();
}


void Screen::setVRAMAddr(const byte* addr)
{
    m_image = QImage(addr, c_screen_size.width(), c_screen_size.height(), 32, QImage::Format_MonoLSB);
}

void Screen::render()
{
    QPainter painter(&m_screen);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::gray);
    while (true)
    {
        if (Q_LIKELY(m_renderingEnabled))
        {
            std::this_thread::sleep_for(std::chrono::duration<double>(1/120.f));
            emit VBlankMiddle();
            std::this_thread::sleep_for(std::chrono::duration<double>(1/120.f));
            painter.drawPixmap(m_screen.rect(), QPixmap::fromImage(m_image).
                               transformed(QTransform().rotate(-90)));
            emit VBlank();
            update();
        }
        if (Q_UNLIKELY(m_killRendering))
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
    if (m_drawBackground)
    {
        painter.setCompositionMode(QPainter::CompositionMode_Screen);
        painter.drawImage(event->rect(), m_background.transformed(QTransform().rotate(-90)));
    }
    if (m_pauseOpacity > 0)
    {
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        painter.setOpacity(m_pauseOpacity);
        painter.drawImage(event->rect().center() - QPointF(m_pauseIcon.size().width(),
                                                             m_pauseIcon.size().height()) / 2, m_pauseIcon);
    }
}
