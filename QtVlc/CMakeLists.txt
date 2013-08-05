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

project(QtVlc_Core)

#-----------------------------------------------------------------------------
# Sources

SET(SOURCES
	common_p.cpp
	VlcError.cpp
	VlcInstance.cpp
	VlcInstance_p.cpp
	VlcMedia.cpp
	VlcMedia_p.cpp
	VlcMediaPlayer.cpp
	VlcMediaPlayer_p.cpp
	VlcMediaPlayerAudio.cpp
	VlcMediaPlayerVideo.cpp
	AbstractMediaList.cpp
	VlcMediaList.cpp
	VlcMediaList_p.cpp
	AbstractMediaListPlayer.cpp
	VlcMediaListPlayer.cpp
	VlcMediaListPlayer_p.cpp
)

#-----------------------------------------------------------------------------
# Private Headers

SET(HEADERS_PRIVATE
	common_p.h
	VlcInstance_p.h
	VlcMedia_p.h
	VlcMediaPlayer_p.h
	VlcMediaList_p.h
	VlcMediaListPlayer_p.h
)

#-----------------------------------------------------------------------------
# Public Headers

SET(HEADERS
	QtVlcConfig.h
	VlcEnum.h
	VlcError.h
	VlcInstance.h
	VlcMedia.h
	VlcMediaPlayer.h
	IVlcVideoDelegate.h
	VlcMediaPlayerAudio.h
	VlcMediaPlayerVideo.h
	AbstractMediaList.h
	VlcMediaList.h
	AbstractMediaListPlayer.h
	VlcMediaListPlayer.h
)

#-----------------------------------------------------------------------------
# Build the library

add_definitions(-DQtVlc_LIBRARY)
add_library(QtVlc SHARED ${SOURCES} ${HEADERS} ${HEADERS_PRIVATE})
QT5_USE_MODULES(QtVlc Core)
#target_link_libraries(QtVlc ${LIBVLC_LIBRARIES} ${LIBVLCCORE_LIBRARIES})
target_link_libraries(QtVlc ${LIBVLC_LIBRARIES})

IF(BUILD_VLC)
	add_dependencies(QtVlc libvlc)
ENDIF(BUILD_VLC)

GET_PROPERTY(QtVlc_LIBRARY TARGET QtVlc PROPERTY LOCATION)
SET(QtVlc_LIBRARY ${QtVlc_LIBRARY} CACHE INTERNAL "")

#-----------------------------------------------------------------------------
# Install library (on windows, put the dll in 'bin' and the archive in 'lib')

set_target_properties(QtVlc PROPERTIES PUBLIC_HEADER "${HEADERS}")

install(TARGETS QtVlc EXPORT QtVlcTargets COMPONENT core-dev
				RUNTIME DESTINATION bin COMPONENT core
				LIBRARY DESTINATION lib COMPONENT core
				ARCHIVE DESTINATION lib COMPONENT core
				PUBLIC_HEADER DESTINATION include/QtVlc)

#-----------------------------------------------------------------------------
# Export CMake Config

if(WIN32 AND NOT CYGWIN)
	set(LIB_CMAKE_PATH "cmake")
else()
	set(LIB_CMAKE_PATH "lib/cmake/QtVlc")
endif()

export(TARGETS QtVlc FILE "${PROJECT_BINARY_DIR}/QtVlcTargets.cmake")
export(PACKAGE QtVlc)

# build tree
set(INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
configure_file(QtVlcConfig.cmake.in "${PROJECT_BINARY_DIR}/QtVlcConfig.cmake" @ONLY)
# install tree
set(INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/include/QtVlc")
configure_file(QtVlcConfig.cmake.in "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/QtVlcConfig.cmake" @ONLY)
# both
configure_file(QtVlcConfigVersion.cmake.in "${PROJECT_BINARY_DIR}/QtVlcConfigVersion.cmake" @ONLY)

install(EXPORT QtVlcTargets DESTINATION "${LIB_CMAKE_PATH}" COMPONENT core-dev)
install(FILES ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/QtVlcConfig.cmake
			  ${PROJECT_BINARY_DIR}/QtVlcConfigVersion.cmake
		DESTINATION "${LIB_CMAKE_PATH}" COMPONENT core-dev)


