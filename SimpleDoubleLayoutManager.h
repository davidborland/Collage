///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        SimpleDoubleLayoutManager.h
//
// Author:      Mike Conway
//
// Description: Lays out images in two rows
//
///////////////////////////////////////////////////////////////////////////////////////////////


#ifndef SIMPLEDOUBLELAYOUTMANAGER_H
#define SIMPLEDOUBLELAYOUTMANAGER_H


#include "CollageLayoutManager.h"


class SimpleDoubleLayoutManager : public CollageLayoutManager {
public:
	SimpleDoubleLayoutManager(CollageGraphics* collageGraphics);
	virtual ~SimpleDoubleLayoutManager();

	virtual void Layout(unsigned int start = 0);
};


#endif

