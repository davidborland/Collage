///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        CollageLayoutManager.h
//
// Author:      Mike Conway
//
// Description: Top level class that defines a layout manager, arranging content on the 
//              display in some manner
//
///////////////////////////////////////////////////////////////////////////////////////////////


#ifndef COLLAGELAYOUTMANAGER_H
#define COLLAGELAYOUTMANAGER_H

#include "SceneManager.h"
//#include "Vec2.h"

// Forward declarations
class CollageGraphics;


class CollageLayoutManager {
public:
	CollageLayoutManager(CollageGraphics* collageGraphics);
	virtual ~CollageLayoutManager();
	virtual void Layout(unsigned int start = 0) = 0;

protected:
	CollageGraphics* collageGraphics;
	SceneManager* sceneManager;
};


class CollageLayoutManagerException {
public:
	CollageLayoutManagerException(std::string message);
	virtual ~CollageLayoutManagerException();
	std::string GetMessage();

protected:
	std::string message;
};



#endif
