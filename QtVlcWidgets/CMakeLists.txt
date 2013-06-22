#/*****************************************************************************
# * QtVlc - C++ bindings for libVLC using Qt awesomeness
# * Copyright (C) 2013 Orochimarufan <orochimarufan.x3@gmail.com>
# *
# * This library is free software: you can redistribute it and/or modify
# * it under the terms of the GNU Lesser General Public License as published
# * by the Free Software Foundation, either version 3 of the License, or
# * (at your option) any later version.
# *
# * This library is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# * GNU Lesser General Public License for more details.
# *
# * You should have received a copy of the GNU Lesser General Public License
# * along with this library. If not, see <http://www.gnu.org/licenses/>.
# *****************************************************************************/

SET(QtVlcWidgets_SOURCES
VlcVideoWidget.cpp
)

SET(QtVlcWidgets_HEADERS
../include/QtVlcWidgets/VlcVideoWidget.h
)

add_definitions(-DQtVlcWidgets_LIBRARY)
add_library(QtVlcWidgets SHARED ${QtVlcWidgets_SOURCES} ${QtVlcWidgets_HEADERS})
QT5_USE_MODULES(QtVlcWidgets Core Gui Widgets)
target_link_libraries(QtVlcWidgets ${QtVlc_LIBRARY})

GET_PROPERTY(QtVlcWidgets_LIBRARY TARGET QtVlcWidgets PROPERTY LOCATION)
SET(QtVlcWidgets_LIBRARY ${QtVlcWidgets_LIBRARY} CACHE INTERNAL "")

