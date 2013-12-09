///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        SmartSingleLayoutManager.cpp
//
// Author:      Mike Conway
//
// Description: Lays out images in a row
//
///////////////////////////////////////////////////////////////////////////////////////////////


#include "SmartSingleLayoutManager.h"

#include "CollageGraphics.h"
#include <cmath>



SmartSingleLayoutManager::SmartSingleLayoutManager(CollageGraphics* collageGraphics) : CollageLayoutManager(collageGraphics) {	
}

SmartSingleLayoutManager::~SmartSingleLayoutManager() {
}


void SmartSingleLayoutManager::Layout(unsigned int start) {
	std::cout << "SmartSingleLayoutManager::Layout() : doing layout" << std::endl;

	// if the scene was not properly initialized, then this is an error.  if this test is true, then it can be assumed
	// that the scene is rationally configured.

	if (!sceneManager->IsCalculated()) {
		throw SceneManagerException("scene was not calculated, be sure to call CalculateScene() after initialization of SceneManager!");
	}

	// phase I (planning)
	// find # of available thumbnail walls
	unsigned int numberThumbnailDisplays = sceneManager->GetNumberOfDisplaysByRole(SceneManager::SM_THUMBNAIL_DISPLAY);
	unsigned int thumbnailDisplayWidth = sceneManager->GetAvailableSingleDisplayWidth();
	unsigned int thumbnailDisplayHeight = sceneManager->GetAvailableSingleDisplayHeight();

	// look at the number of images, see what scale they need to be to be fit on one display (respecting stitch lines), options will be
	// - 1 image per
	// - 2 image per
	// - 4 image per
	// based on image size and number of available displays.  How to handle overflow is an open question ('more' and paging, scrollbars, or wrapping?)

	unsigned int imageCount = this->collageGraphics->GetImages().size();

	// if no images, or no thumbnails to display on, then jump out

	if (imageCount == 0) {
		return;
	}

	if (numberThumbnailDisplays == 0) {
		throw SceneManagerException("no thumbnail displays to lay data out on");
	}

	unsigned int imagesPerDisplay = (unsigned int) floor((float) imageCount / (float) numberThumbnailDisplays);
	// at least one image per display!, otherwise it will be 1, 2, or 4
	if (imagesPerDisplay == 0) {
		imagesPerDisplay = 1;
	} else if (imagesPerDisplay == 3) {
		imagesPerDisplay = 4;
	} else if (imagesPerDisplay > 4) {
		// more than four images per display, will need logic to to wrap, or page...
		// TODO: need to work this out, make it four for now
		imagesPerDisplay = 4;
	}

	unsigned int displayGroupI = 0;
	unsigned int displayI = 0;
	unsigned int imagesPlacedOnDisplay = 0;
	double xAnchorPos = 0.0;
	double yAnchorPos = 0.0;
	double scaleValue = 0.0;
	//bool firstImagePlaced = false;
	bool thumbnailFound = false;

	// spin to first thumbnail image, if I can't find one, something is wrong

	while (!thumbnailFound) {
		if (sceneManager->GetSceneDisplayGroups().at(displayGroupI)->GetSceneDisplays()->at(displayI)->GetDisplayRole() == SceneManager::SM_THUMBNAIL_DISPLAY) {
			thumbnailFound = true;
		} else {
			displayI++;
			if (displayI == sceneManager->GetSceneDisplayGroups().at(displayGroupI)->GetSceneDisplays()->size()) {
				displayI = 0;
				displayGroupI++;
				if (displayGroupI >= sceneManager->GetSceneDisplayGroups().size()) {
					throw SceneManagerException("I can't find a thumbnail display");
				}
			}

		}
	} // end while

	// I should be positioned at the first thumbnail display

	// phase II (fitting)
	// go thru each image and set scale and position, assigning to each available slot in the available thumbnail displays
	for (int i = start; i < (int)collageGraphics-> GetImages().size(); i++) {


		imagesPlacedOnDisplay++;
		// if I've filled up this display, look for the next thumbnail display
		if (imagesPlacedOnDisplay <= imagesPerDisplay) {
			// I'm ok, add the image at the current display
		} else {
			// spin to the next thumbnail image, if I can't find one, something is wrong
			thumbnailFound = false;
			while (!thumbnailFound) {
				displayI++;
				if (displayI == sceneManager->GetSceneDisplayGroups().at(displayGroupI)->GetSceneDisplays()->size()) {
					displayI = 0;
					displayGroupI++;
					if (displayGroupI >= sceneManager->GetSceneDisplayGroups().size()) {
						throw SceneManagerException("I can't find a thumbnail display");
					} else {
						imagesPlacedOnDisplay = 1;
					}
				}
				if (sceneManager->GetSceneDisplayGroups().at(displayGroupI)->GetSceneDisplays()->at(displayI)->GetDisplayRole() == SceneManager::SM_THUMBNAIL_DISPLAY) {
					thumbnailFound = true;
				}	

			} // end while, thumbnail found
		}

		// i will be pointing to the correct 'SceneDisplay' which will provide the correct x/y offsets for that display, all
		// calcs for positioning within the display will be relative to these offsets


		// slot this image into the display, and size it

		if (imagesPerDisplay == 1) {
			//   1 per, place the image
			//xAnchorPos += collageGraphics->GetImages().at(i)->GetAspectRatio() * collageGraphics->GetImages().at(i)->GetScale() * 0.5;
			
			// scale the image to fill the frame
			//	
			scaleValue = GetDisplayFittingImageScale(imagesPerDisplay,
				collageGraphics->GetImages().at(i)->GetAspectRatio(),	
				sceneManager->GetSceneDisplayGroups().at(displayGroupI)->GetSceneDisplays()->at(displayI));
			collageGraphics->GetImages().at(i)->SetScale(scaleValue); 

			// x position
			xAnchorPos = (double) sceneManager->GetSceneDisplayGroups().at(displayGroupI)->GetSceneDisplays()->at(displayI)->GetXOrigin();
			xAnchorPos = xAnchorPos + (sceneManager->GetSceneDisplayGroups().at(displayGroupI)->GetSceneDisplays()->at(displayI)->GetWidthInPixels() * 0.5);
			// change from pixels to scale
			xAnchorPos = xAnchorPos / sceneManager->GetSceneDisplayGroups().at(displayGroupI)->GetSceneDisplays()->at(displayI)->GetHeightInPixels();

			// yPosition
			yAnchorPos = (double) sceneManager->GetSceneDisplayGroups().at(displayGroupI)->GetSceneDisplays()->at(displayI)->GetYOrigin();
			yAnchorPos = yAnchorPos +  (sceneManager->GetSceneDisplayGroups().at(displayGroupI)->GetSceneDisplays()->at(displayI)->GetHeightInPixels() * 0.5);
			// change from pixels to scale
			yAnchorPos = yAnchorPos / (sceneManager->GetTotalDisplayHeight());

			collageGraphics->GetImages().at(i)->SetPosition(Vec2(xAnchorPos, yAnchorPos));
			
		} else if (imagesPerDisplay == 2) {

			//	 2 per, size to 1/2 the display, add the 'offset factor' to the second images position

		} else {

			//	 4 per, size to 1/4 the display, add the 'offset factor' to each images position (1-4)

		}

		// at the end, the image x,y and size will be in the images vector for the next round of rendering


	} // end of for loop




	/*
	const float border = 0.05f;
	float xPos = 0.0;

	std::vector<CollageImage*> images = collageGraphics->GetImages();
	for (int i = start; i < (int)images.size(); i++) {
	if (i > (int)start) 
	{
	xPos += images[i - 1]->GetAspectRatio() * images[i - 1]->GetScale() * 0.5;
	}
	xPos += images[i]->GetAspectRatio() * images[i]->GetScale() * 0.5 + border;
	images[i]->SetPosition(Vec2(xPos, 0.5));
	}
	*/
}

int SmartSingleLayoutManager::GetImagesPerDisplay(const int imageCount, const int numberOfDisplays) {
	int imagesPer = 0;

	return imagesPer;
}


// return the scale value that fits an image into the available display area, taking specified
// padding into account
//	return: double containing the scale value 
//  params: imagesPerDisplay - unsigned int containing the number of images per display (currently accepts values of 1, 2, 4
//			imageAspectRatio - the aspect ratio (width/height) of the image to be scaled
double SmartSingleLayoutManager::GetDisplayFittingImageScale(unsigned int imagesPerDisplay, double imageAspectRatio, SceneDisplay* sceneDisplay) {
	double imageScale = 0.0;
	if (imagesPerDisplay == 1 || imagesPerDisplay==2 || imagesPerDisplay==4) {
		// we're ok
	} else {
		throw CollageLayoutManagerException("invalid parameter, cannot fit this number of images, can only fit 1, 2, or 4 per display");
	}

	if (imagesPerDisplay == 1) {
		// one image, scale to available width and height


		sceneDisplay->GetAvailableHeightInPixels();
		// first, get the factor for width that accounts for any padding ?????
		imageScale = (double) sceneDisplay->GetAvailableWidthInPixels() / ((double) sceneDisplay->GetHeightInPixels() * (double) imageAspectRatio);
		// scale = avail width / (height * ar)

		

	} else if (imagesPerDisplay == 2) {
	} else if (imagesPerDisplay == 4) {
	}

	return imageScale;
}
