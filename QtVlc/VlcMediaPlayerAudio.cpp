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


VlcMediaPlayerAudio::VlcMediaPlayerAudio(VlcMediaPlayer *player) :
    QObject(player), player(player)
{
}

VlcMediaPlayerAudio::~VlcMediaPlayerAudio()
{
}

int VlcMediaPlayerAudio::volume() const
{
    if (player->data())
        return libvlc_audio_get_volume(player->data());
    else
        return -1;
}

bool VlcMediaPlayerAudio::isMuted() const
{
    if (player->data())
        return libvlc_audio_get_mute(player->data());
    else
        return false;
}

void VlcMediaPlayerAudio::setTrack(const int &track)
{
    if (player->data())
        libvlc_audio_set_track(player->data(), track);
}

int VlcMediaPlayerAudio::track() const
{
    if (player->data())
        return libvlc_audio_get_track(player->data());
    else
        return -1;
}

int VlcMediaPlayerAudio::trackCount() const
{
    if (player->data())
        return libvlc_audio_get_track_count(player->data());
    else
        return -1;
}

QHash<int, QString> VlcMediaPlayerAudio::trackDescription() const
{
    QHash<int, QString> descriptions;

    if (player->data())
    {
        libvlc_track_description_t *desc = libvlc_audio_get_track_description(player->data());

        descriptions[desc->i_id] = QString::fromUtf8(desc->psz_name);
        for (int i = 1; i < trackCount(); i++)
        {
            desc = desc->p_next;
            descriptions[desc->i_id] = QString::fromUtf8(desc->psz_name);
        }
    }

    return descriptions;
}

void VlcMediaPlayerAudio::setVolume(const int &volume)
{
    if (player->data())
        libvlc_audio_set_volume(player->data(), volume);
}

void VlcMediaPlayerAudio::setMuted(const bool &mute)
{
    if (player->data())
        libvlc_audio_set_mute(player->data(), mute);
}

void VlcMediaPlayerAudio::toggleMuted()
{
    if (player->data())
        libvlc_audio_toggle_mute(player->data());
}
