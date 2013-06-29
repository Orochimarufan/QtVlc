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

#ifndef QTVLC_VLCMEDIALIST_H
#define QTVLC_VLCMEDIALIST_H

#include <QtVlc/config.h>
#include <QtVlc/AbstractMediaList.h>

class VlcInstance;
class VlcMediaListPrivate;

struct libvlc_instance_t;
struct libvlc_media_list_t;
struct libvlc_event_t;

class QtVlc_EXPORT VlcMediaList : public AbstractMediaList
{
    Q_OBJECT
    VlcMediaListPrivate *d;
    void d_connect();

public:
    /**
     * @brief Check if this VlcMediaList is valid
     * True if this VlcMediaList has underlying data.
     * Most methods will throw a NullPointer exception if called invalid objects.
     */
    bool isValid();

    // copy, assignment & libvlc primitive
    VlcMediaList(const VlcMediaList &);
    VlcMediaList &operator =(const VlcMediaList &);
    VlcMediaList(libvlc_media_list_t *);
    VlcMediaList &operator =(libvlc_media_list_t *);
    libvlc_media_list_t *data(); // refcount is NOT increased!

    /**
     * @brief VlcMediaList constructor
     * Uses the global VlcInstance
     */
    VlcMediaList();

    /**
     * @brief VlcMediaList constructor
     */
    explicit VlcMediaList(const VlcInstance &);
    explicit VlcMediaList(libvlc_instance_t *);

    virtual ~VlcMediaList();

    void setMedia(const VlcMedia &);
    VlcMedia media();

    virtual int length() const;
    virtual VlcMedia at(int index);
    virtual int indexOf(const VlcMedia &media, int from);

    virtual bool isReadOnly();
    virtual void append(const VlcMedia &media);
    virtual void insert(int index, const VlcMedia &media);
    virtual void removeAt(int index);

Q_SIGNALS:
    void libvlcEvent(const libvlc_event_t *);
};

#endif // QTVLC_VLCMEDIALIST_H
