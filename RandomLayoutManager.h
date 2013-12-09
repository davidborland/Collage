///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        RandomLayoutManager.h
//
// Author:      David Borland
//
// Description: Lays out images randomly
//
///////////////////////////////////////////////////////////////////////////////////////////////


#ifndef RANDOMLAYOUTMANAGER_H
#define RANDOMLAYOUTMANAGER_H


#include "CollageLayoutManager.h"


class RandomLayoutManager : public CollageLayoutManager {
public:
	RandomLayoutManager(CollageGraphics* collageGraphics);
	virtual ~RandomLayoutManager();

	virtual void Layout(unsigned int start = 0);
};


#endif
