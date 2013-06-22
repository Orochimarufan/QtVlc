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

#include <QtVlc/config.h>
#include <QtVlc/enum.h>

class QUrl;

struct libvlc_instance_t;
struct libvlc_media_t;
struct libvlc_event_manager_t;
struct libvlc_event_t;
struct libvlc_media_stats_t;

class QtVlc_EXPORT VlcMedia : public QObject
{
    Q_OBJECT
public:
    // from MRL
    explicit VlcMedia(const QString &location, libvlc_instance_t *instance);
    explicit VlcMedia(const QUrl &location, libvlc_instance_t *instance);

    // from path
    static VlcMedia *new_path(const QString &path, libvlc_instance_t *instance);

    // from fd
    explicit VlcMedia(int fd, libvlc_instance_t *instance);

    // duplicate
    VlcMedia *duplicate();


    // libvlc primitive
    VlcMedia(libvlc_media_t *);
    libvlc_media_t *libvlc_t(); // refcount is NOT increased!
    operator libvlc_media_t *(); // refcount is NOT increased!


    // destructor
    virtual ~VlcMedia();


    // location
    QString mrl();


    // Meta
    QString meta(VlcMeta::Type meta);
    void setMeta(VlcMeta::Type meta, QString value);
    bool saveMeta();
    void parse(bool async = true);
    bool isParsed();


    // State
    VlcState::Type state();


    // duration
    qint64 duration();


    // user data
    void setVlcUserData(void *data);
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

    // events
    void init();
    void deinit();
    static void libvlc_event_cb(const libvlc_event_t *, void *);
    void libvlc_event(const libvlc_event_t *);
};

// ************* inline ****************
// libvlc primitives
inline libvlc_media_t *VlcMedia::libvlc_t()
{
    return _media;
}

inline VlcMedia::operator libvlc_media_t *()
{
    return _media;
}

#endif // VLCMEDIA_H
