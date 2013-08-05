# Find System LibVLC.

# We can copy VLC to keep the qt4 interface plugin away.
#OPTION(SYSTEM_VLC_COPY "Copy the system libvlc and it's plugins to guarantee removal of qt4 plugins." OFF)
SET(SYSTEM_VLC_COPY OFF) # Not working yet.

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
	IF(NOT LIBVLC_DLL OR NOT LIBVLCCORE_DLL OR NOT LIBVLC_DLL_A)
		MESSAGE(FATAL_ERROR "Could not find required files : ${LIBVLC_DLL} ${LIBVLCCORE_DLL} ${LIBVLC_DLL_A}")
	ENDIF()
	GET_FILENAME_COMPONENT(LIBVLC_LIBRARY_DIRS ${LIBVLC_DLL_A} PATH)
ENDIF()

IF(SYSTEM_VLC_COPY)
	IF(WIN32)
		MESSAGE(STATUS "    (WIN32) Copying VLC DLLs")
		FILE(COPY "${LIBVLC_DLL}" "${LIBVLCCORE_DLL}" "${VLC_PATH}\\plugins"
			 DESTINATION "${PROJECT_BINARY_DIR}")
		FILE(REMOVE "${PROJECT_BINARY_DIR}\\plugins\\gui\\libqt4_plugin.dll")
		SET(LIBVLC_LIBRARIES ${LIBVLC_DLL_A})
	ELSE()
		MESSAGE(FATAL "    Copying only on windows for now.")
	ENDIF()
ENDIF()

include_directories(${LIBVLC_INCLUDE_DIRS})
link_directories(${LIBVLC_LIBRARY_DIRS})

MESSAGE(STATUS "    found libvlc, version ${LIBVLC_VERSION} (system)")
