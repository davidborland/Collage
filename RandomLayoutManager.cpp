///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        RandomLayoutManager.cpp
//
// Author:      David Borland
//
// Description: Lays out images randomly
//
///////////////////////////////////////////////////////////////////////////////////////////////


#include "RandomLayoutManager.h"

#include "CollageGraphics.h"


RandomLayoutManager::RandomLayoutManager(CollageGraphics* collageGraphics)
: CollageLayoutManager(collageGraphics) {
}

RandomLayoutManager::~RandomLayoutManager() {
}


void RandomLayoutManager::Layout(unsigned int start) {
    std::cout << "RandomLayoutManager::Layout() : doing layout" << std::endl;

 
    std::vector<CollageImage*> images = collageGraphics->GetImages();

    float w = collageGraphics->GetViewWidth();
    float h = collageGraphics->GetViewHeight();

    float scale = w * h / (float)images.size();

    for (int i = 0; i < (int)images.size(); i++) {
        images[i]->SetScale(scale);
    }
    
    for (int i = start; i < (int)images.size(); i++) {
        float x = (float)rand() / (float)RAND_MAX * (w - images[i]->GetWidth()) + images[i]->GetWidth() * 0.5;
        float y = (float)rand() / (float)RAND_MAX * (h - images[i]->GetHeight()) + images[i]->GetHeight() * 0.5;
        images[i]->SetPosition(Vec2(x, y));
    }
}