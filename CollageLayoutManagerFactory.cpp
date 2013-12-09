///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        CollageLayoutManagerFactory.cpp
//
// Author:      Mike Conway
//
// Description: Factory to create appropriate layout manager that 
//              subclasses CollageLayoutManager
//
///////////////////////////////////////////////////////////////////////////////////////////////


#include "CollageLayoutManagerFactory.h"

#include <iostream>


CollageLayoutManagerFactory::CollageLayoutManagerFactory(CollageGraphics* collageGraphics) {
	this->collageGraphics = collageGraphics;
}

CollageLayoutManagerFactory::~CollageLayoutManagerFactory() {
}


CollageLayoutManager* CollageLayoutManagerFactory::CreateLayoutManager(CollageLayoutManagerFactory::LayoutType layoutType) {
	CollageLayoutManager* selectedLayout = NULL;

	switch (layoutType) {
	case CollageLayoutManagerFactory::LM_SIMPLE_SINGLE:
		selectedLayout = CreateSimpleSingleLayoutManager();
		break;
	case CollageLayoutManagerFactory::LM_SIMPLE_DOUBLE:
		selectedLayout = CreateSimpleDoubleLayoutManager();
		break;
	case CollageLayoutManagerFactory::LM_SMART_SINGLE:
		selectedLayout = CreateSmartSingleLayoutManager();
		break;
    case CollageLayoutManagerFactory::LM_RANDOM:
        selectedLayout = CreateRandomLayoutManager();
        break;
    case CollageLayoutManagerFactory::LM_FILL_ROOM:
        selectedLayout = CreateFillRoomLayoutManager();
        break;
	default:
		selectedLayout = CreateSimpleSingleLayoutManager();
	}

	return selectedLayout;
}

// Factory method returns a simple, single-row layout
CollageLayoutManager* CollageLayoutManagerFactory::CreateSimpleSingleLayoutManager() {
	return  new SimpleSingleLayoutManager(collageGraphics);
}

// Factory method returns a simple, double-row layout
CollageLayoutManager* CollageLayoutManagerFactory::CreateSimpleDoubleLayoutManager() {
	return  new SimpleDoubleLayoutManager(collageGraphics);
}

// Factory method returns a smart, single-row layout
CollageLayoutManager* CollageLayoutManagerFactory::CreateSmartSingleLayoutManager() {
	return  new SmartSingleLayoutManager(collageGraphics);
}

// Factory method returns a random layout
CollageLayoutManager* CollageLayoutManagerFactory::CreateRandomLayoutManager() {
	return  new RandomLayoutManager(collageGraphics);
}

// Factory method returns a layout that fills the room horizontally
CollageLayoutManager* CollageLayoutManagerFactory::CreateFillRoomLayoutManager() {
	return  new FillRoomLayoutManager(collageGraphics);
}