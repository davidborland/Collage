CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )


PROJECT( Haggis )

OPTION( CMAKE_VERBOSE_MAKEFILE  "Enable/Disable verbose compiler output" ON )
OPTION( CMAKE_COLOR_MAKEFILE "Enable/Disable color cues when building" ON )
MARK_AS_ADVANCED( CLEAR CMAKE_VERBOSE_MAKEFILE CMAKE_COLOR_MAKEFILE )


#######################################
# Include glut
#######################################

FIND_PATH( GLUT_DIR glut32.lib )

INCLUDE_DIRECTORIES( ${GLUT_DIR} )
LINK_DIRECTORIES( ${GLUT_DIR} )

SET( GLUT_LIB glut32.lib)


#######################################
# Include glew
#######################################

FIND_PATH( GLEW_ROOT_DIR include/GL/glew.h )

INCLUDE_DIRECTORIES( ${GLEW_ROOT_DIR}/include )
LINK_DIRECTORIES( ${GLEW_ROOT_DIR}/lib )

SET( GLEW_LIB glew32.lib)


#######################################
# Include bass
#######################################

FIND_PATH( BASS_ROOT_DIR c/bass.h )

INCLUDE_DIRECTORIES( ${BASS_ROOT_DIR}/c )
LINK_DIRECTORIES( ${BASS_ROOT_DIR}/c )

SET( BASS_LIB bass.lib)


#######################################
# Include DevIL
#######################################

FIND_PATH( DEVIL_ROOT_DIR include/IL/il.h )

INCLUDE_DIRECTORIES( ${DEVIL_ROOT_DIR}/include )
LINK_DIRECTORIES( ${DEVIL_ROOT_DIR}/lib )

SET( DEVIL_LIB DevIL.lib)


#######################################
# Include Windows SDK
#######################################

FIND_PATH( WINDOWS_SDK_DIR Include/DShow.h )

INCLUDE_DIRECTORIES( ${WINDOWS_SDK_DIR}/Include )


#######################################
# Include DirectX
#######################################

FIND_PATH( DIRECTX_DIR Include/ddraw.h )

INCLUDE_DIRECTORIES( ${DIRECTX_DIR}/Include )


#######################################
# Include wxWidgets
#######################################

INCLUDE( LocalUsewxWidgets.cmake )
INCLUDE( ${CMAKE_ROOT}/Modules/UsewxWidgets.cmake )
INCLUDE_DIRECTORIES( ${wxWidgets_INCLUDES} )
LINK_DIRECTORIES( ${wxWidgets_LIBRARY_DIRS} )



ADD_SUBDIRECTORY( Graphics       )
ADD_SUBDIRECTORY( Media          )
ADD_SUBDIRECTORY( Quat           )
ADD_SUBDIRECTORY( RenciWxWidgets )
ADD_SUBDIRECTORY( Utilities      ) 
