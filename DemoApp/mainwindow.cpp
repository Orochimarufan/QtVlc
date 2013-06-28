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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

    audio = player.audio();

    connect(ui->stopButton, SIGNAL(clicked()), &player, SLOT(stop()));

    connect(&player, SIGNAL(positionChanged(float)), SLOT(onPositionChanged(float)));
    connect(&player, SIGNAL(stateChanged(VlcState::Type)), SLOT(onStateChanged(VlcState::Type)));
    connect(&player, SIGNAL(endReached()), &player, SLOT(stop()));
    connect(&player, SIGNAL(mediaChanged(libvlc_media_t*)), SLOT(onMediaChanged(libvlc_media_t*)));

    player.setVideoDelegate(ui->video);
}

MainWindow::~MainWindow()
{
    ui->video->release();
    delete ui;
}

// UI slots
void MainWindow::on_actionOpen_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, "Open Video");
    if (path.isNull()) return;
    media = VlcMedia(path, true);
    player.open(media);
}

void MainWindow::on_position_sliderMoved(int new_value)
{
    player.setPosition(((float) new_value) / 1000.0);
}

void MainWindow::on_volume_volumeChanged(int i)
{
    audio.setVolume(i);
}

void MainWindow::on_volume_muteChanged(bool m)
{
    audio.setMuted(m);
}

void MainWindow::on_playButton_clicked()
{
    switch(player.state()) {
    case VlcState::Playing:
        player.pause();
        break;
    case VlcState::Paused:
        player.resume();
        break;
    case VlcState::NothingSpecial:
        on_actionOpen_triggered();
        break;
    case VlcState::Stopped:
        player.play();
    }
}

// player slots
void MainWindow::onStateChanged(const VlcState::Type &new_state)
{
    if (new_state == VlcState::Stopped)
    {
        ui->position->setPosition(-1, 0, 0);
        ui->timeLabel->setDisplayPosition(-1, 0, 0);
    }
    ui->playButton->updateButtonIcons(new_state == VlcState::Playing ||
                                      new_state == VlcState::Buffering ||
                                      new_state == VlcState::Opening);
}

void MainWindow::onPositionChanged(const float &pos)
{
    //ui->position->setValue((int)(pos * 1000.0));
    auto time = player.time();
    auto length = player.length();
    ui->position->setPosition(pos, time, length);
    ui->timeLabel->setDisplayPosition(pos, time, length);
    // TODO: find a better way to do this
    ui->volume->updateVolume(audio.volume());
    ui->volume->setMuted(audio.isMuted());
}

void MainWindow::onMediaChanged(libvlc_media_t *m)
{
    VlcMedia media = m;
    if (!media.isParsed()) media.parse(false);
    setWindowTitle(QStringLiteral("%1 - QtVlc Demo").arg(media.meta(VlcMeta::Title)));
}
