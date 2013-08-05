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

project(QtVlc_Widgets)

#-----------------------------------------------------------------------------
# Sources

SET(SOURCES
	VlcVideoWidget.cpp
	TimeLabel.cpp
	TimeTooltip.cpp
	SeekStyle.cpp
	SeekSlider.cpp
	SoundSlider.cpp
	SoundWidget.cpp
	SpecialButtons.cpp
)

#-----------------------------------------------------------------------------
# Private Headers

SET(HEADERS
	QtVlcWidgetsConfig.h
	QtVlcWidgetsConst.h
	VlcVideoWidget.h
	TimeLabel.h
	TimeTooltip.h
	SeekSlider.h
	SoundSlider.h
	SoundWidget.h
	SpecialButtons.h
)

#-----------------------------------------------------------------------------
# Public Headers

SET(HEADERS_PRIVATE
	SeekStyle.h
)

#-----------------------------------------------------------------------------
# Qt Resources

SET(RESOURCES
	../vlc/modules/gui/qt4/vlc.qrc
)

#-----------------------------------------------------------------------------
# Build the library

include_directories(../QtVlc)

QT5_ADD_RESOURCES(RCC ${RESOURCES})
add_definitions(-DQtVlcWidgets_LIBRARY)
add_library(QtVlcWidgets SHARED ${SOURCES} ${HEADERS} ${HEADERS_PRIVATE} ${RCC})
QT5_USE_MODULES(QtVlcWidgets Core Gui Widgets)
target_link_libraries(QtVlcWidgets QtVlc)

GET_PROPERTY(QtVlcWidgets_LIBRARY TARGET QtVlcWidgets PROPERTY LOCATION)
SET(QtVlcWidgets_LIBRARY ${QtVlcWidgets_LIBRARY} CACHE INTERNAL "")

#-----------------------------------------------------------------------------
# Install library (on windows, put the dll in 'bin' and the archive in 'lib')

set_target_properties(QtVlcWidgets PROPERTIES PUBLIC_HEADER "${HEADERS}")

install(TARGETS QtVlcWidgets EXPORT QtVlcWidgetsTargets COMPONENT Widgets
				RUNTIME DESTINATION bin
				LIBRARY DESTINATION lib
				ARCHIVE DESTINATION lib
				PUBLIC_HEADER DESTINATION include/QtVlcWidgets)

#-----------------------------------------------------------------------------
# Export CMake Config

if(WIN32 AND NOT CYGWIN)
	set(LIB_CMAKE_PATH "cmake")
else()
	set(LIB_CMAKE_PATH "lib/cmake/QtVlcWidgets")
endif()

export(TARGETS QtVlcWidgets QtVlc FILE "${PROJECT_BINARY_DIR}/QtVlcWidgetsTargets.cmake")
export(PACKAGE QtVlcWidgets)

# build tree
set(EXPORT_LOCAL ON)
set(INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
configure_file(QtVlcWidgetsConfig.cmake.in "${PROJECT_BINARY_DIR}/QtVlcWidgetsConfig.cmake" @ONLY)
# install tree
set(EXPORT_LOCAL OFF)
set(INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/include/QtVlcWidgets")
configure_file(QtVlcWidgetsConfig.cmake.in "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/QtVlcWidgetsConfig.cmake" @ONLY)
# both
configure_file(QtVlcWidgetsConfigVersion.cmake.in "${PROJECT_BINARY_DIR}/QtVlcWidgetsConfigVersion.cmake" @ONLY)

install(EXPORT QtVlcWidgetsTargets DESTINATION "${LIB_CMAKE_PATH}" COMPONENT Widgets-dev)
install(FILES ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/QtVlcWidgetsConfig.cmake
			  ${PROJECT_BINARY_DIR}/QtVlcWidgetsConfigVersion.cmake
		DESTINATION "${LIB_CMAKE_PATH}" COMPONENT Widgets-dev)
