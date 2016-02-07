/*  mainwindow.hpp MainWindow - Yann BOUCHER (yann) 24/01/2016
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
#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QFileInfoList>
#include <QTextStream>
#include <QKeyEvent>
#include <QMap>

#include "cpu.hpp"
#include  "screen.hpp"

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);

    public:

    signals:
        void keyPressed(int key) const;
        void keyReleased(int key) const;

    protected:
        virtual void keyPressEvent(QKeyEvent *) override;
        virtual void keyReleaseEvent(QKeyEvent *) override;
        void changeEvent(QEvent *e) override;

    private:
        QMap<int, bool> m_activeKeys;
        i8080::CPU m_cpu;
};

#endif // MAINWINDOW_HPP
