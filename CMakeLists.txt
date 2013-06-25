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

project(QtVlc)
IF(WIN32)
    cmake_minimum_required(VERSION 2.8.11)
ELSE()
    cmake_minimum_required(VERSION 2.8.10)
ENDIF()

###### version ######
SET(QTVLC_VERSION 0.1.0)

FIND_PROGRAM(GIT git)
IF(GIT)
        EXECUTE_PROCESS(
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                COMMAND ${GIT} rev-parse --short HEAD
                OUTPUT_VARIABLE QTVLC_VERSION_GIT OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        IF(QTVLC_VERSION_GIT STREQUAL "")
                SET(QTVLC_VERSION_GIT exported)
        ENDIF()
ELSE(GIT)
        SET(QTVLC_VERSION_GIT exported)
ENDIF(GIT)

MESSAGE(STATUS "Building QtVlc ${QTVLC_VERSION} (${QTVLC_VERSION_GIT})")

######## Set CMake options ########
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Output all executables and shared libs in the main build folder, not in subfolders.
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

IF(WIN32)
    SET(ENV{PATH} "${PROJECT_SOURCE_DIR}\\bin;$ENV{PATH}")
ENDIF()

######## Set compiler flags ########
IF(APPLE)
	# assume clang 4.1.0+, add C++0x/C++11 stuff
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -stdlib=libc++")
ELSEIF(UNIX)
	# assume GCC, add C++0x/C++11 stuff
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
ELSEIF(MINGW)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
ENDIF()

###### libvlc ######
# TODO: build vlc from source
IF(WIN32)
    SET(VLC_PF_PATH "VideoLAN\\VLC")
    IF(ENV{VLC_PATH})
        MESSAGE(STATUS "(WIN32) Searching for VLC in \"$ENV{VLC_PATH}\"")
        SET(VLC_PATH "$ENV{VLC_PATH}")
    ELSE()
        # try to guess vlc path
        MESSAGE(STATUS "(WIN32) Searching for VLC in \"%PF%\\${VLC_PF_PATH}\"")
        SET(VLC_PATH "$ENV{PROGRAMFILES}\\${VLC_PF_PATH}")
    ENDIF()
    SET(VLC_SDK "${VLC_PATH}\\sdk")
    SET(ENV{PKG_CONFIG_PATH} "${VLC_SDK}\\lib\\pkgconfig};$ENV{PKG_CONFIG_PATH}")
ENDIF()

include(FindPkgConfig)
pkg_check_modules(LIBVLC REQUIRED libvlc QUIET)
pkg_search_module(LIBVLCCORE REQUIRED libvlccore vlc-plugin QUIET)

IF(WIN32)
    # fix paths
    SET(LIBVLC_INCLUDE_DIRS "${VLC_SDK}\\include")
    SET(LIBVLC_LIBRARY_DIRS "${VLC_SDK}\\lib")
    MESSAGE(STATUS "(WIN32) Copying VLC DLLs")
    FILE(COPY "${VLC_PATH}\\libvlc.dll" "${VLC_PATH}\\libvlccore.dll" "${VLC_PATH}\\plugins"
         DESTINATION "${PROJECT_BINARY_DIR}")
    FILE(REMOVE "${PROJECT_BINARY_DIR}\\plugins\\gui\\libqt4_plugin.dll")
ENDIF()

include_directories(${LIBVLC_INCLUDE_DIRS})
link_directories(${LIBVLC_LIBRARY_DIRS})

MESSAGE(STATUS "found libvlc, version ${LIBVLC_VERSION}")

###### Qt ######
find_package(Qt5Core REQUIRED)
include_directories(${Qt5Core_INCLUDE_DIRS})
find_package(Qt5Widgets REQUIRED)
include_directories(${Qt5Widgets_INCLUDE_DIRS})

MESSAGE(STATUS "found Qt5, version ${Qt5Core_VERSION_STRING}")

###### config ######
CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/build_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/inc/build_config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/inc)

###### build ######
include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(QtVlc)
add_subdirectory(QtVlcWidgets)

#OPTION(DEMO "Build Demo App" ON)
#IF(DEMO)
	add_subdirectory(DemoApp)
#ENDIF()
