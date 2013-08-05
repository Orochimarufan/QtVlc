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

#include "VlcError.h"

#include <cstring>

VlcError::VlcError() noexcept :
    std::exception()
{
    reason = strdup(libvlc_errmsg());
}

VlcError::~VlcError() noexcept
{
    delete reason;
}

const char *VlcError::what() const noexcept
{
    return reason;
}

VlcError *VlcError::create() noexcept
{
    VlcError *e = nullptr;
    if (libvlc_errmsg())
        e = new VlcError();
    libvlc_clearerr();
    return e;
}

VlcError *VlcError::createNoClear() noexcept
{
    if (libvlc_errmsg())
        return new VlcError();
    return nullptr;
}
