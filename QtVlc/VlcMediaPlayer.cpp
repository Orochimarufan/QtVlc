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
#include <QtVlc/error.h>

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

VlcMediaPlayer::VlcMediaPlayer() :
    QObject(), d(nullptr)
{
}

bool VlcMediaPlayer::isValid()
{
    return d != nullptr;
}

VlcMediaPlayer::VlcMediaPlayer(const VlcMediaPlayer &o) :
    QObject(), d(o.d)
{
    if (d != nullptr)
    {
        d->retain();
        d_connect();
    }
}

VlcMediaPlayer &VlcMediaPlayer::operator =(const VlcMediaPlayer &o)
{
    if (d != nullptr)
    {
        disconnect(d);
        d->release();
    }

    d = o.d;

    if (d != nullptr)
    {
        d->retain();
        d_connect();
    }

    return *this;
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

    if (d != nullptr)
        d_connect();
}

VlcMediaPlayer &VlcMediaPlayer::operator =(libvlc_media_player_t *player)
{
    if (d != nullptr)
    {
        disconnect(d);
        d->release();
    }

    d = VlcMediaPlayerPrivate::instance(player);

    if (d != nullptr)
        d_connect();

    return *this;
}

VlcMediaPlayer::~VlcMediaPlayer()
{
    if (d != nullptr)
        d->release();
}

libvlc_media_player_t *VlcMediaPlayer::data()
{
    CHECKNP
    return d->data();
}

void VlcMediaPlayer::setMedia(libvlc_media_t *media)
{
    CHECKNP
    d->setMedia(media);
}

VlcMedia VlcMediaPlayer::media()
{
    CHECKNP
    return VlcMedia(d->media());
}

libvlc_media_t *VlcMediaPlayer::media_()
{
    CHECKNP
    return d->media();
}

void VlcMediaPlayer::open(libvlc_media_t *media)
{
    CHECKNP
    d->setMedia(media);
    d->play();
}

void VlcMediaPlayer::open(const VlcMedia &media)
{
    CHECKNP
    d->setMedia(getref<VlcMedia>(media)->data());
    d->play();
}

qint64 VlcMediaPlayer::length() const
{
    CHECKNP
    return d->length();
}

qint64 VlcMediaPlayer::time() const
{
    CHECKNP
    return d->time();
}

void VlcMediaPlayer::setTime(const qint64 &time)
{
    CHECKNP
    d->setTime(time);
}

float VlcMediaPlayer::position() const
{
    CHECKNP
    return d->position();
}

void VlcMediaPlayer::setPosition(const float &position)
{
    CHECKNP
    d->setPosition(position);
}

void VlcMediaPlayer::setVideoWidget(WId widget)
{
    CHECKNP
    d->setVideoWidget(widget);
}

void VlcMediaPlayer::setVideoDelegate(IVlcVideoDelegate *delegate)
{
    CHECKNP
    d->setVideoDelegate(delegate);
}

WId VlcMediaPlayer::videoWidget()
{
    CHECKNP
    return d->videoWidget();
}

IVlcVideoDelegate *VlcMediaPlayer::videoDelegate()
{
    CHECKNP
    return d->videoDelegate();
}

void VlcMediaPlayer::play()
{
    CHECKNP
    d->play();
}

void VlcMediaPlayer::pause()
{
    CHECKNP
    d->setPause(true);
}

void VlcMediaPlayer::resume()
{
    CHECKNP
    d->setPause(false);
}

void VlcMediaPlayer::togglePause()
{
    CHECKNP
    d->pause();
}

void VlcMediaPlayer::stop()
{
    CHECKNP
    d->stop();
}
