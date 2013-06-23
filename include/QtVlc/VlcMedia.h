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

#ifndef VLCMEDIA_H
#define VLCMEDIA_H

#include <QtCore/QObject>
#include <QtCore/QUrl>

#include <QtVlc/config.h>
#include <QtVlc/enum.h>

#include <QtVlc/VlcInstance.h>

struct libvlc_media_t;
struct libvlc_event_manager_t;
struct libvlc_event_t;
struct libvlc_media_stats_t;

class VlcMedia;
typedef VlcMedia * VlcMediaPtr;

class QtVlc_EXPORT VlcMedia : public QObject
{
    Q_OBJECT
public:
    // from location
    static VlcMediaPtr new_location(const QString &location, libvlc_instance_t *instance);
    static VlcMediaPtr new_location(const QString &location, VlcInstancePtr instance);
    static VlcMediaPtr new_location(const QUrl &location, libvlc_instance_t *instance);
    static VlcMediaPtr new_location(const QUrl &location, VlcInstancePtr instance);

    // from path
    static VlcMediaPtr new_path(const QString &path, libvlc_instance_t *instance);
    static VlcMediaPtr new_path(const QString &path, VlcInstancePtr instance);

    // from fd
    static VlcMediaPtr new_fd(const int &fd, libvlc_instance_t *instance);
    static VlcMediaPtr new_fd(const int &fd, VlcInstancePtr instance);

    // duplicate
    VlcMediaPtr duplicate();

    // libvlc primitive
    static VlcMediaPtr create(libvlc_media_t *);
    libvlc_media_t *data(); // refcount is NOT increased!

    // destructor
    virtual ~VlcMedia();

    /**
     * @brief Get the media MRL
     * @return the MRL
     */
    QString mrl();

    // meta
    /**
     * @brief Get media metadata
     * @param meta the metadata type to get
     * @return the metadata value
     */
    QString meta(VlcMeta::Type meta);

    /**
     * @brief Set media metadata
     * @param meta the metadata type to set
     * @param value the new metadata value
     */
    void setMeta(VlcMeta::Type meta, QString value);

    /**
     * @brief Save metadata to file (if possible)
     * @return whether it succeeded
     */
    bool saveMeta();

    /**
     * @brief Parse the metadata contained in the pointed file
     * @param async whether or not to parse it asynchronously
     * parsedChanged() will be emitted when an async parse succeeds and
     * it wasn't parsed already. use isParsed() to determine that before calling.
     */
    void parse(bool async = true);

    /**
     * @brief Get whether or not this media is parsed
     * @return wether or not this media is parsed
     */
    bool isParsed();

    // state
    /**
     * @brief Get the current playback state of this media.
     * @return the playback state
     */
    VlcState::Type state();


    // duration
    /**
     * @brief Get the media duration
     * @return the media duration (in ms)
     */
    qint64 duration();


    // user data
    /**
     * @brief Set the user data
     * @param the data
     */
    void setVlcUserData(void *data);

    /**
     * @brief Get the user data
     * @return the data
     */
    void *vlcUserData();


    // options
    void addOption(const QString &options);
    void addOptionFlag(const QString &options, unsigned flags);


    // stats
    libvlc_media_stats_t *stats(libvlc_media_stats_t *p_stats = nullptr);

signals:
    void libvlcEvent(const libvlc_event_t *);
    void metaChanged(const VlcMeta::Type &);
    void subItemAdded(libvlc_media_t *);
    void durationChanged(const qint64 &);
    void parsedChanged(const int &);
    void freed(libvlc_media_t *);
    void stateChanged(const VlcState::Type &);

private:
    libvlc_media_t *_media;
    libvlc_event_manager_t *_evm;

    explicit VlcMedia(libvlc_media_t *); // does not refcount-increase!

    // events
    void init_events();
    void kill_events();
    static void libvlc_event_cb(const libvlc_event_t *, void *);
    void libvlc_event(const libvlc_event_t *);
};

// ************* inline ****************
// create
inline VlcMediaPtr VlcMedia::new_location(const QString &location, VlcInstancePtr instance)
{
    return new_location(location, instance->data());
}

inline VlcMediaPtr VlcMedia::new_location(const QUrl &location, VlcInstancePtr instance)
{
    return new_location(location.toString(), instance->data());
}

inline VlcMediaPtr VlcMedia::new_location(const QUrl &location, libvlc_instance_t *instance)
{
    return new_location(location.toString(), instance);
}

inline VlcMediaPtr VlcMedia::new_path(const QString &path, VlcInstancePtr instance)
{
    return new_path(path, instance->data());
}

inline VlcMediaPtr VlcMedia::new_fd(const int &fd, VlcInstancePtr instance)
{
    return new_fd(fd, instance->data());
}

inline libvlc_media_t *VlcMedia::data()
{
    return _media;
}

#endif // VLCMEDIA_H
