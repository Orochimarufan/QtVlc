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

SET(QtVlc_SOURCES
VlcInstance.cpp
VlcMedia.cpp
VlcMediaPlayer.cpp
VlcMediaPlayerAudio.cpp
VlcMediaPlayerVideo.cpp
)

SET(QtVlc_HEADERS
../include/QtVlc/config.h
../include/QtVlc/enum.h
../include/QtVlc/VlcInstance.h
../include/QtVlc/VlcMedia.h
../include/QtVlc/VlcMediaPlayer.h
../include/QtVlc/IVlcVideoDelegate.h
../include/QtVlc/VlcMediaPlayerAudio.h
../include/QtVlc/VlcMediaPlayerVideo.h
)


add_definitions(-DQtVlc_LIBRARY)
add_library(QtVlc SHARED ${QtVlc_SOURCES} ${QtVlc_HEADERS})
QT5_USE_MODULES(QtVlc Core)
#target_link_libraries(QtVlc ${LIBVLC_LIBRARIES} ${LIBVLCCORE_LIBRARIES})
target_link_libraries(QtVlc ${LIBVLC_LIBRARIES})

GET_PROPERTY(QtVlc_LIBRARY TARGET QtVlc PROPERTY LOCATION)
SET(QtVlc_LIBRARY ${QtVlc_LIBRARY} CACHE INTERNAL "")

