///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        FillRoomLayoutManager.h
//
// Author:      David Borland
//
// Description: Lays out images to fill the room horizontally
//
///////////////////////////////////////////////////////////////////////////////////////////////


#ifndef FILLROOMLAYOUTMANAGER_H
#define FILLROOMLAYOUTMANAGER_H


#include "CollageLayoutManager.h"


class FillRoomLayoutManager : public CollageLayoutManager {
public:
	FillRoomLayoutManager(CollageGraphics* collageGraphics);
    virtual ~FillRoomLayoutManager();

	virtual void Layout(unsigned int start = 0);
};


#endif
