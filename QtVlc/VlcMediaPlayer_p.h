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

#include "VlcEnum.h"
#include "IVlcVideoDelegate.h"

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

    // video
    void video_setKeyInput(bool on);
    void video_setMouseInput(bool on);

    bool video_getSize(unsigned num, unsigned *x, unsigned *y) const;
    bool video_getCursor(unsigned num, int *x, int *y) const;

    float video_scale() const;
    void video_setScale(float factor);

    QString video_aspectRatio() const;
    void video_setAspectRatio(const QString &ratio);

    int video_spu() const;
    int video_spuCount() const;
    QHash<int, QString> video_spuDescription() const;
    bool video_setSpu(int spu);
    bool video_setSubtitleFile(const QString &filename);
    qint64 video_spuDelay() const;
    bool video_setSpuDelay(qint64 delay);

    QHash<int, QString> video_titleDescription() const;
    QHash<int, QString> video_chapterDescription(int title) const;

    QString video_cropGeometry() const;
    void video_setCropGeometry(const QString &geometry);

    int video_teletext() const;
    void video_setTeletext(int page);
    void video_toggleTeletext();

    int video_trackCount() const;
    int video_track() const;
    QHash<int, QString> video_trackDescription() const;
    bool video_setTrack(int track);

    bool video_takeSnapshot(unsigned num, const QString &filename, unsigned int width, unsigned int height) const;

    void video_setDeinterlace(const QString &mode);

    int video_marqueeInt(unsigned option) const;
    QString video_marqueeString(unsigned option) const;
    void video_setMarqueeInt(unsigned option, int value);
    void video_setMarqueeString(unsigned option, const QString &text);

    int video_logoInt(unsigned option) const;
    void video_setLogoInt(unsigned option, int value);
    void video_setLogoString(unsigned option, const QString &value);

    int video_adjustInt(unsigned option) const;
    float video_adjustFloat(unsigned option) const;
    void video_setAdjustInt(unsigned option, int value);
    void video_setAdjustFloat(unsigned option, float value);

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
