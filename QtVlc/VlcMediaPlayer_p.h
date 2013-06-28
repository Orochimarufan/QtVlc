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

#ifndef QTVLC_VLCMEDIAPLAYERPRIVATE_H
#define QTVLC_VLCMEDIAPLAYERPRIVATE_H

#include <vlc/vlc.h>

#include <QtGui/qwindowdefs.h>

#include <QtVlc/enum.h>
#include <QtVlc/IVlcVideoDelegate.h>

#include "common_p.h"

class VlcMediaPlayerPrivate : public VlcWrapperImpl
{
    Q_OBJECT
    VLC_WRAPPER_IMPL_PRIVATE(VlcMediaPlayerPrivate, libvlc_media_player_t)

public:
    VLC_WRAPPER_IMPL_PUBLIC(VlcMediaPlayerPrivate, libvlc_media_player_t)

    explicit VlcMediaPlayerPrivate(libvlc_instance_t *);

    libvlc_media_t *media();

    void setMedia(libvlc_media_t *);

    qint64 length() const;
    qint64 time() const;
    void setTime(const qint64 &);
    float position() const;
    void setPosition(const float &);

    void setVideoWidget(WId);
    WId videoWidget();

    void setVideoDelegate(IVlcVideoDelegate *);
    IVlcVideoDelegate *videoDelegate();

    void play();
    void pause();
    void setPause(bool);
    void stop();

    bool isPausable() const;
    bool isSeekable() const;

    VlcState::Type state();

    // Audio
    int audio_volume() const;
    void audio_setVolume(const int &);

    bool audio_isMuted() const;
    void audio_setMuted(const int &);
    void audio_toggleMuted();

    int audio_track() const;
    int audio_trackCount() const;
    QHash<int, QString> audio_trackDescription() const;
    void audio_setTrack(const int &);

private:
    libvlc_event_manager_t *p_eventmanager;
    void attach_events();
    void detach_events();
    static void event_cb(const libvlc_event_t *, void *);

    IVlcVideoDelegate *p_delegate;
    WId widget;
    void _setVideoWidget(WId);

signals:
    void libvlcEvent(const libvlc_event_t *);
    void mediaChanged(libvlc_media_t *);
    void buffering(const float &);
    void stateChanged(const VlcState::Type &);
    void forward();
    void backward();
    void endReached();
    void encounteredError();
    void timeChanged(const qint64 &);
    void positionChanged(const float &);
    void seekableChanged(const int &);
    void pausableChanged(const int &);
    void titleChanged(const int &);
    void snapshotTaken(const QString &);
    void lengthChanged(const qint64 &);
    void voutChanged(const int &);
};

#endif
