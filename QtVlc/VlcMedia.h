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

#ifndef QTVLC_VLCMEDIA_H
#define QTVLC_VLCMEDIA_H

#include <QtCore/QObject>

#include "QtVlcConfig.h"
#include "VlcEnum.h"

struct libvlc_instance_t;
struct libvlc_media_t;
struct libvlc_event_manager_t;
struct libvlc_event_t;
class VlcInstance;
class VlcMediaPrivate;

/**
 * @brief The VlcMedia class
 * An abstract presentation of a playable media
 */
class QtVlc_EXPORT VlcMedia : public QObject
{
    Q_OBJECT
    VlcMediaPrivate *d;
    void d_connect();

public:
    /**
     * @brief Check if this VlcMedia is valid.
     * True if this VlcMedia has underlying data.
     * Most methods will throw a NullPointer exception if called invalid objects.
     */
    bool isValid();

    // copy, assign & libvlc primitive
    VlcMedia &operator =(const VlcMedia &);
    VlcMedia(const VlcMedia &);
    bool operator ==(const VlcMedia &);
    VlcMedia &operator =(libvlc_media_t *);
    VlcMedia(libvlc_media_t *);
    bool operator ==(libvlc_media_t *);
    libvlc_media_t *data();

    // NULL constructor
    /**
     * @brief VlcMedia constructor
     * Create a NULL VlcMedia
     */
    VlcMedia();

    // location constructors
    /**
     * @brief VlcMedia constructor
     * @param instance The VlcInstance
     * @param location The media location
     * @param local Wether location is a local path
     */
    explicit VlcMedia(const VlcInstance &instance, QString location, bool local = false);

    /**
     * @brief VlcMedia constructor
     * @param instance The libvlc_instance_t
     * @param location The media location
     * @param local Wether location is a local path
     */
    explicit VlcMedia(libvlc_instance_t *instance, QString location, bool local = false);

    /**
     * @brief VlcMedia constructor
     * @param location The media location
     * @param local Wether location is a local path
     * Uses the global VlcInstance
     */
    explicit VlcMedia(QString location, bool local = false);

    /**
     * @brief VlcMedia constructor
     * @param instance The VlcInstance
     * @param location The media location
     */
    explicit VlcMedia(const VlcInstance &instance, QUrl location);

    /**
     * @brief VlcMedia constructor
     * @param instance The libvlc_instance_t
     * @param location The media location
     */
    explicit VlcMedia(libvlc_instance_t *instance, QUrl location);

    /**
     * @brief VlcMedia constructor
     * @param location The media location
     * Uses the global VlcInstance
     */
    explicit VlcMedia(QUrl location);

    // fd constructors
    /**
     * @brief VlcMedia constructor
     * @param instance The VlcInstance
     * @param fd The open file descriptor
     */
    explicit VlcMedia(const VlcInstance &instance, int fd);

    /**
     * @brief VlcMedia constructor
     * @param instance The libvlc_instance_t
     * @param fd The open file descriptor
     */
    explicit VlcMedia(libvlc_instance_t *instance, int fd);

    /**
     * @brief VlcMedia constructor
     * @param fd The open file descriptor
     * Uses the global VlcInstance
     */
    explicit VlcMedia(int fd);

    // duplicate
    VlcMedia duplicate();
    libvlc_media_t *duplicate_();

    // destructor
    virtual ~VlcMedia();

    /**
     * @brief Get the media MRL
     * @return the MRL
     */
    QString mrl() const;

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

Q_SIGNALS:
    void libvlcEvent(const libvlc_event_t *);
    void metaChanged(const VlcMeta::Type &);
    void subItemAdded(libvlc_media_t *);
    void durationChanged(const qint64 &);
    void parsedChanged(const int &);
    void freed(libvlc_media_t *);
    void stateChanged(const VlcState::Type &);
};

#endif // QTVLC_VLCMEDIA_H
