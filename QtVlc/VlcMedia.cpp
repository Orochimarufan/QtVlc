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

#include <vlc/vlc.h>

#include <QtVlc/VlcMedia.h>


// create
VlcMediaPtr VlcMedia::new_location(const QString &location, libvlc_instance_t *instance)
{
    auto media = libvlc_media_new_location(instance, location.toUtf8().data());

    return new VlcMedia(media);
}

VlcMediaPtr VlcMedia::new_path(const QString &path, libvlc_instance_t *instance)
{
    auto media = libvlc_media_new_path(instance, path.toUtf8().data());

    return new VlcMedia(media);
}

VlcMediaPtr VlcMedia::new_fd(const int &fd, libvlc_instance_t *instance)
{
    auto media = libvlc_media_new_fd(instance, fd);

    return new VlcMedia(media);
}

VlcMediaPtr VlcMedia::create(libvlc_media_t *media)
{
    libvlc_media_retain(media);

    return new VlcMedia(media);
}

VlcMedia::VlcMedia(libvlc_media_t *media) :
    QObject(), _media(media)
{
    init_events();
}

// duplicate
VlcMediaPtr VlcMedia::duplicate()
{
    return new VlcMedia(libvlc_media_duplicate(_media));
}

// destructor
VlcMedia::~VlcMedia()
{
    kill_events();
    libvlc_media_release(_media);
}


// events
inline void VlcMedia::init_events()
{
    _evm = libvlc_media_event_manager(_media);

#define ATTACH(event_type) libvlc_event_attach(_evm, event_type, &VlcMedia::libvlc_event_cb, this)
    ATTACH(libvlc_MediaMetaChanged);
    ATTACH(libvlc_MediaSubItemAdded);
    ATTACH(libvlc_MediaDurationChanged);
    ATTACH(libvlc_MediaParsedChanged);
    ATTACH(libvlc_MediaFreed);
    ATTACH(libvlc_MediaStateChanged);
#undef ATTACH
}

inline void VlcMedia::kill_events()
{
#define DETACH(event_type) libvlc_event_detach(_evm, event_type, &VlcMedia::libvlc_event_cb, this)
    DETACH(libvlc_MediaMetaChanged);
    DETACH(libvlc_MediaSubItemAdded);
    DETACH(libvlc_MediaDurationChanged);
    DETACH(libvlc_MediaParsedChanged);
    DETACH(libvlc_MediaFreed);
    DETACH(libvlc_MediaStateChanged);
#undef DETACH
}

void VlcMedia::libvlc_event_cb(const libvlc_event_t *e, void *o)
{
    QObject *object = static_cast<QObject *>(o);
    VlcMedia *media = qobject_cast<VlcMedia *>(object);
    if (media)
        media->libvlc_event(e);
}

inline void VlcMedia::libvlc_event(const libvlc_event_t *e)
{
    emit libvlcEvent(e);

    switch(e->type)
    {
    case libvlc_MediaMetaChanged:
        emit metaChanged(static_cast<VlcMeta::Type>(e->u.media_meta_changed.meta_type));
        break;
    case libvlc_MediaSubItemAdded:
        emit subItemAdded(e->u.media_subitem_added.new_child);
        break;
    case libvlc_MediaDurationChanged:
        emit durationChanged(e->u.media_duration_changed.new_duration);
        break;
    case libvlc_MediaParsedChanged:
        emit parsedChanged(e->u.media_parsed_changed.new_status);
        break;
    case libvlc_MediaFreed:
        emit freed(e->u.media_freed.md);
        break;
    case libvlc_MediaStateChanged:
        emit stateChanged(static_cast<VlcState::Type>(e->u.media_state_changed.new_state));
        break;
    default:
        qDebug("VlcMedia: unknown Event: %i", e->type);
    }
}


// data
QString VlcMedia::mrl()
{
    return libvlc_media_get_mrl(_media);
}

VlcState::Type VlcMedia::state()
{
    return static_cast<VlcState::Type>(libvlc_media_get_state(_media));
}

qint64 VlcMedia::duration()
{
    return libvlc_media_get_duration(_media);
}


// user data
void VlcMedia::setVlcUserData(void *data)
{
    libvlc_media_set_user_data(_media, data);
}

void *VlcMedia::vlcUserData()
{
    return libvlc_media_get_user_data(_media);
}


// stats
libvlc_media_stats_t *VlcMedia::stats(libvlc_media_stats_t *p_stats)
{
    libvlc_media_stats_t *out;
    if (p_stats == nullptr)
        out = new libvlc_media_stats_t;
    else
        out = p_stats;

    if (!libvlc_media_get_stats(_media, out))
    {
        if (p_stats == nullptr)
            delete out;
        return nullptr;
    }

    return out;
}


// meta
QString VlcMedia::meta(VlcMeta::Type meta)
{
    return libvlc_media_get_meta(_media, static_cast<libvlc_meta_t>(meta));
}

void VlcMedia::setMeta(VlcMeta::Type meta, QString value)
{
    libvlc_media_set_meta(_media, static_cast<libvlc_meta_t>(meta), value.toUtf8().data());
}

bool VlcMedia::saveMeta()
{
    return libvlc_media_save_meta(_media);
}

void VlcMedia::parse(bool async)
{
    if (async)
        libvlc_media_parse_async(_media);
    else
        libvlc_media_parse(_media);
}

bool VlcMedia::isParsed()
{
    return libvlc_media_is_parsed(_media);
}


// Options
void VlcMedia::addOption(const QString &options)
{
    libvlc_media_add_option(_media, options.toUtf8().data());
}

void VlcMedia::addOptionFlag(const QString &options, unsigned flags)
{
    libvlc_media_add_option_flag(_media, options.toUtf8().data(), flags);
}
