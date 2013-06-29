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

#include "VlcMedia_p.h"

VLC_WRAPPER_IMPL_CPP2(VlcMediaPrivate, libvlc_media_t, libvlc_media,
                      attach_events, detach_events)

VlcMediaPrivate::VlcMediaPrivate(libvlc_instance_t *instance, const QString &mrl, bool local) :
    VlcWrapperImpl()
{
    if (local)
        d = libvlc_media_new_path(instance, mrl.toUtf8().data());
    else
        d = libvlc_media_new_location(instance, mrl.toUtf8().data());

    VLC_WRAPPER_IMPL_INIT();

    attach_events();
}

VlcMediaPrivate::VlcMediaPrivate(libvlc_instance_t *instance, int fd) :
    VlcWrapperImpl()
{
    d = libvlc_media_new_fd(instance, fd);

    VLC_WRAPPER_IMPL_INIT();

    attach_events();
}

libvlc_media_t *VlcMediaPrivate::duplicate()
{
    if (!d) return nullptr;
    return libvlc_media_duplicate(d);
}

QString VlcMediaPrivate::mrl() const
{
    if (!d) return QString::null;
    return libvlc_media_get_mrl(d);
}

VlcState::Type VlcMediaPrivate::state()
{
    return static_cast<VlcState::Type>(libvlc_media_get_state(d));
}

qint64 VlcMediaPrivate::duration() const
{
    if (!d) return 0;
    return libvlc_media_get_duration(d);
}

void VlcMediaPrivate::setVlcUserData(void *p)
{
    if (!d) return;
    libvlc_media_set_user_data(d, p);
}

void *VlcMediaPrivate::vlcUserData()
{
    if (!d) return nullptr;
    return libvlc_media_get_user_data(d);
}

/*libvlc_media_stats_t *VlcMedia::stats(libvlc_media_stats_t *p_stats)
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
}*/

QString VlcMediaPrivate::meta(const VlcMeta::Type &meta)
{
    if (!d) return QString::null;
    return libvlc_media_get_meta(d, static_cast<libvlc_meta_t>(meta));
}

void VlcMediaPrivate::setMeta(const VlcMeta::Type &meta, const QString &value)
{
    if (!d) return;
    libvlc_media_set_meta(d, static_cast<libvlc_meta_t>(meta), value.toUtf8().data());
}

int VlcMediaPrivate::saveMeta() const
{
    if (!d) return -1;
    return libvlc_media_save_meta(d);
}

void VlcMediaPrivate::parse(bool async)
{
    if (!d) return;
    if (async)
        libvlc_media_parse_async(d);
    else
        libvlc_media_parse(d);
}

bool VlcMediaPrivate::isParsed() const
{
    if (!d) return false;
    return libvlc_media_is_parsed(d);
}

void VlcMediaPrivate::addOption(const QString &options)
{
    if (!d) return;
    libvlc_media_add_option(d, options.toUtf8().data());
}

void VlcMediaPrivate::addOptionFlag(const QString &options, unsigned flags)
{
    if (!d) return;
    libvlc_media_add_option_flag(d, options.toUtf8().data(), flags);
}

// events
inline void VlcMediaPrivate::attach_events()
{
    p_eventmanager = libvlc_media_event_manager(d);

#define ATTACH(event_type) libvlc_event_attach(p_eventmanager, event_type, &VlcMediaPrivate::event_cb, this)
    ATTACH(libvlc_MediaMetaChanged);
    ATTACH(libvlc_MediaSubItemAdded);
    ATTACH(libvlc_MediaDurationChanged);
    ATTACH(libvlc_MediaParsedChanged);
    ATTACH(libvlc_MediaFreed);
    ATTACH(libvlc_MediaStateChanged);
#undef ATTACH
}

inline void VlcMediaPrivate::detach_events()
{
#define DETACH(event_type) libvlc_event_detach(p_eventmanager, event_type, &VlcMediaPrivate::event_cb, this)
    DETACH(libvlc_MediaMetaChanged);
    DETACH(libvlc_MediaSubItemAdded);
    DETACH(libvlc_MediaDurationChanged);
    DETACH(libvlc_MediaParsedChanged);
    DETACH(libvlc_MediaFreed);
    DETACH(libvlc_MediaStateChanged);
#undef DETACH
}

void VlcMediaPrivate::event_cb(const libvlc_event_t *e, void *o)
{
    VlcMediaPrivate *p = static_cast<VlcMediaPrivate *>(o);

    emit p->libvlcEvent(e);

    switch(e->type)
    {
    case libvlc_MediaMetaChanged:
        emit p->metaChanged(static_cast<VlcMeta::Type>(e->u.media_meta_changed.meta_type));
        break;
    case libvlc_MediaSubItemAdded:
        emit p->subItemAdded(e->u.media_subitem_added.new_child);
        break;
    case libvlc_MediaDurationChanged:
        emit p->durationChanged(e->u.media_duration_changed.new_duration);
        break;
    case libvlc_MediaParsedChanged:
        emit p->parsedChanged(e->u.media_parsed_changed.new_status);
        break;
    case libvlc_MediaFreed:
        emit p->freed(e->u.media_freed.md);
        break;
    case libvlc_MediaStateChanged:
        emit p->stateChanged(static_cast<VlcState::Type>(e->u.media_state_changed.new_state));
        break;
    default:
        qDebug("VlcMedia: unknown Event: %i (%s)", e->type, libvlc_event_type_name(e->type));
    }
}
