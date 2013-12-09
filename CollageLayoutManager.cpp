///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        CollageLayoutManager.cpp
//
// Author:      Mike Conway
//
// Description: Top level abstract class that defines a layout manager, 
//				arranging content on the display in some manner
//
///////////////////////////////////////////////////////////////////////////////////////////////


#include "CollageLayoutManager.h"
#include "CollageGraphics.h"

// default constructor takes a collageGraphics object
CollageLayoutManager::CollageLayoutManager(CollageGraphics* collageGraphics) {
	this->collageGraphics = collageGraphics;
	// use collageGraphics to obtain a handle to the SceneManager, which provides layouts with
	// information describing the display environment
	this->sceneManager = this->collageGraphics->GetSceneManager();
}

CollageLayoutManager::~CollageLayoutManager() {
}




/////////////////////////////////////////////////////////////
// CollageLayoutManagerException - general exeption for layout manager processing
/////////////////////////////////////////////////////////////

CollageLayoutManagerException::CollageLayoutManagerException(std::string message) {
	this->message = message;
}

CollageLayoutManagerException::~CollageLayoutManagerException() {
}

std::string CollageLayoutManagerException::GetMessage() {
	return this->message;
}
