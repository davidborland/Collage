///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        FillRoomLayoutManager.cpp
//
// Author:      David Borland
//
// Description: Lays out images to fill the room horizontally
//
///////////////////////////////////////////////////////////////////////////////////////////////


#include "FillRoomLayoutManager.h"

#include "CollageGraphics.h"


FillRoomLayoutManager::FillRoomLayoutManager(CollageGraphics* collageGraphics)
: CollageLayoutManager(collageGraphics) {	
}

FillRoomLayoutManager::~FillRoomLayoutManager() {
}


void FillRoomLayoutManager::Layout(unsigned int start) {
    std::cout << "FillRoomLayoutManager::Layout() : doing layout" << std::endl;

	const float border = 0.05f;
    
	std::vector<CollageImage*> images = collageGraphics->GetImages();

    // Get the current width of all images combined, with borders
    float width = 0.0;
    for (int i = 0; i < (int)images.size(); i++) {
        width += images[i]->GetWidth();
    }

    float scale = (collageGraphics->GetViewWidth() - (int)images.size() * border) / width; 

    for (int i = 0; i < (int) images.size(); i++) {
        images[i]->SetScale(images[i]->GetScale() * scale);
    }

	float xPos = -border / 2.0;
	for (int i = start; i < (int)images.size(); i++) {
		if (i > (int)start) xPos += images[i - 1]->GetWidth() * 0.5;
		xPos += images[i]->GetWidth() * 0.5 + border;
		images[i]->SetPosition(Vec2(xPos, 0.5));
	}
}