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

project(QtVlc-DemoApp)
IF(WIN32)
	cmake_minimum_required(VERSION 2.8.11)
ELSE()
	cmake_minimum_required(VERSION 2.8.10)
ENDIF()

SET(DemoApp_SOURCES
main.cpp
mainwindow.cpp
)

SET(DemoApp_HEADERS
mainwindow.h
)

SET(DemoApp_UI
mainwindow.ui
)

QT5_WRAP_UI(DemoApp_UI_HEADERS ${DemoApp_UI})

include_directories(../QtVlc ../QtVlcWidgets)

add_executable(QtVlcDemo ${DemoApp_SOURCES} ${DemoApp_HEADERS} ${DemoApp_UI_HEADERS})
qt5_use_modules(QtVlcDemo Core Widgets)
target_link_libraries(QtVlcDemo QtVlc QtVlcWidgets)
