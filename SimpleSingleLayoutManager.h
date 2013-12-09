///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        SimpleSingleLayoutManager.h
//
// Author:      Mike Conway
//
// Description: Lays out images in a row
//
///////////////////////////////////////////////////////////////////////////////////////////////


#ifndef SIMPLESINGLELAYOUTMANAGER_H
#define SIMPLESINGLELAYOUTMANAGER_H


#include "CollageLayoutManager.h"


class SimpleSingleLayoutManager : public CollageLayoutManager {
public:
	SimpleSingleLayoutManager(CollageGraphics* collageGraphics);
    virtual ~SimpleSingleLayoutManager();

	virtual void Layout(unsigned int start = 0);
};


#endif
