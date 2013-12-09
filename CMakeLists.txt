CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )

PROJECT( Collage )

OPTION( CMAKE_VERBOSE_MAKEFILE  "Enable/Disable verbose compiler output" ON )
OPTION( CMAKE_COLOR_MAKEFILE "Enable/Disable color cues when building" ON )
MARK_AS_ADVANCED( CLEAR CMAKE_VERBOSE_MAKEFILE CMAKE_COLOR_MAKEFILE )


#######################################
# Include wxWidgets
#######################################

INCLUDE( LocalUsewxWidgets.cmake )
INCLUDE( ${CMAKE_ROOT}/Modules/UsewxWidgets.cmake )
INCLUDE_DIRECTORIES( ${wxWidgets_INCLUDES} )
LINK_DIRECTORIES( ${wxWidgets_LIBRARY_DIRS} )


#######################################
# Include glew
#######################################

FIND_PATH( GLEW_ROOT_DIR include/GL/glew.h )

INCLUDE_DIRECTORIES( ${GLEW_ROOT_DIR}/include )
LINK_DIRECTORIES( ${GLEW_ROOT_DIR}/lib )

SET( GLEW_LIB glew32.lib)



#######################################
# Include Haggis
#######################################

FIND_PATH( HAGGIS_SRC_DIR SCR/SCRFrame.h )
FIND_PATH( HAGGIS_BIN_DIR Haggis.sln )

INCLUDE_DIRECTORIES( ${HAGGIS_SRC_DIR}/Media 
                     ${HAGGIS_SRC_DIR}/Quat   
                     ${HAGGIS_SRC_DIR}/RenciWxWidgets )
LINK_DIRECTORIES( ${HAGGIS_BIN_DIR}/Media
                  ${HAGGIS_BIN_DIR}/Quat
                  ${HAGGIS_BIN_DIR}/RenciWxWidgets )

SET( HAGGIS_LIBS Media.lib Quat.lib RenciWxWidgets.lib)


#######################################
# Include Windows SDK
#######################################

FIND_PATH( WINDOWS_SDK_DIR Include/DShow.h )

INCLUDE_DIRECTORIES( ${WINDOWS_SDK_DIR}/Include )
LINK_DIRECTORIES( ${WINDOWS_SDK_DIR}/Lib )

SET( WINDOWS_SDK_LIBS strmiids.lib )


#######################################
# Include DirectX
#######################################

FIND_PATH( DIRECTX_DIR Include/ddraw.h )

INCLUDE_DIRECTORIES( ${DIRECTX_DIR}/Include )

#######################################
# Include FTGL and FreeType font libraries
#######################################


#INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR}/include )
#LINK_DIRECTORIES( ${PROJECT_SOURCE_DIR}/lib )

#SET( FTGL_LIBS freetype2110MT_D.lib ftgl.lib)


#######################################
# Include Collage code
#######################################

SET( SRC Collage.h Collage.cpp
         CollageFrame.h CollageFrame.cpp
         CollageGraphics.h CollageGraphics.cpp
         CollageImage.h CollageImage.cpp 
         CollageItemMetadata.h
         CollageLayoutManager.h	CollageLayoutManager.cpp
         CollageLayoutManagerFactory.h CollageLayoutManagerFactory.cpp
         FillRoomLayoutManager.h FillRoomLayoutManager.cpp
         RandomLayoutManager.h RandomLayoutManager.cpp
         SimpleDoubleLayoutManager.h SimpleDoubleLayoutManager.cpp
         SimpleSingleLayoutManager.h SimpleSingleLayoutManager.cpp
         SmartSingleLayoutManager.h SmartSingleLayoutManager.cpp
         SCRSceneManager.h SCRSceneManager.cpp
         SceneManager.h SceneManager.cpp 
         TeleImmersionSceneManager.h TeleImmersionSceneManager.cpp )
ADD_EXECUTABLE( Collage WIN32 MACOSX_BUNDLE ${SRC} )
TARGET_LINK_LIBRARIES( Collage ${VTK_LIBS} ${GLEW_LIB} ${HAGGIS_LIBS} ${WINDOWS_SDK_LIBS} )