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

#ifndef QTVLC_VLCMEDIAPLAYER_H
#define QTVLC_VLCMEDIAPLAYER_H

#include <QtCore/QObject>
#include <QtGui/qwindowdefs.h>

#include <QtVlc/config.h>
#include <QtVlc/enum.h>


struct libvlc_event_t;
struct libvlc_media_t;
struct libvlc_media_player_t;
struct libvlc_instance_t;

class VlcInstance;
class VlcMedia;
class VlcMediaPlayerPrivate;
class VlcMediaPlayerAudio;
class IVlcVideoDelegate;


/**
 * @brief The VlcMediaPlayer class
 * The media playback controller
 */
class QtVlc_EXPORT VlcMediaPlayer : public QObject
{
    Q_OBJECT
    VlcMediaPlayerPrivate *d;
    void d_connect();

public:
    /**
     * @brief Check if this VlcMediaPlayer is valid
     * True if this VlcMediaPlayer has underlying data.
     * Most methods will throw a NullPointer exception if called invalid objects.
     */
    bool isValid();

    // copy, assign & libvlc primitive
    VlcMediaPlayer(const VlcMediaPlayer &);
    VlcMediaPlayer &operator =(const VlcMediaPlayer &);
    VlcMediaPlayer(libvlc_media_player_t *player);
    VlcMediaPlayer &operator =(libvlc_media_player_t *);
    libvlc_media_player_t *data();

    /**
     * @brief VlcMediaPlayer constructor
     * Create a new VlcMediaPlayer instance
     * @param instance The VlcInstance
     */
    explicit VlcMediaPlayer(const VlcInstance &instance);

    /**
     * @brief VlcMediaPlayer constructor
     * Create a new VlcMediaPlayer instance
     * @param instance The libvlc_instance_t
     */
    explicit VlcMediaPlayer(libvlc_instance_t *instance);

    /**
     * @brief VlcMediaPlayer constructor
     * Create a new VlcMediaPlayer instance on the global VlcInstance
     */
    VlcMediaPlayer();

    // destructor
    virtual ~VlcMediaPlayer();

    // media
    /**
     * @brief Returns the current media
     * @return the media object
     */
    VlcMedia media();
    libvlc_media_t *media_();

    /**
     * @brief Set the current media
     * @param media the media object
     */
    void setMedia(libvlc_media_t *media);
    void setMedia(const VlcMedia &media);

    /**
     * @brief Set the current media and start playing
     * @param media the media object
     */
    void open(libvlc_media_t *media);
    void open(const VlcMedia &media);

    // position
    /**
     * @brief Get the media length
     * @return the media length in ms
     */
    qint64 length() const;

    /**
     * @brief Get the current playback time
     * @return the current playback time in ms
     */
    qint64 time() const;

    /**
     * @brief Set the playback time
     * @param time the new playback time in ms
     */
    void setTime(const qint64 &time);

    /**
     * @brief Get the current playback position
     * @return the current playback position in % (0.0-1.0)
     */
    float position() const;

    /**
     * @brief Set the playback position
     * @param pos the new playback position in % (0.0-1.0)
     */
    void setPosition(const float &pos);

    // state
    /**
     * @brief Get the current playback state
     */
    VlcState::Type state();

    // window
    /**
     * @brief Set the Video Widget ID
     * Calls are ignored if a IVlcVideoDelegate is set!
     * @param widget the WId of the target widget
     */
    void setVideoWidget(WId widget);

    /**
     * @brief Get the Video Widget ID
     * @return the Video Widget WId
     */
    WId videoWidget();

    /**
     * @brief Set the Video Widget Delegate
     * @param delegate object implementing IVlcVideoDelegate
     */
    void setVideoDelegate(IVlcVideoDelegate *delegate);

    /**
     * @brief Get the Video Widget Delegate
     * @return the IVlcVideoDelegate or nullptr
     */
    IVlcVideoDelegate *videoDelegate();

    // audio
    VlcMediaPlayerAudio audio();

public Q_SLOTS:
    // player
    /**
     * @brief Start playback
     */
    void play();

    /**
     * @brief Pause playback
     */
    void pause();

    /**
     * @brief Resume playback if paused
     */
    void resume();

    /**
     * @brief Toggle pause
     */
    void togglePause();

    /**
     * @brief Stop playback
     */
    void stop();

Q_SIGNALS:
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

#endif // QTVLC_VLCMEDIAPLAYER_H
