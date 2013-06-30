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

#ifndef QTVLC_VLCMEDIALISTPLAYER_H
#define QTVLC_VLCMEDIALISTPLAYER_H

#include <QtVlc/AbstractMediaListPlayer.h>

class VlcInstance;
struct libvlc_instance_t;
struct libvlc_media_list_player_t;
class VlcMediaListPlayerPrivate;

class VlcMediaListPlayer : public AbstractMediaListPlayer
{
    Q_OBJECT
    VlcMediaListPlayerPrivate *d;
    void d_connect();

public:
    /**
     * @brief Check if this VlcMediaListPlayer is valid
     * True if this VlcMediaListPlayer has underlying data.
     * Most methods will throw a NullPointer exception if called invalid objects.
     */
    bool isValid();

    // copy, assignment & libvlc primitive
    VlcMediaListPlayer(const VlcMediaListPlayer &);
    VlcMediaListPlayer &operator =(const VlcMediaListPlayer &);
    VlcMediaListPlayer(libvlc_media_list_player_t *);
    VlcMediaListPlayer &operator =(libvlc_media_list_player_t *);
    libvlc_media_list_player_t *data(); // refcount is NOT increased!

    /**
     * @brief VlcMediaListPlayer constructor
     * Uses the global VlcInstance
     */
    VlcMediaListPlayer();

    /**
     * @brief VlcMediaListPlayer constructor
     */
    explicit VlcMediaListPlayer(const VlcInstance &);
    explicit VlcMediaListPlayer(libvlc_instance_t *);

    virtual ~VlcMediaListPlayer();

    virtual void setMediaPlayer(const VlcMediaPlayer &player);
    virtual void setMediaList(const AbstractMediaList &list);

    virtual bool isPlaying();
    virtual VlcState::Type state();

    virtual void play();
    virtual void play(const VlcMedia &media);
    virtual void play(int index);
    virtual void pause();
    virtual void stop();
    virtual void next();
    virtual void previous();
    virtual void setPlaybackMode(const VlcPlaybackMode::Type &mode);

Q_SIGNALS:
    void played();
    void nextItemSet(const VlcMedia &);
    void stopped();
};

#endif // QTVLC_VLCMEDIALISTPLAYER_H
