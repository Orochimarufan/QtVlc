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

#include "QtVlc/config.h"
#include "QtVlc/enum.h"
#include "QtVlc/VlcInstance.h"

// constructor
VlcInstance::VlcInstance(const QStringList &args) :
    QObject(), _instance(nullptr)
{
    char **argv = (char **)malloc(sizeof(char **) * args.count());
    for (int i = 0; i < args.count(); i++)
        argv[i] = (char *)qstrdup(args.at(i).toLocal8Bit().data());

    _instance = libvlc_new(args.count(), argv);

    if (_instance)
        qDebug("Initialized libvlc.");
    else
        abort();

    // don't need to libvlc_retain(), libvlc_new() has refcount == 1
}

// libvlc primitive
VlcInstance::VlcInstance(libvlc_instance_t *libvlc_t) :
    QObject(), _instance(libvlc_t)
{
    libvlc_retain(_instance);
}

// destructor
VlcInstance::~VlcInstance()
{
    libvlc_release(_instance);
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
    libvlc_set_user_agent(_instance, appString.toLocal8Bit().data(), httpString.toLocal8Bit().data());
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

QString VlcInstance::QtVlc_build_libvlccore_version()
{
    return QStringLiteral(QTVLC_BUILT_LIBVLCCORE_VERSION);
}

QString VlcInstance::QtVlc_build_qt_version()
{
    return QStringLiteral(QTVLC_BUILT_QTCORE_VERSION);
}
