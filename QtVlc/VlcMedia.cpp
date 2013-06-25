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

#include <QtCore/QUrl>

#include <QtVlc/VlcInstance.h>
#include <QtVlc/VlcMedia.h>
#include "VlcMedia_p.h"


VlcMedia::VlcMedia(const VlcMedia &o) :
    QObject(), d(o.d)
{
    d->retain();
}

VlcMedia::VlcMedia(libvlc_media_t *m) :
    QObject()
{
    d = VlcMediaPrivate::instance(m);
}

VlcMedia::VlcMedia(libvlc_instance_t *instance, QString location, bool local) :
    QObject()
{
    d = new VlcMediaPrivate(instance, location, local);
}

VlcMedia::VlcMedia(const VlcInstance &instance, QString location, bool local) :
    QObject()
{
    d = new VlcMediaPrivate(getref<VlcInstance>(instance)->data(), location, local);
}

VlcMedia::VlcMedia(libvlc_instance_t *instance, QUrl location)
{
    d = new VlcMediaPrivate(instance, location.toString());
}

VlcMedia::VlcMedia(const VlcInstance &instance, QUrl location)
{
    d = new VlcMediaPrivate(getref<VlcInstance>(instance)->data(), location.toString());
}

VlcMedia::VlcMedia(libvlc_instance_t *instance, int fd)
{
    d = new VlcMediaPrivate(instance, fd);
}

VlcMedia::VlcMedia(const VlcInstance &instance, int fd)
{
    d = new VlcMediaPrivate(getref<VlcInstance>(instance)->data(), fd);
}

VlcMedia::~VlcMedia()
{
    d->release();
}

libvlc_media_t *VlcMedia::data()
{
    return d->data();
}

// dupe
libvlc_media_t *VlcMedia::duplicate_()
{
    return d->duplicate();
}

VlcMedia VlcMedia::duplicate()
{
    return VlcMedia(d->duplicate());
}

// data
QString VlcMedia::mrl() const
{
    return d->mrl();
}

VlcState::Type VlcMedia::state()
{
    return d->state();
}

qint64 VlcMedia::duration()
{
    return d->duration();
}


// user data
void VlcMedia::setVlcUserData(void *data)
{
    d->setVlcUserData(data);
}

void *VlcMedia::vlcUserData()
{
    return d->vlcUserData();
}


// meta
QString VlcMedia::meta(VlcMeta::Type meta)
{
    return d->meta(meta);
}

void VlcMedia::setMeta(VlcMeta::Type meta, QString value)
{
    d->setMeta(meta, value);
}

bool VlcMedia::saveMeta()
{
    return d->saveMeta();
}

void VlcMedia::parse(bool async)
{
    d->parse(async);
}

bool VlcMedia::isParsed()
{
    return d->isParsed();
}


// Options
void VlcMedia::addOption(const QString &options)
{
    d->addOption(options);
}

void VlcMedia::addOptionFlag(const QString &options, unsigned flags)
{
    d->addOptionFlag(options, flags);
}
