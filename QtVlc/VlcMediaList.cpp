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

#include "VlcMediaList.h"
#include "VlcInstance.h"
#include "VlcMedia.h"
#include "VlcError.h"

#include "VlcMediaList_p.h"

void VlcMediaList::d_connect()
{
    connect(d, SIGNAL(libvlcEvent(const libvlc_event_t *)), SIGNAL(libvlcEvent(const libvlc_event_t*)));
    connect(d, SIGNAL(willAddItem(VlcMedia,int)), SIGNAL(itemAboutToBeAdded(VlcMedia,int)));
    connect(d, SIGNAL(itemAdded(VlcMedia,int)), SIGNAL(itemAdded(VlcMedia,int)));
    connect(d, SIGNAL(willDeleteItem(VlcMedia,int)), SIGNAL(itemAboutToBeRemoved(VlcMedia,int)));
    connect(d, SIGNAL(itemDeleted(VlcMedia,int)), SIGNAL(itemRemoved(VlcMedia,int)));
}

bool VlcMediaList::isValid()
{
    return d != nullptr;
}

VlcMediaList::VlcMediaList(const VlcMediaList &o) :
    AbstractMediaList(), d(o.d)
{
    if (d != nullptr)
    {
        d->retain();
        d_connect();
    }
}

VlcMediaList &VlcMediaList::operator =(const VlcMediaList &o)
{
    if (d != nullptr)
        d->release();

    d = o.d;

    if (d != nullptr)
    {
        d->retain();
        d_connect();
    }

    return *this;
}

VlcMediaList::VlcMediaList(libvlc_media_list_t *p_ml) :
    AbstractMediaList()
{
    d = VlcMediaListPrivate::instance(p_ml);

    if (d != nullptr)
        d_connect();
}

VlcMediaList &VlcMediaList::operator =(libvlc_media_list_t *p_ml)
{
    if (d != nullptr)
        d->release();

    d = VlcMediaListPrivate::instance(p_ml);

    if (d != nullptr)
        d_connect();

    return *this;
}

libvlc_media_list_t *VlcMediaList::data()
{
    CHECKNP
    return d->data();
}

VlcMediaList::VlcMediaList() :
    AbstractMediaList()
{
    d = new VlcMediaListPrivate(VlcInstance::globalInstance());

    d_connect();
}

VlcMediaList::VlcMediaList(const VlcInstance &inst)
{
    d = new VlcMediaListPrivate(getref<VlcInstance>(inst)->data());

    d_connect();
}

VlcMediaList::VlcMediaList(libvlc_instance_t *p_i)
{
    d = new VlcMediaListPrivate(p_i);

    d_connect();
}

VlcMediaList::~VlcMediaList()
{
    d->release();
}

void VlcMediaList::setMedia(const VlcMedia &m)
{
    CHECKNP
    d->setMedia(getref<VlcMedia>(m)->data());
}

VlcMedia VlcMediaList::media()
{
    CHECKNP
    libvlc_media_t *m1 = d->media();
    VlcMedia m2 = VlcMedia(m1);
    libvlc_media_release(m1);
    return m2;
}


struct LOCK
{
    VlcMediaListPrivate *d;
    LOCK(VlcMediaListPrivate *p_d) : d(p_d)
    {
        d->lock();
    }
    ~LOCK() {
        d->unlock();
    }
};
#define WITH_LOCK LOCK l(d)

int VlcMediaList::length() const
{
    CHECKNP WITH_LOCK;
    return d->count();
}

VlcMedia VlcMediaList::at(int index)
{
    CHECKNP WITH_LOCK;
    libvlc_media_t *m1 = d->itemAtIndex(index);
    VlcMedia m2 = VlcMedia(m1);
    libvlc_media_release(m1);
    return m2;
}

int VlcMediaList::indexOf(const VlcMedia &media, int from)
{
    CHECKNP WITH_LOCK;

    int index = d->indexOfItem(getref<VlcMedia>(media)->data());

    if (index < from && index != -1)
    {
        if (from > d->count())
            return -1;

        for (index=from; index < d->count(); index++)
            if (d->itemAtIndex(index) == getref<VlcMedia>(media)->data())
                return index;

        return -1;
    }

    return index;
}

bool VlcMediaList::isReadOnly()
{
    CHECKNP WITH_LOCK;
    return d->isReadOnly();
}

void VlcMediaList::append(const VlcMedia &media)
{
    CHECKNP WITH_LOCK;
    d->addMedia(getref<VlcMedia>(media)->data());
}

void VlcMediaList::insert(int index, const VlcMedia &media)
{
    CHECKNP WITH_LOCK;
    d->insertMedia(getref<VlcMedia>(media)->data(), index);
}

void VlcMediaList::removeAt(int index)
{
    CHECKNP WITH_LOCK;
    d->removeIndex(index);
}
