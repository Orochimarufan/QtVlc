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

#include "VlcMediaListPlayer_p.h"

#include <QtVlc/VlcMedia.h>

VLC_WRAPPER_IMPL_CPP2(VlcMediaListPlayerPrivate, libvlc_media_list_player_t,
                      libvlc_media_list_player, attach_events, detach_events)

VlcMediaListPlayerPrivate::VlcMediaListPlayerPrivate(libvlc_instance_t *inst) :
    VlcWrapperImpl()
{
    d = libvlc_media_list_player_new(inst);

    VLC_WRAPPER_IMPL_INIT();

    attach_events();
}

void VlcMediaListPlayerPrivate::attach_events()
{
#define ATTACH(event_type) libvlc_event_attach(p_eventmanager, event_type, &VlcMediaListPlayerPrivate::event_cb, this)
    ATTACH(libvlc_MediaListPlayerPlayed);
    ATTACH(libvlc_MediaListPlayerNextItemSet);
    ATTACH(libvlc_MediaListPlayerStopped);
#undef ATTACH
}

void VlcMediaListPlayerPrivate::detach_events()
{
#define DETACH(event_type) libvlc_event_detach(p_eventmanager, event_type, &VlcMediaListPlayerPrivate::event_cb, this)
    DETACH(libvlc_MediaListPlayerPlayed);
    DETACH(libvlc_MediaListPlayerNextItemSet);
    DETACH(libvlc_MediaListPlayerStopped);
#undef DETACH
}

void VlcMediaListPlayerPrivate::event_cb(const libvlc_event_t *e, void *o)
{
    VlcMediaListPlayerPrivate *p = static_cast<VlcMediaListPlayerPrivate *>(o);

    emit p->libvlcEvent(e);

    switch(e->type)
    {
    case libvlc_MediaListPlayerPlayed:
        emit p->played();
        break;
    case libvlc_MediaListPlayerNextItemSet:
        emit p->nextItemSet(e->u.media_list_player_next_item_set.item);
        emit p->nextItemSet(VlcMedia(e->u.media_list_player_next_item_set.item));
        break;
    case libvlc_MediaListPlayerStopped:
        emit p->stopped();
        break;
    default:
        qDebug("VlcMediaListPlayer: unknown Event: %i (%s)", e->type, libvlc_event_type_name(e->type));
    }
}

void VlcMediaListPlayerPrivate::setMediaPlayer(libvlc_media_player_t *p_mi)
{
    D libvlc_media_list_player_set_media_player(d, p_mi);
}

void VlcMediaListPlayerPrivate::setMediaList(libvlc_media_list_t *p_ml)
{
    D libvlc_media_list_player_set_media_list(d, p_ml);
}

bool VlcMediaListPlayerPrivate::isPlaying()
{
    D return libvlc_media_list_player_is_playing(d);
    else return false;
}

VlcState::Type VlcMediaListPlayerPrivate::state()
{
    D return static_cast<VlcState::Type>(libvlc_media_list_player_get_state(d));
    else return VlcState::Error;
}

void VlcMediaListPlayerPrivate::play()
{
    D libvlc_media_list_player_play(d);
}

void VlcMediaListPlayerPrivate::playItem(libvlc_media_t *p_md)
{
    D libvlc_media_list_player_play_item(d, p_md);
}

void VlcMediaListPlayerPrivate::playItemAtIndex(int index)
{
    D libvlc_media_list_player_play_item_at_index(d, index);
}

void VlcMediaListPlayerPrivate::pause()
{
    D libvlc_media_list_player_pause(d);
}

void VlcMediaListPlayerPrivate::stop()
{
    D libvlc_media_list_player_stop(d);
}

void VlcMediaListPlayerPrivate::next()
{
    D libvlc_media_list_player_next(d);
}

void VlcMediaListPlayerPrivate::previous()
{
    D libvlc_media_list_player_previous(d);
}

void VlcMediaListPlayerPrivate::setPlaybackMode(const VlcPlaybackMode::Type &m)
{
    D libvlc_media_list_player_set_playback_mode(d, static_cast<libvlc_playback_mode_t>(m));
}
