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

#ifndef QTVLC_IVLCVIDEODELEGATE_H
#define QTVLC_IVLCVIDEODELEGATE_H

#include "QtVlcConfig.h"


/**
 * @brief The IVlcVideoDelegate class
 * A interface to be provided by Video Vidgets
 */
class QtVlc_EXPORT IVlcVideoDelegate
{
public:
    virtual ~IVlcVideoDelegate() {}

    virtual WId request(bool b_keep_size = true, unsigned int i_width = 0,  unsigned int i_height = 0) = 0;
    virtual void release() = 0;
};

#endif // QTVLC_IVLCVIDEODELEGATE_H
