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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), m_resetScore(tr(R"a(   <a href="reset" style="color:#55C1E6">Reset score</a>)a"))
{
    setupUi(this);

    m_board.setupScreen(*screen);
    m_board.setInputCheckCallback([this]
    {
        m_board.credit = m_activeKeys[Qt::Key_C];
        m_board.tilt = m_activeKeys[Qt::Key_T];

        m_board.p1Start = m_activeKeys[Qt::Key_S];
        m_board.p1Shoot = m_activeKeys[Qt::Key_Z];
        m_board.p1Left = m_activeKeys[Qt::Key_Q];
        m_board.p1Right = m_activeKeys[Qt::Key_D];

        m_board.p2Start = m_activeKeys[Qt::Key_K];
        m_board.p2Shoot = m_activeKeys[Qt::Key_I];
        m_board.p2Left = m_activeKeys[Qt::Key_J];
        m_board.p2Right = m_activeKeys[Qt::Key_L];

        m_board.dip_coinInfo = !coinInfo->isChecked();
        m_board.dip_extraShipScoreBonus = bonusLife->value() != bonusLife->maximum();
        m_board.dip_shipNumberb0 = (lives->value() - 3) & 1;
        m_board.dip_shipNumberb1 = ((lives->value() - 3) & 2) >> 1;
    });

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
            case Qt::Key_P:
                if (m_activeKeys[key])
                {
                    pauseButton->setChecked(!pauseButton->isChecked());
                }
                break;
        }
    };

    connect(this, &MainWindow::keyPressed, keyCheck);
    connect(this, &MainWindow::keyReleased, keyCheck);

    connect(volume, &QDial::valueChanged, [this](int val)
    {
        m_board.setVolume(val / 100.f);
    });
    connect(audioBox, &QGroupBox::toggled, [this](bool val)
    {
        m_board.enableSound(val);
    });

    connect(pauseButton, &QPushButton::toggled, [this](bool checked)
    {
        pauseButton->setText(checked ? tr("Resume") : tr("Pause"));
        m_board.setPaused(checked);
        screen->setPaused(checked);
    });

    connect(scaleButton, &QPushButton::toggled, [this](bool checked)
    {
        scaleButton->setText(checked ? tr("Normal") : tr("x2"));
        screen->setScale(checked ? 2 : 0.5);
        QTimer::singleShot(50, [this]
        {
            resize(0, 0);
        });
    });

    connect(resetButton, &QPushButton::clicked, [this]
    {
        m_board.reset();
        m_board.start();
    });

    m_hiscoreLabel.setText("0");

    QMainWindow::statusBar()->addWidget(new QLabel(tr("Overall highscore : "), QMainWindow::statusBar()));
    QMainWindow::statusBar()->addWidget(&m_hiscoreLabel);
    m_resetScore.setOpenExternalLinks(false);
    connect(&m_resetScore, &QLabel::linkActivated, [this]
    {
        m_board.resetHighScore();
        m_hiScore = 0;
        saveSettings();
    });
    QMainWindow::statusBar()->addWidget(&m_resetScore);
    QMainWindow::statusBar()->addWidget(&m_ipsLabel);

    m_scoreUpdate.setInterval(500);
    connect(&m_scoreUpdate, &QTimer::timeout, [this]
    {
        if (m_board.highScore() > m_hiScore)
        {
            m_hiScore = m_board.highScore();
        }
        m_hiscoreLabel.setText(QString::number(m_hiScore, 16));
        m_ipsLabel.setText("    IPS : " + QString::number(m_board.ips()));
    });

    loadSettings();

    m_scoreUpdate.start();

    m_board.start();
}

void MainWindow::closeEvent(QCloseEvent * e)
{
    saveSettings();
    e->accept();
}

void MainWindow::loadSettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    colorOverlay->setChecked(settings.value("screen/color", true).toBool());
    screen->setColorOverlay(colorOverlay->isChecked());
    bkgd_enable->setChecked(settings.value("screen/bkgd", false).toBool());
    screen->setBackgroundImage(bkgd_enable->isChecked());

    lives->setValue(settings.value("dip/lives", 3).toInt());
    bonusLife->setValue(settings.value("dip/extraShipAt", 1000).toInt());
    coinInfo->setChecked(settings.value("dip/coinInfo", true).toBool());

    audioBox->setChecked(settings.value("audio/enabled", true).toBool());
    volume->setValue(settings.value("audio/volume", 80).toInt());

    m_hiScore = settings.value("game/highScore", 0).toUInt();
}

void MainWindow::saveSettings() const
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setValue("screen/color", colorOverlay->isChecked());
    settings.setValue("screen/bkgd", bkgd_enable->isChecked());

    settings.setValue("dip/lives", lives->value());
    settings.setValue("dip/extraShipAt", bonusLife->value());
    settings.setValue("dip/coinInfo", coinInfo->isChecked());

    settings.setValue("audio/enabled", audioBox->isChecked());
    settings.setValue("audio/volume", volume->value());

    settings.setValue("game/highScore", m_hiScore);
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
