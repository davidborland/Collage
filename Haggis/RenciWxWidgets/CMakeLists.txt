PROJECT( RenciWxWidgets )

SET( SRC RenciFrame.h RenciFrame.cpp 
         RenciGLCanvas.h RenciGLCanvas.cpp
         RenciGraphics.h RenciGraphics.cpp
         RenciLog.h RenciLog.cpp )

ADD_LIBRARY( RenciWxWidgets ${SRC} )