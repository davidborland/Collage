///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        SimpleDoubleLayoutManager.cpp
//
// Author:      Mike Conway
//
// Description: Lays out images in two rows
//
///////////////////////////////////////////////////////////////////////////////////////////////


#include "SimpleDoubleLayoutManager.h"

#include "CollageGraphics.h"


SimpleDoubleLayoutManager::SimpleDoubleLayoutManager(CollageGraphics* collageGraphics)
: CollageLayoutManager(collageGraphics) {
}

SimpleDoubleLayoutManager::~SimpleDoubleLayoutManager() {
}


void SimpleDoubleLayoutManager::Layout(unsigned int start) {
    std::cout << "SimpleDoubleLayoutManager::Layout() : doing layout" << std::endl;


	const float xBorder = 0.05f; 
	const float yBorder = 0.01f;
	

	std::vector<CollageImage*> images = collageGraphics->GetImages();
	std::vector<CollageImage*> temp;
	for (int i = start; i < (int)images.size(); i++) {
		temp.push_back(images[i]);
	}


	for (int i = start; i < (int)temp.size(); i++) {
		bool matched = false;
		for (int j = i + 1; j < (int)temp.size(); j++) {
			if (temp[i]->GetHeight() + temp[j]->GetHeight() < 1.0 - 3.0 * yBorder) {
				CollageImage* match = temp[j];
				temp.erase(temp.begin() + j);
				temp.insert(temp.begin() + i + 1, match);

				i++;

				break;
			}
		}
	}


	float xPos = xBorder;
	for (int i = start; i < (int)temp.size(); i++) {
		bool matched = false;
		if (i + 1 < (int)temp.size()) {
			CollageImage* image1 = temp[i];
			CollageImage* image2 = temp[i + 1];
			if (image1->GetHeight() + image2->GetHeight() < 1.0 - 3.0 * yBorder) {
				double width1 = image1->GetWidth();
				double width2 = image2->GetWidth();
				double maxWidth = width1 >= width2 ? width1 : width2;

				xPos += maxWidth / 2;

				image1->SetPosition(Vec2(xPos, 0.5 + yBorder * 0.5 + image1->GetHeight() * 0.5));
				image2->SetPosition(Vec2(xPos, 0.5 - yBorder * 0.5 - image2->GetHeight() * 0.5));

				i++;

				xPos += maxWidth / 2 + xBorder;

				matched = true;
			}
		}

		if (!matched) {
			CollageImage* image = temp[i];

			double width = image->GetWidth();

			xPos += width / 2;

			image->SetPosition(Vec2(xPos, 0.5));

			xPos += width / 2 + xBorder;
		}
	}
}