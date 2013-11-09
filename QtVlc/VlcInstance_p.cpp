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

#include <QStringList>

#include "VlcEnum.h"

VLC_WRAPPER_IMPL_CPP(VlcInstancePrivate, libvlc_instance_t, libvlc)

VlcInstancePrivate *VlcInstancePrivate::global = nullptr;

VlcInstancePrivate::VlcInstancePrivate(const QStringList &args) :
    VlcWrapperImpl(), d(nullptr)
{
    qRegisterMetaType<VlcState::Type>("VlcState::Type");
    qRegisterMetaType<VlcMeta::Type>("VlcMeta::Type");

    char **argv = (char **)malloc(sizeof(char **) * (args.count()+1));

    // We need to pass -Idummy so we have a *chance* that
    // VLC doesn't try to load the Qt4 plugin.
    argv[args.count()] = (char *)"-Idummy";

    for (int i = 0; i < args.count(); i++)
        argv[i] = (char *)qstrdup(args.at(i).toLocal8Bit().data());

    d = libvlc_new(args.count()+1, argv);

    if (d)
        qDebug("Initialized libvlc.");
    else
        abort();

    VLC_WRAPPER_IMPL_INIT();
}

void VlcInstancePrivate::setUserAgent(const QString &appString, const QString &httpString)
{
    libvlc_set_user_agent(d, appString.toLocal8Bit().data(), httpString.toLocal8Bit().data());
}

#ifndef __GNUC__
#include <cstdlib>
#endif

VlcInstancePrivate *VlcInstancePrivate::globalInstance()
{
    if (global == nullptr)
        initGlobalInstance(QStringList());

    return global;
}

bool VlcInstancePrivate::initGlobalInstance(const QStringList &args)
{
    if (global == nullptr)
    {
        global = new VlcInstancePrivate(args);
#ifndef __GNUC__
        static bool atexit_reg = false;
        if (!atexit_reg)
        {
            atexit(&__Janitor__run);
            atexit_reg = true;
        }
#endif
        return true;
    }
    else
        return false;
}

void VlcInstancePrivate::freeGlobalInstance()
{
    if (global != nullptr)
    {
        global->release();
        global = nullptr;
    }
}
