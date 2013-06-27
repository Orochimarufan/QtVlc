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

#ifndef QTVLC_ERROR_H
#define QTVLC_ERROR_H

#include <QtVlc/config.h>

#include <exception>

class QtVlc_EXPORT NullPointer : public std::exception
{
    virtual const char *what() const noexcept
    {
        return "Tried to work with a Vlc object that points nowhere.";
    }
};

class QtVlc_EXPORT VlcError : public std::exception
{
public:
    static VlcError *create() noexcept;
    static VlcError *createNoClear() noexcept;
    virtual const char *what() const noexcept;
    ~VlcError() noexcept;

private:
    const char *reason;
    VlcError() noexcept;
};

#ifndef NO_NULL_CHECK
#   define CHECKNP if (d == nullptr) throw new NullPointer();
#else
#   define CHECKNP
#endif

#ifndef NO_VLC_ERROR_CHECK
#   define CHECKERR if (libvlc_errmsg()) throw VlcError::create();
#else
#   define CHECKERR
#endif

#endif // QTVLC_ERROR_H
