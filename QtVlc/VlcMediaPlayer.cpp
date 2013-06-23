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

#include <QtCore/QObject>

#include <vlc/vlc.h>

#include <QtVlc/VlcMediaPlayer.h>
#include <QtVlc/VlcMediaPlayerAudio.h>
#include <QtVlc/VlcMediaPlayerVideo.h>

QHash<libvlc_media_player_t *, QWeakPointer<VlcMediaPlayer>> VlcMediaPlayer::instances;

// instance management
VlcMediaPlayer::VlcMediaPlayer(libvlc_instance_t *instance) :
    QObject(), _widget(0), _delegate(nullptr)
{
    _player = libvlc_media_player_new(instance);

    _audio = new VlcMediaPlayerAudio(this);
    _video = new VlcMediaPlayerVideo(this);

    init_events();
}

VlcMediaPlayer::VlcMediaPlayer(libvlc_media_player_t *player) :
    QObject(), _player(player), _delegate(nullptr)
{
    libvlc_media_player_retain(_player);

    _widget = videoWidget();

    init_events();
}

void VlcMediaPlayer::deletePtr(VlcMediaPlayer *ptr)
{
    if (instances.contains(ptr->data()))
        instances.remove(ptr->data());
    delete ptr;
}

// destructor
VlcMediaPlayer::~VlcMediaPlayer()
{
    kill_events();

    libvlc_media_player_release(_player);
}


// media
libvlc_media_t *VlcMediaPlayer::media()
{
    return libvlc_media_player_get_media(_player);
}

void VlcMediaPlayer::setMedia(libvlc_media_t *media)
{
    libvlc_media_player_set_media(_player, media);
}

// widget
WId VlcMediaPlayer::videoWidget()
{
#if defined(Q_OS_WIN32)
    return libvlc_media_player_get_hwnd(_player);
#elif defined(Q_OS_MAC)
    return libvlc_media_player_get_nsobject(_player);
#elif defined(Q_OS_UNIX)
    return libvlc_media_player_get_xwindow(_player);
#else
    qDebug("Uknown platform!!!");
    return 0;
#endif
}

void VlcMediaPlayer::setVideoWidget(WId widget)
{
    if (_delegate == nullptr)
        _widget = widget;
}

inline void VlcMediaPlayer::_setVideoWidget(WId widget)
{
#if defined(Q_OS_WIN32)
    libvlc_media_player_set_hwnd(_player, widget);
#elif defined(Q_OS_MAC)
    libvlc_media_player_set_nsobject(_player, widget);
#elif defined(Q_OS_UNIX)
    libvlc_media_player_set_xwindow(_player, widget);
#else
    qDebug("Unknown Platform!!!");
#endif
}

// position
qint64 VlcMediaPlayer::length() const
{
    return libvlc_media_player_get_length(_player);
}

qint64 VlcMediaPlayer::time() const
{
    return libvlc_media_player_get_time(_player);
}

void VlcMediaPlayer::setTime(const qint64 &time)
{
    libvlc_media_player_set_time(_player, time);
}

float VlcMediaPlayer::position() const
{
    return libvlc_media_player_get_position(_player);
}

void VlcMediaPlayer::setPosition(const float &pos)
{
    libvlc_media_player_set_position(_player, pos);
}


// events
void VlcMediaPlayer::libvlc_event_cb(const libvlc_event_t *e, void *o)
{
    QObject *object = static_cast<QObject *>(o);
    VlcMediaPlayer *player = qobject_cast<VlcMediaPlayer *>(object);
    if (player)
        player->libvlc_event(e);
}

inline void VlcMediaPlayer::libvlc_event(const libvlc_event_t *e)
{
    emit libvlcEvent(e);

    switch(e->type)
    {
    case libvlc_MediaPlayerMediaChanged:
        emit mediaChanged(e->u.media_player_media_changed.new_media);
        break;
    case libvlc_MediaPlayerNothingSpecial:
        emit stateChanged(VlcState::NothingSpecial);
        break;
    case libvlc_MediaPlayerOpening:
        emit stateChanged(VlcState::Opening);
        break;
    case libvlc_MediaPlayerBuffering:
        emit stateChanged(VlcState::Buffering);
        emit buffering(e->u.media_player_buffering.new_cache);
        break;
    case libvlc_MediaPlayerPlaying:
        emit stateChanged(VlcState::Playing);
        break;
    case libvlc_MediaPlayerPaused:
        emit stateChanged(VlcState::Paused);
        break;
    case libvlc_MediaPlayerStopped:
        emit stateChanged(VlcState::Stopped);
        break;
    case libvlc_MediaPlayerForward:
        emit forward();
        break;
    case libvlc_MediaPlayerBackward:
        emit backward();
        break;
    case libvlc_MediaPlayerEndReached:
        emit endReached();
        break;
    case libvlc_MediaPlayerEncounteredError:
        emit encounteredError();
        break;
    case libvlc_MediaPlayerTimeChanged:
        emit timeChanged(e->u.media_player_time_changed.new_time);
        break;
    case libvlc_MediaPlayerPositionChanged:
        emit positionChanged(e->u.media_player_position_changed.new_position);
        break;
    case libvlc_MediaPlayerSeekableChanged:
        emit seekableChanged(e->u.media_player_seekable_changed.new_seekable);
        break;
    case libvlc_MediaPlayerPausableChanged:
        emit pausableChanged(e->u.media_player_pausable_changed.new_pausable);
        break;
    case libvlc_MediaPlayerTitleChanged:
        emit titleChanged(e->u.media_player_title_changed.new_title);
        break;
    case libvlc_MediaPlayerSnapshotTaken:
        emit snapshotTaken(e->u.media_player_snapshot_taken.psz_filename);
        break;
    case libvlc_MediaPlayerLengthChanged:
        emit lengthChanged(e->u.media_player_length_changed.new_length);
        break;
    case libvlc_MediaPlayerVout:
        emit voutChanged(e->u.media_player_vout.new_count);
        break;
    default:
        qDebug("VlcMediaPlayer: unknown event: %i", e->type);
    }
}

inline void VlcMediaPlayer::init_events()
{
    _evm = libvlc_media_player_event_manager(_player);

#define ATTACH(event_type) libvlc_event_attach(_evm, event_type, &VlcMediaPlayer::libvlc_event_cb, this)
    ATTACH(libvlc_MediaPlayerMediaChanged);
    ATTACH(libvlc_MediaPlayerNothingSpecial);
    ATTACH(libvlc_MediaPlayerOpening);
    ATTACH(libvlc_MediaPlayerBuffering);
    ATTACH(libvlc_MediaPlayerPlaying);
    ATTACH(libvlc_MediaPlayerPaused);
    ATTACH(libvlc_MediaPlayerStopped);
    ATTACH(libvlc_MediaPlayerForward);
    ATTACH(libvlc_MediaPlayerBackward);
    ATTACH(libvlc_MediaPlayerEndReached);
    ATTACH(libvlc_MediaPlayerEncounteredError);
    ATTACH(libvlc_MediaPlayerTimeChanged);
    ATTACH(libvlc_MediaPlayerPositionChanged);
    ATTACH(libvlc_MediaPlayerSeekableChanged);
    ATTACH(libvlc_MediaPlayerPausableChanged);
    ATTACH(libvlc_MediaPlayerTitleChanged);
    ATTACH(libvlc_MediaPlayerSnapshotTaken);
    ATTACH(libvlc_MediaPlayerLengthChanged);
    ATTACH(libvlc_MediaPlayerVout);
#undef ATTACH
}

inline void VlcMediaPlayer::kill_events()
{
#define DETACH(event_type) libvlc_event_detach(_evm, event_type, &VlcMediaPlayer::libvlc_event_cb, this)
    DETACH(libvlc_MediaPlayerMediaChanged);
    DETACH(libvlc_MediaPlayerNothingSpecial);
    DETACH(libvlc_MediaPlayerOpening);
    DETACH(libvlc_MediaPlayerBuffering);
    DETACH(libvlc_MediaPlayerPlaying);
    DETACH(libvlc_MediaPlayerPaused);
    DETACH(libvlc_MediaPlayerStopped);
    DETACH(libvlc_MediaPlayerForward);
    DETACH(libvlc_MediaPlayerBackward);
    DETACH(libvlc_MediaPlayerEndReached);
    DETACH(libvlc_MediaPlayerEncounteredError);
    DETACH(libvlc_MediaPlayerTimeChanged);
    DETACH(libvlc_MediaPlayerPositionChanged);
    DETACH(libvlc_MediaPlayerSeekableChanged);
    DETACH(libvlc_MediaPlayerPausableChanged);
    DETACH(libvlc_MediaPlayerTitleChanged);
    DETACH(libvlc_MediaPlayerSnapshotTaken);
    DETACH(libvlc_MediaPlayerLengthChanged);
    DETACH(libvlc_MediaPlayerVout);
#undef DETACH
}


// slots
void VlcMediaPlayer::play()
{
    if (!_player) return;

    if (_delegate)
        _widget = _delegate->request(); // TODO gather size

    _setVideoWidget(_widget);

    libvlc_media_player_play(_player);
}

void VlcMediaPlayer::pause()
{
    if (!_player) return;

    if (libvlc_media_player_can_pause(_player))
        libvlc_media_player_set_pause(_player, true);
}

void VlcMediaPlayer::resume()
{
    if (!_player) return;

    if (libvlc_media_player_can_pause(_player))
        libvlc_media_player_set_pause(_player, false);
}

void VlcMediaPlayer::togglePause()
{
    if (!_player) return;

    if (libvlc_media_player_can_pause(_player))
        libvlc_media_player_pause(_player);
}

void VlcMediaPlayer::stop()
{
    if (!_player) return;

    libvlc_media_player_stop(_player);

    if (_delegate)
    {
        _delegate ->release();
        _widget = 0;
        _setVideoWidget(0);
    }
}
