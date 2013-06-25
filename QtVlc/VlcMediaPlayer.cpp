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

#include <QtVlc/VlcMediaPlayer.h>
#include <QtVlc/VlcInstance.h>
#include <QtVlc/VlcMedia.h>
#include <QtVlc/VlcMediaPlayerAudio.h>

#include "VlcMediaPlayer_p.h"

void VlcMediaPlayer::d_connect()
{
#define SIG(signal) connect(d, signal, signal)
    SIG(SIGNAL(libvlcEvent(const libvlc_event_t *)));
    SIG(SIGNAL(mediaChanged(libvlc_media_t *)));
    SIG(SIGNAL(buffering(const float &)));
    SIG(SIGNAL(stateChanged(const VlcState::Type &)));
    SIG(SIGNAL(forward()));
    SIG(SIGNAL(backward()));
    SIG(SIGNAL(endReached()));
    SIG(SIGNAL(encounteredError()));
    SIG(SIGNAL(timeChanged(const qint64 &)));
    SIG(SIGNAL(positionChanged(const float &)));
    SIG(SIGNAL(seekableChanged(const int &)));
    SIG(SIGNAL(pausableChanged(const int &)));
    SIG(SIGNAL(titleChanged(const int &)));
    SIG(SIGNAL(snapshotTaken(const QString &)));
    SIG(SIGNAL(lengthChanged(const qint64 &)));
    SIG(SIGNAL(voutChanged(const int &)));
#undef SIG
}

VlcMediaPlayer::VlcMediaPlayer(const VlcMediaPlayer &o) :
    QObject(), d(o.d)
{
    d->retain();
    d_connect();
}

VlcMediaPlayer::VlcMediaPlayer(const VlcInstance &instance) :
    QObject()
{
    d = new VlcMediaPlayerPrivate(getref<VlcInstance>(instance)->data());
    d_connect();
}

VlcMediaPlayer::VlcMediaPlayer(libvlc_instance_t *instance) :
    QObject()
{
    d = new VlcMediaPlayerPrivate(instance);
    d_connect();
}

VlcMediaPlayer::VlcMediaPlayer(libvlc_media_player_t *player) :
    QObject()
{
    d = VlcMediaPlayerPrivate::instance(player);
    d_connect();
}

VlcMediaPlayer::~VlcMediaPlayer()
{
    d->release();
}

libvlc_media_player_t *VlcMediaPlayer::data()
{
    return d->data();
}

void VlcMediaPlayer::setMedia(libvlc_media_t *media)
{
    d->setMedia(media);
}

VlcMedia VlcMediaPlayer::media()
{
    return VlcMedia(d->media());
}

libvlc_media_t *VlcMediaPlayer::media_()
{
    return d->media();
}

void VlcMediaPlayer::open(libvlc_media_t *media)
{
    d->setMedia(media);
    d->play();
}

void VlcMediaPlayer::open(const VlcMedia &media)
{
    d->setMedia(getref<VlcMedia>(media)->data());
    d->play();
}

qint64 VlcMediaPlayer::length() const
{
    return d->length();
}

qint64 VlcMediaPlayer::time() const
{
    return d->time();
}

void VlcMediaPlayer::setTime(const qint64 &time)
{
    d->setTime(time);
}

float VlcMediaPlayer::position() const
{
    return d->position();
}

void VlcMediaPlayer::setPosition(const float &position)
{
    d->setPosition(position);
}

void VlcMediaPlayer::setVideoWidget(WId widget)
{
    d->setVideoWidget(widget);
}

void VlcMediaPlayer::setVideoDelegate(IVlcVideoDelegate *delegate)
{
    d->setVideoDelegate(delegate);
}

WId VlcMediaPlayer::videoWidget()
{
    return d->videoWidget();
}

IVlcVideoDelegate *VlcMediaPlayer::videoDelegate()
{
    return d->videoDelegate();
}

void VlcMediaPlayer::play()
{
    d->play();
}

void VlcMediaPlayer::pause()
{
    d->setPause(true);
}

void VlcMediaPlayer::resume()
{
    d->setPause(false);
}

void VlcMediaPlayer::togglePause()
{
    d->pause();
}

void VlcMediaPlayer::stop()
{
    d->stop();
}

VlcMediaPlayerAudio VlcMediaPlayer::audio()
{
    return VlcMediaPlayerAudio(d);
}
