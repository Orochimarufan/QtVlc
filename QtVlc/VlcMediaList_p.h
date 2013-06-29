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

#ifndef QTVLC_VLCMEDIALIST_P_H
#define QTVLC_VLCMEDIALIST_P_H

#include "common_p.h"

#include <vlc/vlc.h>

class VlcMedia;

class VlcMediaListPrivate : public VlcWrapperImpl
{
    Q_OBJECT
    VLC_WRAPPER_IMPL_PRIVATE(VlcMediaListPrivate, libvlc_media_list_t)

public:
    VLC_WRAPPER_IMPL_PUBLIC(VlcMediaListPrivate, libvlc_media_list_t)

    explicit VlcMediaListPrivate(libvlc_instance_t *);

    void setMedia(libvlc_media_t *); // no lock!
    libvlc_media_t *media(); // no lock! release media!

    int addMedia(libvlc_media_t *); // lock!
    int insertMedia(libvlc_media_t *, int); // lock!

    int removeIndex(int); // lock!

    int count(); // lock!
    libvlc_media_t *itemAtIndex(int); // lock! release media!
    int indexOfItem(libvlc_media_t *); // lock!

    int isReadOnly();

    void lock();
    void unlock();

signals:
    void libvlcEvent(const libvlc_event_t *);

    void willAddItem(const VlcMedia &, int);
    void itemAdded(const VlcMedia &, int);

    void willDeleteItem(const VlcMedia &, int);
    void itemDeleted(const VlcMedia &, int);

private:
    libvlc_event_manager_t *p_eventmanager;

    void attach_events();
    void detach_events();
    static void event_cb(const libvlc_event_t *, void *);
};

#endif // QTVLC_VLCMEDIALIST_P_H
