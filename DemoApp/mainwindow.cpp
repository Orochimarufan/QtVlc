/*****************************************************************************
 * QtVlc - C++ bindings for libVLC using Qt awesomeness
 * Copyright (C) 2013 Orochimarufan <orochimarufan.x3@gmail.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets/QFileDialog>

#include <QtVlc/VlcMediaPlayerAudio.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

    inst = new VlcInstance();
    player = new VlcMediaPlayer(*inst);
    media = nullptr;

    connect(ui->btn_play, SIGNAL(clicked()), player, SLOT(play()));
    connect(ui->btn_pause, SIGNAL(clicked()), player, SLOT(pause()));
    connect(ui->btn_resume, SIGNAL(clicked()), player, SLOT(resume()));
    connect(ui->btn_toggle, SIGNAL(clicked()), player, SLOT(togglePause()));
    connect(ui->btn_stop, SIGNAL(clicked()), player, SLOT(stop()));
    connect(player, SIGNAL(positionChanged(float)), SLOT(setPosition(float)));

    player->setVideoDelegate(ui->video);

    ui->volume->setValue(player->audio().volume());
}

MainWindow::~MainWindow()
{
    if (media)
        delete media;
    delete player;
    delete inst;
    ui->video->release();
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, "Open Video");
    if (path.isNull()) return;
    if (media) media->deleteLater();
    media = new VlcMedia(*inst, path, true);
    player->open(*media);
}

void MainWindow::on_position_sliderMoved(int new_value)
{
    player->setPosition(((float) new_value) / 1000.0);
}

void MainWindow::setPosition(const float &pos)
{
    //ui->position->setValue((int)(pos * 1000.0));
    auto time = player->time();
    auto length = player->length();
    ui->position->setPosition(pos, time, length);
    ui->timeLabel->setDisplayPosition(pos, time, length);
}

void MainWindow::on_volume_sliderMoved(int i)
{
    player->audio().setVolume(i);
}
