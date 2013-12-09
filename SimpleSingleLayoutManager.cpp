///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        SimpleSingleLayoutManager.cpp
//
// Author:      Mike Conway
//
// Description: Lays out images in a row
//
///////////////////////////////////////////////////////////////////////////////////////////////


#include "SimpleSingleLayoutManager.h"

#include "CollageGraphics.h"


SimpleSingleLayoutManager::SimpleSingleLayoutManager(CollageGraphics* collageGraphics)
: CollageLayoutManager(collageGraphics) {	
}

SimpleSingleLayoutManager::~SimpleSingleLayoutManager() {
}


void SimpleSingleLayoutManager::Layout(unsigned int start) {
    std::cout << "SimpleSingleLayoutManager::Layout() : doing layout" << std::endl;


	const float border = 0.05f;
	float xPos = 0.0;

	std::vector<CollageImage*> images = collageGraphics->GetImages();
	for (int i = start; i < (int)images.size(); i++) {
		if (i > (int)start) xPos += images[i - 1]->GetAspectRatio() * images[i - 1]->GetScale() * 0.5;
		xPos += images[i]->GetAspectRatio() * images[i]->GetScale() * 0.5 + border;
		images[i]->SetPosition(Vec2(xPos, 0.5));
	}
}