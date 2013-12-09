PROJECT( Quat )

SET( SRC Quat.h Quat.cpp
         Vec2.h Vec2.cpp
         Vec3.h Vec3.cpp )

ADD_LIBRARY( Quat ${SRC} )