///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        CollageLayoutManagerFactory.h
//
// Author:      Mike Conway
//
// Description: Factory to create appropriate layout manager that 
//              subclasses CollageLayoutManager
//
///////////////////////////////////////////////////////////////////////////////////////////////


#ifndef COLLAGELAYOUTMANAGERFACTORY_H
#define COLLAGELAYOUTMANAGERFACTORY_H


#include "CollageLayoutManager.h"
#include "SimpleSingleLayoutManager.h"
#include "SmartSingleLayoutManager.h"
#include "SimpleDoubleLayoutManager.h"
#include "RandomLayoutManager.h"
#include "FillRoomLayoutManager.h"


class CollageLayoutManagerFactory {
public:
	CollageLayoutManagerFactory(CollageGraphics* collageGraphics);
	~CollageLayoutManagerFactory();
    
    enum LayoutType {
        LM_SIMPLE_SINGLE,
        LM_SIMPLE_DOUBLE,
        LM_SMART_SINGLE,
        LM_RANDOM,
        LM_FILL_ROOM
    };

	CollageLayoutManager* CreateLayoutManager(LayoutType layoutType);

protected:
	CollageGraphics* collageGraphics;

	CollageLayoutManager* CreateSimpleSingleLayoutManager();
	CollageLayoutManager* CreateSmartSingleLayoutManager();
	CollageLayoutManager* CreateSimpleDoubleLayoutManager();
    CollageLayoutManager* CreateRandomLayoutManager();
    CollageLayoutManager* CreateFillRoomLayoutManager();
};


#endif