/*  mainwindow.cpp MainWindow - Yann BOUCHER (yann) 24/01/2016
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
#include "mainwindow.hpp"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    screen->setVRAMAddr(m_cpu.state().mem.VRAM());
    screen->startRendering();

    QFile f(":qdarkstyle/style.qss");
    if (!f.exists())
    {
        qWarning("Unable to set stylesheet, file not found\n");
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }

    auto keyCheck = [this](int key)
    {
        switch (key)
        {
            case Qt::Key_T:
                tiltButton->setChecked(m_activeKeys[key]);
                break;

            case Qt::Key_C:
                coinButton->setChecked(m_activeKeys[key]);
                break;


            case Qt::Key_S:
                P1StartButton->setChecked(m_activeKeys[key]);
                break;

            case Qt::Key_Z:
                P1ShootButton->setChecked(m_activeKeys[key]);
                break;

            case Qt::Key_Q:
                P1LeftButton->setChecked(m_activeKeys[key]);
                break;

            case Qt::Key_D:
                P1RightButton->setChecked(m_activeKeys[key]);
                break;


            case Qt::Key_K:
                P2StartButton->setChecked(m_activeKeys[key]);
                break;

            case Qt::Key_I:
                P2ShootButton->setChecked(m_activeKeys[key]);
                break;

            case Qt::Key_J:
                P2LeftButton->setChecked(m_activeKeys[key]);
                break;

            case Qt::Key_L:
                P2RightButton->setChecked(m_activeKeys[key]);
                break;
        }
    };

    connect(this, &MainWindow::keyPressed, keyCheck);
    connect(this, &MainWindow::keyReleased, keyCheck);
    connect(screen, &Screen::VBlank, [this]{
        m_cpu.interrupt(0xCF); // RST 8
    });
    connect(screen, &Screen::VBlankMiddle, [this]{
        m_cpu.interrupt(0xD7); // RST 10
    });

    m_cpu.run(0);

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    m_activeKeys[event->key()] = true;
    emit keyPressed(event->key());
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    m_activeKeys[event->key()] = false;
    emit keyReleased(event->key());
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            retranslateUi(this);
            break;
        default:
            break;
    }
}
