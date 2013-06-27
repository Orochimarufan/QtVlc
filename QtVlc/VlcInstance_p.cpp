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

#include "VlcInstance_p.h"

#include <cstdlib>

#include <QStringList>

#include <QtVlc/enum.h>

VLC_WRAPPER_IMPL_CPP(VlcInstancePrivate, libvlc_instance_t, libvlc)

VlcInstancePrivate *VlcInstancePrivate::global = nullptr;

VlcInstancePrivate::VlcInstancePrivate(const QStringList &args) :
    VlcWrapperImpl(), d(nullptr)
{

    qRegisterMetaType<VlcState::Type>("VlcState::Type");
    qRegisterMetaType<VlcMeta::Type>("VlcMeta::Type");

    char **argv = (char **)malloc(sizeof(char **) * args.count());
    for (int i = 0; i < args.count(); i++)
        argv[i] = (char *)qstrdup(args.at(i).toLocal8Bit().data());

    d = libvlc_new(args.count(), argv);

    if (d)
        qDebug("Initialized libvlc.");
    else
        abort();

    VLC_WRAPPER_IMPL_INIT()
}

void VlcInstancePrivate::setUserAgent(const QString &appString, const QString &httpString)
{
    libvlc_set_user_agent(d, appString.toLocal8Bit().data(), httpString.toLocal8Bit().data());
}

VlcInstancePrivate *VlcInstancePrivate::globalInstance()
{
    if (global == nullptr)
    {
        global = new VlcInstancePrivate(QStringList());
        atexit(&VlcInstancePrivate::deleteGlobalInstance);
    }

    return global;
}

void VlcInstancePrivate::deleteGlobalInstance() noexcept
{
    delete global;
    global = nullptr;
}
