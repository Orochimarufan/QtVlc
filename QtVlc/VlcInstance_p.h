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

#ifndef QTVLC_VLCINSTANCE_P_H
#define QTVLC_VLCINSTANCE_P_H

#include <vlc/vlc.h>

#include <QObject>

#include "common_p.h"

class VlcInstancePrivate : public VlcWrapperImpl
{
    Q_OBJECT
    VLC_WRAPPER_IMPL_PRIVATE(VlcInstancePrivate, libvlc_instance_t)

    static VlcInstancePrivate *global;
    static void deleteGlobalInstance() noexcept;

public:
    VLC_WRAPPER_IMPL_PUBLIC(VlcInstancePrivate, libvlc_instance_t)
    explicit VlcInstancePrivate(const QStringList &);

    void setUserAgent(const QString &, const QString &);

    static VlcInstancePrivate *globalInstance();
};

#endif // QTVLC_VLCINSTANCE_P_H
