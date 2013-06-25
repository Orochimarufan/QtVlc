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

#include "VlcMediaPlayer_p.h"


VlcMediaPlayerAudio::VlcMediaPlayerAudio(const VlcMediaPlayerAudio &o) :
    QObject(), d(o.d)
{
    d->retain();
}

VlcMediaPlayerAudio::VlcMediaPlayerAudio(const VlcMediaPlayer &player) :
    QObject()
{
    d = VlcMediaPlayerPrivate::instance(getref<VlcMediaPlayer>(player)->data());
}

VlcMediaPlayerAudio::VlcMediaPlayerAudio(libvlc_media_player_t *player) :
    QObject()
{
    d = VlcMediaPlayerPrivate::instance(player);
}

VlcMediaPlayerAudio::VlcMediaPlayerAudio(VlcMediaPlayerPrivate *p) :
    QObject(), d(p)
{
    d->retain();
}

VlcMediaPlayerAudio::~VlcMediaPlayerAudio()
{
    d->release();
}

int VlcMediaPlayerAudio::volume() const
{
    return d->audio_volume();
}

bool VlcMediaPlayerAudio::isMuted() const
{
    return d->audio_isMuted();
}

void VlcMediaPlayerAudio::setTrack(const int &track)
{
    d->audio_setTrack(track);
}

int VlcMediaPlayerAudio::track() const
{
    return d->audio_track();
}

int VlcMediaPlayerAudio::trackCount() const
{
    return d->audio_trackCount();
}

QHash<int, QString> VlcMediaPlayerAudio::trackDescription() const
{
    return d->audio_trackDescription();
}

void VlcMediaPlayerAudio::setVolume(const int &volume)
{
    d->audio_setVolume(volume);
}

void VlcMediaPlayerAudio::setMuted(const bool &mute)
{
    d->audio_setMuted(mute);
}

void VlcMediaPlayerAudio::toggleMuted()
{
    d->audio_toggleMuted();
}
