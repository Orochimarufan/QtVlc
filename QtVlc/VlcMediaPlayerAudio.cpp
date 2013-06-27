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

#include <vlc/vlc.h>

#include <QtVlc/VlcMediaPlayerAudio.h>
#include <QtVlc/VlcMediaPlayer.h>
#include <QtVlc/error.h>

#include "VlcMediaPlayer_p.h"

// validity
bool VlcMediaPlayerAudio::isValid()
{
    return d != nullptr;
}

// NULL constructor
VlcMediaPlayerAudio::VlcMediaPlayerAudio() :
    QObject(), d(nullptr)
{
}

// constructor/assign
VlcMediaPlayerAudio::VlcMediaPlayerAudio(const VlcMediaPlayerAudio &o) :
    QObject(), d(o.d)
{
    if (d != nullptr)
        d->retain();
}

VlcMediaPlayerAudio &VlcMediaPlayerAudio::operator =(const VlcMediaPlayerAudio &o)
{
    if (d != nullptr)
        d->release();

    d = o.d;

    if (d != nullptr)
        d->retain();

    return *this;
}

VlcMediaPlayerAudio::VlcMediaPlayerAudio(const VlcMediaPlayer &player) :
    QObject()
{
    d = VlcMediaPlayerPrivate::instance(getref<VlcMediaPlayer>(player)->data());
}

VlcMediaPlayerAudio &VlcMediaPlayerAudio::operator =(const VlcMediaPlayer &player)
{
    if (d != nullptr)
        d->release();

    d = VlcMediaPlayerPrivate::instance(getref<VlcMediaPlayer>(player)->data());

    return *this;
}

VlcMediaPlayerAudio::VlcMediaPlayerAudio(libvlc_media_player_t *player) :
    QObject()
{
    d = VlcMediaPlayerPrivate::instance(player);
}

VlcMediaPlayerAudio &VlcMediaPlayerAudio::operator =(libvlc_media_player_t *player)
{
    if (d != nullptr)
        d->release();

    d = VlcMediaPlayerPrivate::instance(player);

    return *this;
}

VlcMediaPlayerAudio::VlcMediaPlayerAudio(VlcMediaPlayerPrivate *p) :
    QObject(), d(p)
{
    if (d != nullptr)
        d->retain();
}

// destructor
VlcMediaPlayerAudio::~VlcMediaPlayerAudio()
{
    if (d != nullptr)
        d->release();
}

// volume
int VlcMediaPlayerAudio::volume() const
{
    CHECKNP
    return d->audio_volume();
}

bool VlcMediaPlayerAudio::isMuted() const
{
    CHECKNP
    return d->audio_isMuted();
}

// tracks
void VlcMediaPlayerAudio::setTrack(const int &track)
{
    CHECKNP
    d->audio_setTrack(track);
}

int VlcMediaPlayerAudio::track() const
{
    CHECKNP
    return d->audio_track();
}

int VlcMediaPlayerAudio::trackCount() const
{
    CHECKNP
    return d->audio_trackCount();
}

QHash<int, QString> VlcMediaPlayerAudio::trackDescription() const
{
    CHECKNP
    return d->audio_trackDescription();
}

// slots
void VlcMediaPlayerAudio::setVolume(const int &volume)
{
    CHECKNP
    d->audio_setVolume(volume);
}

void VlcMediaPlayerAudio::setMuted(const bool &mute)
{
    CHECKNP
    d->audio_setMuted(mute);
}

void VlcMediaPlayerAudio::toggleMuted()
{
    CHECKNP
    d->audio_toggleMuted();
}
