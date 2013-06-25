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

#ifndef QTVLC_VLCMEDIA_P_H
#define QTVLC_VLCMEDIA_P_H

#include <vlc/vlc.h>

#include <QtVlc/enum.h>

#include "common_p.h"

class VlcMediaPrivate : public VlcWrapperImpl
{
    Q_OBJECT
    VLC_WRAPPER_IMPL_PRIVATE(VlcMediaPrivate, libvlc_media_t)

public:
    VLC_WRAPPER_IMPL_PUBLIC(VlcMediaPrivate, libvlc_media_t)

    VlcMediaPrivate(libvlc_instance_t *, const QString &, bool local = false);
    VlcMediaPrivate(libvlc_instance_t *, int fd);

    libvlc_media_t *duplicate();

    QString mrl() const;

    QString meta(const VlcMeta::Type &);
    void setMeta(const VlcMeta::Type &, const QString &);
    int saveMeta() const;
    void parse(bool async = true);
    bool isParsed() const;

    VlcState::Type state();

    qint64 duration() const;

    void setVlcUserData(void *);
    void *vlcUserData();

    void addOption(const QString &);
    void addOptionFlag(const QString &, unsigned);
    
signals:
    void libvlcEvent(const libvlc_event_t *);
    void metaChanged(const VlcMeta::Type &);
    void subItemAdded(libvlc_media_t *);
    void durationChanged(const qint64 &);
    void parsedChanged(const int &);
    void freed(libvlc_media_t *);
    void stateChanged(const VlcState::Type &);
    
private:
    libvlc_event_manager_t *p_eventmanager;

    void attach_events();
    void detach_events();
    static void event_cb(const libvlc_event_t *, void *);
};

#endif // QTVLC_VLCMEDIA_P_H
