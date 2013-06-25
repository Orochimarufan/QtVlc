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
#include <QtCore/QHash>
#include <QtCore/QSharedPointer>
#include <QtGui/qwindowdefs.h>

#include <QtVlc/config.h>
#include <QtVlc/enum.h>
#include <QtVlc/VlcInstance.h>
#include <QtVlc/VlcMedia.h>
#include <QtVlc/IVlcVideoDelegate.h>


struct libvlc_event_t;
struct libvlc_event_manager_t;
struct libvlc_media_t;
struct libvlc_media_player_t;
struct libvlc_instance_t;

class VlcMediaPlayer;
class VlcMediaPlayerAudio;
class VlcMediaPlayerVideo;


/**
 * @brief A VlcMediaPlayer shared pointer.
 */
typedef QSharedPointer<VlcMediaPlayer> VlcMediaPlayerPtr;


/**
 * @brief The VlcMediaPlayer class
 * This class is no mere proxy but holds data,
 * therefore there must only be one instance per underlying libvlc_media_player_t
 * you should only use it by means of the VlcMediaPlayerPtr (QSharedPointer)
 */
class QtVlc_EXPORT VlcMediaPlayer : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief VlcMediaPlayer constructor
     * Create a new VlcMediaPlayer instance
     * @param instance the instance object [libvlc_instance_t *]
     * @return a QSharedPointer pointing to the instance [VlcMediaPlayerPtr]
     */
    static VlcMediaPlayerPtr create(libvlc_instance_t *instance);

    /**
     * @brief VlcMediaPlayer constructor
     * Create a new VlcMediaPlayer instance
     * @param instancethe instance object [VlcInstance *]
     * @returna QSharedPointer pointing to the instance [VlcMediaPlayerPtr]
     */
    static VlcMediaPlayerPtr create(VlcInstance *instance);

    /**
     * @brief VlcMediaPlayer constructor
     * Create a VlcMediaPlayer instance from an existing libvlc_media_player_t pointer.
     * If there already is an instance pointing to that libvlc_media_player_t, it gets recalled.
     * @param player the player object [libvlc_media_player_t *]
     * @return a QSharedPointer pointing to the instance [VlcMediaPlayerPtr]
     */
    static VlcMediaPlayerPtr create(libvlc_media_player_t *player);
    inline libvlc_media_player_t *data();

    // destructor
    virtual ~VlcMediaPlayer();

    // media
    /**
     * @brief Returns the current media
     * @return the media object
     */
    libvlc_media_t *media();

    /**
     * @brief Set the current media
     * @param media the media object
     */
    inline void setMedia(VlcMediaPtr media);
    void setMedia(libvlc_media_t *media);

    /**
     * @brief Set the current media and start playing
     * @param media the media object
     */
    inline void open(VlcMediaPtr media);
    inline void open(libvlc_media_t *media);

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

    // window id
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
    inline void setVideoDelegate(IVlcVideoDelegate *delegate);

    /**
     * @brief Get the Video Widget Delegate
     * @return the IVlcVideoDelegate or nullptr
     */
    inline IVlcVideoDelegate *videoDelegate();

    inline VlcMediaPlayerAudio *audio();
    inline VlcMediaPlayerVideo *video();

    inline void connect(const char *signal, const QObject *receiver, const char *slot);
    inline void connect(const QObject *sender, const char *signal, const char *slot);

public slots:
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

private:
    libvlc_media_player_t *_player;
    libvlc_event_manager_t *_evm;

    VlcMediaPlayerAudio *_audio;
    VlcMediaPlayerVideo *_video;

    // widget
    WId _widget;
    IVlcVideoDelegate *_delegate;
    void _setVideoWidget(WId widget);

    // events
    void init_events();
    void kill_events();
    static void libvlc_event_cb(const libvlc_event_t *, void *);
    void libvlc_event(const libvlc_event_t *);

    // instance management
    explicit VlcMediaPlayer(libvlc_instance_t *);
    explicit VlcMediaPlayer(libvlc_media_player_t *);
    static QHash<libvlc_media_player_t *, QWeakPointer<VlcMediaPlayer>> instances;
    static void deletePtr(VlcMediaPlayer *);
};


// ************* inline ****************
// instance management
inline VlcMediaPlayerPtr VlcMediaPlayer::create(libvlc_instance_t *instance)
{
    VlcMediaPlayerPtr ptr(new VlcMediaPlayer(instance), &deletePtr);
    instances[ptr->data()] = ptr.toWeakRef();
    return ptr;
}

inline VlcMediaPlayerPtr VlcMediaPlayer::create(VlcInstance *instance)
{
    return create(instance->data());
}

inline VlcMediaPlayerPtr VlcMediaPlayer::create(libvlc_media_player_t *player)
{
    if (instances.contains(player))
        return instances[player].toStrongRef();
    else
    {
        VlcMediaPlayerPtr ptr(new VlcMediaPlayer(player), &deletePtr);
        instances[player] = ptr.toWeakRef();
        return ptr;
    }
}

// libvlc primitives
inline libvlc_media_player_t *VlcMediaPlayer::data()
{
    return _player;
}

// media
inline void VlcMediaPlayer::open(libvlc_media_t *media)
{
    setMedia(media);
    play();
}

inline void VlcMediaPlayer::open(VlcMediaPtr media)
{
    setMedia(media);
    play();
}

inline void VlcMediaPlayer::setMedia(VlcMediaPtr media)
{
    setMedia(media->data());
}

// widget
inline void VlcMediaPlayer::setVideoDelegate(IVlcVideoDelegate *delegate)
{
    _delegate = delegate;
}

inline IVlcVideoDelegate *VlcMediaPlayer::videoDelegate()
{
    return _delegate;
}

// audio/video
inline VlcMediaPlayerAudio *VlcMediaPlayer::audio()
{
    return _audio;
}

inline VlcMediaPlayerVideo *VlcMediaPlayer::video()
{
    return _video;
}

// connect
inline void VlcMediaPlayer::connect(const char *signal, const QObject *receiver, const char *slot)
{
    QObject::connect(this, signal, receiver, slot);
}

inline void VlcMediaPlayer::connect(const QObject *sender, const char *signal, const char *slot)
{
    QObject::connect(sender, signal, slot);
}

#endif // QTVLC_VLCMEDIAPLAYER_H
