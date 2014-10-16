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

#include "QtVlcConfig.h"
#include "VlcEnum.h"
#include "VlcError.h"
#include "VlcInstance.h"
#include "VlcInstance_p.h"

bool VlcInstance::isValid()
{
    return d != nullptr;
}

VlcInstance::VlcInstance(const VlcInstance &o) :
    QObject(), d(o.d)
{
    if (d != nullptr)
        d->retain();
}

VlcInstance &VlcInstance::operator =(const VlcInstance &o)
{
    if (d != nullptr)
        d->release();

    d = o.d;

    if (d != nullptr)
        d->retain();
    
    return *this;
}

VlcInstance::VlcInstance(const QStringList &args) :
    QObject()
{
    d = new VlcInstancePrivate(args);
}

VlcInstance::VlcInstance(libvlc_instance_t *p) :
    QObject()
{
    d = VlcInstancePrivate::instance(p);
}

VlcInstance &VlcInstance::operator =(libvlc_instance_t *p)
{
    if (d != nullptr)
        d->release();

    d = VlcInstancePrivate::instance(p);
    
    return *this;
}

VlcInstance::VlcInstance() :
    QObject()
{
    d = VlcInstancePrivate::globalInstance();

    d->retain();
}

VlcInstance::~VlcInstance()
{
    if (d != nullptr);
        d->release();
}

libvlc_instance_t *VlcInstance::data()
{
    CHECKNP
    return d->data();
}

bool VlcInstance::initGlobalInstance(const QStringList &args)
{
    return VlcInstancePrivate::initGlobalInstance(args);
}

void VlcInstance::freeGlobalInstance()
{
    VlcInstancePrivate::freeGlobalInstance();
}

libvlc_instance_t *VlcInstance::globalInstance()
{
    return VlcInstancePrivate::globalInstance()->data();
}

// version queries
QString VlcInstance::libvlc_version()
{
    return QString(libvlc_get_version());
}

QString VlcInstance::libvlc_compiler()
{
    return QString(libvlc_get_compiler());
}

QString VlcInstance::libvlc_changeset()
{
    return QString(libvlc_get_changeset());
}

// others
void VlcInstance::setUserAgent(const QString &appString, const QString &httpString)
{
    CHECKNP
    d->setUserAgent(appString, httpString);
}

// build config
#include "build_config.h"

QString VlcInstance::QtVlc_version()
{
    return QStringLiteral(QTVLC_BUILT_VERSION);
}

QString VlcInstance::QtVlc_version_git()
{
    return QStringLiteral(QTVLC_BUILT_VERSION_GIT);
}

QString VlcInstance::QtVlc_build_libvlc_version()
{
    return QStringLiteral(QTVLC_BUILT_LIBVLC_VERSION);
}

QString VlcInstance::QtVlc_build_qt_version()
{
    return QStringLiteral(QTVLC_BUILT_QT_VERSION);
}
