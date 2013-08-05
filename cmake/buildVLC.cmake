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
