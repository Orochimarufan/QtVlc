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
cmake_minimum_required(VERSION 2.8.10)

IF(POLICY CMP0020)
    CMAKE_POLICY(SET CMP0020 NEW)
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

###### libvlc ######
MESSAGE(STATUS "Checking for libvlc")
OPTION(BUILD_VLC "build (lib)vlc from source" OFF)

IF(BUILD_VLC)
	SET(VLC_SOURCE_DIR "${PROJECT_SOURCE_DIR}/vlc")
	SET(VLC_BINARY_DIR "${PROJECT_BINARY_DIR}/vlc")

	#FILE(WRITE "${VLC_SOURCE_DIR}/.git" "gitdir: ${PROJECT_SOURCE_DIR}/.git/modules/vlc")

	IF(GIT)
		EXECUTE_PROCESS(
			WORKING_DIRECTORY ${VLC_SOURCE_DIR}
			COMMAND ${GIT} describe HEAD
			OUTPUT_VARIABLE VLC_DESCRIBE_GIT OUTPUT_STRIP_TRAILING_WHITESPACE
		)
		SET(LIBVLC_VERSION ${VLC_DESCRIBE_GIT})
	ENDIF(GIT)
	MESSAGE(STATUS "    building from source (${VLC_DESCRIBE_GIT})")

	include(ExternalProject)
	ExternalProject_add(VLC
		PREFIX "${VLC_BINARY_DIR}"
		STAMP_DIR "${VLC_BINARY_DIR}/stamp"
		SOURCE_DIR "${VLC_SOURCE_DIR}"
		BUILD_IN_SOURCE 1
		PATCH_COMMAND sh ${PROJECT_SOURCE_DIR}/bin/patch_vlc.sh
		CONFIGURE_COMMAND sh -c "./configure \"--prefix=${VLC_BINARY_DIR}\" --disable-qt --disable-rpath \"--with-kde-solid=${VLC_BINARY_DIR}/tmp\" ${VLC_CONFIGURE} && touch \"${VLC_BINARY_DIR}/stamp/build\""
		BUILD_COMMAND python "${PROJECT_SOURCE_DIR}/bin/build_vlc.py" "${VLC_BINARY_DIR}" "${VLC_SOURCE_DIR}" install -j4
	)
	ExternalProject_Add_Step(VLC bootstrap
		COMMAND ./bootstrap
		DEPENDERS configure
		WORKING_DIRECTORY "${VLC_SOURCE_DIR}"
	)
	add_custom_command(
		OUTPUT "${VLC_BINARY_DIR}/lib/libvlc.so"
		DEPENDS VLC
		COMMAND true
	)
	add_custom_target(libvlc
		DEPENDS "${VLC_BINARY_DIR}/lib/libvlc.so"
	)
	SET(LIBVLC_INCLUDE_DIRS "${VLC_BINARY_DIR}/include")
	SET(LIBVLC_LIBRARIES "${VLC_BINARY_DIR}/lib/libvlc.so")

ELSE()
	IF(WIN32)
        IF(ENV{VLC_PATH})
            MESSAGE(STATUS "    (WIN32) Searching for VLC in \"$ENV{VLC_PATH}\"")
            SET(VLC_PATH "$ENV{VLC_PATH}")
        ELSE()
            FIND_FILE(VLC_PKGCONFIG libvlc.pc PATHS [HKEY_LOCAL_MACHINE\\SOFTWARE\\VideoLAN\\VLC;InstallPath])
            IF(NOT VLC_PKGCONFIG)
		        SET(VLC_PF_PATH "VideoLAN\\VLC")
			    # try to guess vlc path
                MESSAGE(STATUS "    (WIN32) Searching for VLC in \"$ENV{ProgramFiles(x86)}\\${VLC_PF_PATH}\"")
                SET(VLC_PATH "$ENV{ProgramFiles(x86)}\\${VLC_PF_PATH}")
		    ENDIF()
        ENDIF()
        IF(VLC_PKGCONFIG)
            GET_FILENAME_COMPONENT(VLC_PKGCONFIG ${VLC_PKGCONFIG} PATH)
        ELSE()
            SET(VLC_PKGCONFIG "${VLC_PATH}\\sdk\\lib\\pkgconfig")
        ENDIF()
        SET(ENV{PKG_CONFIG_PATH} "${VLC_PKGCONFIG}")
	ENDIF()

	include(FindPkgConfig)
    pkg_check_modules(LIBVLC REQUIRED libvlc QUIET)
    pkg_search_module(LIBVLCCORE REQUIRED libvlccore vlc-plugin QUIET)

	IF(WIN32)
		# fix paths
        SET(LIBVLC_INCLUDE_DIRS "${VLC_PATH}/sdk/include")
        FIND_FILE(LIBVLC_DLL_A libvlc.dll.a PATHS "${VLC_PATH}/sdk/lib")
        FIND_FILE(LIBVLC_DLL libvlc.dll PATHS "${VLC_PATH}")
        FIND_FILE(LIBVLCCORE_DLL libvlccore.dll PATHS "${VLC_PATH}")
        IF (NOT LIBVLC_DLL OR NOT LIBVLCCORE_DLL OR NOT LIBVLC_DLL_A)
            MESSAGE(FATAL_ERROR "Could not find required files : ${LIBVLC_DLL} ${LIBVLCCORE_DLL} ${LIBVLC_DLL_A}")
        ENDIF()
        GET_FILENAME_COMPONENT(LIBVLC_LIBRARY_DIRS ${LIBVLC_DLL_A} PATH)
		MESSAGE(STATUS "    (WIN32) Copying VLC DLLs")
        FILE(COPY "${LIBVLC_DLL}" "${LIBVLCCORE_DLL}" "${VLC_PATH}\\plugins"
			 DESTINATION "${PROJECT_BINARY_DIR}")
		FILE(REMOVE "${PROJECT_BINARY_DIR}\\plugins\\gui\\libqt4_plugin.dll")
        SET(LIBVLC_LIBRARIES ${LIBVLC_DLL_A})
	ENDIF()

	include_directories(${LIBVLC_INCLUDE_DIRS})
	link_directories(${LIBVLC_LIBRARY_DIRS})

	MESSAGE(STATUS "    found libvlc, version ${LIBVLC_VERSION} (system)")
ENDIF()

###### Qt ######
MESSAGE(STATUS "Checking for Qt5")

find_package(Qt5Core REQUIRED)
include_directories(${Qt5Core_INCLUDE_DIRS})
find_package(Qt5Widgets REQUIRED)
include_directories(${Qt5Widgets_INCLUDE_DIRS})

MESSAGE(STATUS "    found Qt5, version ${Qt5Core_VERSION_STRING}")

######## Set compiler flags ########
MESSAGE(STATUS "Checking operating system/compiler")
IF(APPLE)
	# assume clang 4.1.0+, add C++0x/C++11 stuff
	MESSAGE(STATUS "    Assuming Apple OSX/CLANG")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -stdlib=libc++")
ELSEIF(UNIX)
	# assume GCC, add C++0x/C++11 stuff
	MESSAGE(STATUS "    Assuming Unix/GCC")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
ELSEIF(MINGW)
	MESSAGE(STATUS "    Assuming Windows/GCC-mingw32")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
ELSE()
	MESSAGE(FATAL_ERROR "    Unknown Operating system! (OSX, UNIX, Windows/MinGW)")
ENDIF()

######## Set CMake options ########
MESSAGE(STATUS "Configuring...")
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Output all executables and shared libs in the main build folder, not in subfolders.
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

IF(WIN32)
	SET(ENV{PATH} "${PROJECT_SOURCE_DIR}\\bin;$ENV{PATH}")
ENDIF()

###### config ######
CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/build_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/inc/build_config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/inc)

###### build ######
include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(QtVlc)
add_subdirectory(QtVlcWidgets)

OPTION(BUILD_DEMO "Build Demo App" ON)
IF(BUILD_DEMO)
	add_subdirectory(DemoApp)
ENDIF()
