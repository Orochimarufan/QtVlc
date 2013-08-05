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

#ifndef QTVLC_VLCMEDIALISTPLAYER_P_H
#define QTVLC_VLCMEDIALISTPLAYER_P_H

#include <vlc/vlc.h>

#include "VlcEnum.h"

#include "common_p.h"

class VlcMedia;

class VlcMediaListPlayerPrivate : public VlcWrapperImpl
{
    Q_OBJECT
    VLC_WRAPPER_IMPL_PRIVATE(VlcMediaListPlayerPrivate, libvlc_media_list_player_t)

public:
    VLC_WRAPPER_IMPL_PUBLIC(VlcMediaListPlayerPrivate, libvlc_media_list_player_t)

    explicit VlcMediaListPlayerPrivate(libvlc_instance_t *);

    void setMediaPlayer(libvlc_media_player_t *);
    void setMediaList(libvlc_media_list_t *);

    bool isPlaying();
    VlcState::Type state();

    void play();
    void playItem(libvlc_media_t *);
    void playItemAtIndex(int);

    void pause();
    void stop();

    void next();
    void previous();

    void setPlaybackMode(const VlcPlaybackMode::Type &);

signals:
    void libvlcEvent(const libvlc_event_t *);
    void played();
    void nextItemSet(libvlc_media_t *item);
    void nextItemSet(const VlcMedia &);
    void stopped();

private:
    libvlc_event_manager_t *p_eventmanager;
    void attach_events();
    void detach_events();
    static void event_cb(const libvlc_event_t *, void *);
};

#endif // QTVLC_VLCMEDIALISTPLAYER_P_H
