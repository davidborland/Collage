PROJECT( Media )

SET( SRC AudioStream.h AudioStream.cpp
         Image.h Image.cpp
         OBJObject.h OBJObject.cpp
         OBJObjectAO.h OBJObjectAO.cpp
         PerlinNoise.h PerlinNoise.cpp
         RenderObject.h RenderObject.cpp
         VideoCapture.h VideoCapture.cpp
         VideoFile.h VideoFile.cpp
         VideoStream.h VideoStream.cpp )
         
INCLUDE_DIRECTORIES( ${Haggis_SOURCE_DIR}/Quat 
                     ${Haggis_SOURCE_DIR}/Utilities )

ADD_LIBRARY( Media ${SRC} )
TARGET_LINK_LIBRARIES( Media ${BASS_LIB} )