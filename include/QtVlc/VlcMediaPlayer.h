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

#ifndef VLCMEDIAPLAYER_H
#define VLCMEDIAPLAYER_H


#include <QtCore/QObject>
#include <QtGui/qwindowdefs.h>

#include <QtVlc/config.h>
#include <QtVlc/enum.h>


struct libvlc_event_t;
struct libvlc_event_manager_t;
struct libvlc_media_t;
struct libvlc_media_player_t;
struct libvlc_instance_t;


/**
 * @brief The IVlcVideoDelegate class
 * A interface to be provided by Video Vidgets
 */
class VlcMediaPlayer;
class QtVlc_EXPORT IVlcVideoDelegate
{
public:
    virtual ~IVlcVideoDelegate() {}

    virtual WId request(bool b_keep_size = true, unsigned int i_width = 0,  unsigned int i_height = 0) = 0;
    virtual void release() = 0;
};

/**
 * @brief The VlcMediaPlayer class
 * implicitly converts to/from libvlc_media_player_t *
 */
class QtVlc_EXPORT VlcMediaPlayer : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief VlcMediaPlayer constructor
     * Create a new VlcMediaPlayer
     * @param instance the instance object
     */
    explicit VlcMediaPlayer(libvlc_instance_t *instance);


    // libvlc primitive
    VlcMediaPlayer(libvlc_media_player_t *);
    libvlc_media_player_t *libvlc_t(); // refcount is NOT increased!
    operator libvlc_media_player_t *(); // refcount is NOT increased!


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
    void setMedia(libvlc_media_t *media);

    /**
     * @brief Set the current media and start playing
     * @param media the media object
     */
    void open(libvlc_media_t *media);

    // position
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
     * WARNING: undefined behaviour when used with multiple VlcMediaPlayer
     *          proxies per libvlc_media_player_t or direct libvlc access
     * @param delegate object implementing IVlcVideoDelegate
     */
    void setVideoDelegate(IVlcVideoDelegate *delegate);

    /**
     * @brief Get the Video Widget Delegate
     * @return the IVlcVideoDelegate or nullptr
     */
    IVlcVideoDelegate *videoDelegate();

public slots:
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

    // widget
    WId _widget;
    IVlcVideoDelegate *_delegate;
    void _setVideoWidget(WId widget);

    // events
    void init_events();
    void kill_events();
    static void libvlc_event_cb(const libvlc_event_t *, void *);
    void libvlc_event(const libvlc_event_t *);
};


// ************* inline ****************
// libvlc primitives
inline libvlc_media_player_t *VlcMediaPlayer::libvlc_t()
{
    return _player;
}

inline VlcMediaPlayer::operator libvlc_media_player_t *()
{
    return _player;
}

// media
inline void VlcMediaPlayer::open(libvlc_media_t *media)
{
    setMedia(media);
    play();
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

#endif // VLCMEDIAPLAYER_H
