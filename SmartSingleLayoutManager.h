///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        SmartSingleLayoutManager.h
//
// Author:      Mike Conway
//
// Description: Lays out images in a row
//
///////////////////////////////////////////////////////////////////////////////////////////////


#ifndef SMARTSINGLELAYOUTMANAGER_H
#define SMARTSINGLELAYOUTMANAGER_H


#include "CollageLayoutManager.h"


class SmartSingleLayoutManager : public CollageLayoutManager {
public:
	SmartSingleLayoutManager(CollageGraphics* collageGraphics);
	virtual ~SmartSingleLayoutManager(void);
	virtual void Layout(unsigned int start = 0);
protected:
	double GetDisplayFittingImageScale(unsigned int imagesPerDisplay, double imageAspectRatio, SceneDisplay* sceneDisplay);
	int GetImagesPerDisplay(const int imageCount, const int numberOfDisplays);
};




#endif
