PROJECT( Graphics )

SET( SRC Camera.h Camera.cpp
         GLSLShader.h GLSLShader.cpp
         Graphics.h Graphics.cpp
         Interactor.h Interactor.cpp ) 

INCLUDE_DIRECTORIES( ${Haggis_SOURCE_DIR}/Quat )

ADD_LIBRARY( Graphics ${SRC} )
TARGET_LINK_LIBRARIES( Graphics ${GLUT_LIB} ${GLEW_LIB} )