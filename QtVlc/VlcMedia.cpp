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
#include <QtVlc/error.h>
#include "VlcMedia_p.h"

// signals
void VlcMedia::d_connect()
{
#define SIG(S) connect(d, SIGNAL(S), this, SIGNAL(S))
    SIG(libvlcEvent(const libvlc_event_t *));
    SIG(metaChanged(const VlcMeta::Type &));
    SIG(subItemAdded(libvlc_media_t *));
    SIG(durationChanged(const qint64 &));
    SIG(parsedChanged(const int &));
    SIG(freed(libvlc_media_t *));
    SIG(stateChanged(const VlcState::Type &));
#undef SIG
}

// validity
bool VlcMedia::isValid()
{
    return d != nullptr;
}

// assign
VlcMedia &VlcMedia::operator =(const VlcMedia &other)
{
    if (d != nullptr)
    {
        disconnect(d);
        d->release();
    }

    d = other.d;

    if (d != nullptr)
    {
        d->retain();
        d_connect();
    }

    return *this;
}

VlcMedia &VlcMedia::operator =(libvlc_media_t *other)
{
    if (d != nullptr)
    {
        disconnect(d);
        d->release();
    }

    d = VlcMediaPrivate::instance(other);

    if (d != nullptr)
    {
        d->retain();
        d_connect();
    }

    return *this;
}

// NULL constructor
VlcMedia::VlcMedia() :
    QObject(), d(nullptr)
{}

// copy constructor
VlcMedia::VlcMedia(const VlcMedia &o) :
    QObject(), d(o.d)
{
    if (d != nullptr)
    {
        d->retain();
        d_connect();
    }
}

VlcMedia::VlcMedia(libvlc_media_t *m) :
    QObject()
{
    d = VlcMediaPrivate::instance(m);

    if (d != nullptr)
        d_connect();
}

// location constructors
VlcMedia::VlcMedia(libvlc_instance_t *instance, QString location, bool local) :
    QObject()
{
    d = new VlcMediaPrivate(instance, location, local);

    d_connect();
}

VlcMedia::VlcMedia(const VlcInstance &instance, QString location, bool local) :
    QObject()
{
    d = new VlcMediaPrivate(getref<VlcInstance>(instance)->data(), location, local);

    d_connect();
}

VlcMedia::VlcMedia(QString location, bool local) :
    QObject()
{
    d = new VlcMediaPrivate(VlcInstance::globalInstance(), location, local);

    d_connect();
}

VlcMedia::VlcMedia(libvlc_instance_t *instance, QUrl location) :
    QObject()
{
    d = new VlcMediaPrivate(instance, location.toString());

    d_connect();
}

VlcMedia::VlcMedia(const VlcInstance &instance, QUrl location) :
    QObject()
{
    d = new VlcMediaPrivate(getref<VlcInstance>(instance)->data(), location.toString());

    d_connect();
}

VlcMedia::VlcMedia(QUrl location) :
    QObject()
{
    d = new VlcMediaPrivate(VlcInstance::globalInstance(), location.toString(), false);

    d_connect();
}

// fd constructors
VlcMedia::VlcMedia(libvlc_instance_t *instance, int fd) :
    QObject()
{
    d = new VlcMediaPrivate(instance, fd);

    d_connect();
}

VlcMedia::VlcMedia(const VlcInstance &instance, int fd) :
    QObject()
{
    d = new VlcMediaPrivate(getref<VlcInstance>(instance)->data(), fd);

    d_connect();
}

VlcMedia::VlcMedia(int fd) :
    QObject()
{
    d = new VlcMediaPrivate(VlcInstance::globalInstance(), fd);

    d_connect();
}

// destructor
VlcMedia::~VlcMedia()
{
    if (d != nullptr)
        d->release();
}

// data
libvlc_media_t *VlcMedia::data()
{
    CHECKNP
    return d->data();
}

// dupe
libvlc_media_t *VlcMedia::duplicate_()
{
    if (d != nullptr)
        return d->duplicate();
    else
        return nullptr;
}

VlcMedia VlcMedia::duplicate()
{
    if (d != nullptr)
        return VlcMedia(d->duplicate());
    else
        return *this;
}

// data
QString VlcMedia::mrl() const
{
    CHECKNP
    return d->mrl();
}

VlcState::Type VlcMedia::state()
{
    CHECKNP
    return d->state();
}

qint64 VlcMedia::duration()
{
    CHECKNP
    return d->duration();
}


// user data
void VlcMedia::setVlcUserData(void *data)
{
    CHECKNP
    d->setVlcUserData(data);
}

void *VlcMedia::vlcUserData()
{
    CHECKNP
    return d->vlcUserData();
}


// meta
QString VlcMedia::meta(VlcMeta::Type meta)
{
    CHECKNP
    return d->meta(meta);
}

void VlcMedia::setMeta(VlcMeta::Type meta, QString value)
{
    CHECKNP
    d->setMeta(meta, value);
}

bool VlcMedia::saveMeta()
{
    CHECKNP
    return d->saveMeta();
}

void VlcMedia::parse(bool async)
{
    CHECKNP
    d->parse(async);
}

bool VlcMedia::isParsed()
{
    CHECKNP
    return d->isParsed();
}


// Options
void VlcMedia::addOption(const QString &options)
{
    CHECKNP
    d->addOption(options);
}

void VlcMedia::addOptionFlag(const QString &options, unsigned flags)
{
    CHECKNP
    d->addOptionFlag(options, flags);
}
