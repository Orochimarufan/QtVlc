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

#ifndef QTVLC_VLCMEDIAPLAYERAUDIO_H
#define QTVLC_VLCMEDIAPLAYERAUDIO_H

#include <QtCore/QObject>
#include <QtCore/QHash>

#include "QtVlcConfig.h"

struct libvlc_media_player_t;
class VlcMediaPlayer;
class VlcMediaPlayerPrivate;

/**
 * @brief The VlcMediaPlayerAudio class
 * A VlcMediaPlayer's Audio controls.
 */
class QtVlc_EXPORT VlcMediaPlayerAudio : public QObject
{
    Q_OBJECT
    VlcMediaPlayerPrivate *d;

public:
    /**
     * @brief create a NULL VlcMediaPlayerAudio
     */
    VlcMediaPlayerAudio();

    /**
     * @brief Check if this VlcMediaPlayerAudio is valid
     * True if not constructed with VlcMediaPlayerAudio().
     * Most methods will throw a NullPointer exception if called invalid objects.
     * @return
     */
    bool isValid();

    VlcMediaPlayerAudio(const VlcMediaPlayerAudio &);
    VlcMediaPlayerAudio &operator =(const VlcMediaPlayerAudio &);
    VlcMediaPlayerAudio(libvlc_media_player_t *);
    VlcMediaPlayerAudio &operator =(libvlc_media_player_t *);
    VlcMediaPlayerAudio(const VlcMediaPlayer &);
    VlcMediaPlayerAudio &operator =(const VlcMediaPlayer &);
    VlcMediaPlayerAudio(VlcMediaPlayerPrivate *);

    virtual ~VlcMediaPlayerAudio();

    /**
     * @brief Get the audio volume
     * @return the audio volume (in %)
     */
    int volume() const;

    /**
     * @brief Get the audio mute state
     * @return the audio mute state
     */
    bool isMuted() const;

    /**
     * @brief Set the audio track number
     * @param track the new track number
     */
    void setTrack(const int &track);

    /**
     * @brief audioTrack
     * @return
     */
    int track() const;

    int trackCount() const;

    QHash<int, QString> trackDescription() const;

public slots:
    /**
     * @brief Set the audio volume
     * @param volume the audio volume (in %)
     */
    void setVolume(const int &volume);

    /**
     * @brief Set audio mute state
     * @param mute the new mute state
     */
    void setMuted(const bool &mute);

    /**
     * @brief Toggle the audio mute state
     */
    void toggleMuted();
};

#endif // QTVLC_VLCMEDIAPLAYERAUDIO_H
