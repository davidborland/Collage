#include "SceneManager.h"
#include "CollageGraphics.h"

///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        SceneManager.h
//
// Author:      Mike Conway
//
// Description: Top level class that defines a scene manager.  The scene manager is in charge of describing the display environment,
// including corners, stitch lines, and aspects of the display that can effect the display of images, such as thumbnail walls and magnification walls.
//
// This class is differentiated from the CollageLayoutManager, which lays out content within the parameters set by the SceneManager.
///////////////////////////////////////////////////////////////////////////////////////////////

// scenemanager constructor intitializes values
// params:  collageGraphics - handle to the CollageGraphics object
SceneManager::SceneManager(CollageGraphics* collageGraphics)
{
	padPixelsBetweenDisplays = 0;
	toroidalDisplay = false;
	padForBoundaries = false;
	calculated = false;
	this->collageGraphics = collageGraphics;
	availableSingleDisplayHeight = 0;
	availableSingleDisplayWidth = 0;
	totalDisplayWidth = 0;
	totalDisplayHeight = 0;
}

// get the vector of SceneDisplayGroups, these are groups collections of physical displays (a row of monitors or projectors)
std::vector<SceneDisplayGroup*> SceneManager::GetSceneDisplayGroups(void) {
	return sceneDisplayGroups;
}

// SceneManager destructor will clean up displays and display groups
SceneManager::~SceneManager(void)
{
	for (int i = 0; i < (int)sceneDisplayGroups.size(); i++) {
		delete sceneDisplayGroups[i];
	}

}

// after configuring all displays into groups, invoke this method to calculate the dimensions and attributes
// of the scene
void SceneManager::CalculateScene(void) {

	if (sceneDisplayGroups.size() == 0) {
		throw SceneManagerException("no display groups in scene manager");
	}

	// for each scene group, calculate the dimensions based on the individual displays
	for (int i = 0; i < (int)sceneDisplayGroups.size(); i++) {
		sceneDisplayGroups[i]->CalculateDisplayGroup();
	}
	// check a display to get the available width and height

	if (sceneDisplayGroups.size() == 0) {
		throw SceneManagerException("no display groups when calculating scene");
	} else if (sceneDisplayGroups[0]->GetSceneDisplays()->size() == 0) {
		throw SceneManagerException("no displays in display group when calculating scene");
	} else {
		this->availableSingleDisplayWidth = sceneDisplayGroups[0]->GetSceneDisplays()->at(0)->GetAvailableWidthInPixels();
		this->availableSingleDisplayHeight = sceneDisplayGroups[0]->GetSceneDisplays()->at(0)->GetAvailableHeightInPixels();
	}

	// TODO:  this is stub code contrived to calculate the total display width and height, it's really hard coded
	// for the scr dimensions, but could be enhanced to calculate other display configurations - MCC
	totalDisplayWidth = 12288;
	totalDisplayHeight = 768;

	calculated = true;
}

bool SceneManager::IsCalculated(void) {
	return this->calculated;
}

// get the number of available displays that fit a specified display role.  
unsigned int SceneManager::GetNumberOfDisplaysByRole(SceneManager::DisplayRole displayRole) {
	unsigned int count = 0;

	for (int i = 0; i < (int)sceneDisplayGroups.size(); i++) {
		for (int j = 0; j < (int)sceneDisplayGroups[i]->GetSceneDisplays()->size(); j++) {

			if (sceneDisplayGroups[i]->GetSceneDisplays()->at(j)->GetDisplayRole() == displayRole) {
				count++;
			}
		}
	}

	return count;
}

// get the effetive width of a display (all displays are assumed to be of the same dimension), accounting for 
// padding if that is specified.
unsigned int SceneManager::GetAvailableSingleDisplayWidth(void) {
	return availableSingleDisplayWidth;
}

// get the effetive height of a display (all displays are assumed to be of the same dimension), accounting for 
// padding if that is specified.
unsigned int SceneManager::GetAvailableSingleDisplayHeight(void) {
	return availableSingleDisplayHeight;
}


// get the total width of all displays 
unsigned int SceneManager::GetTotalDisplayWidth(void) {
	return totalDisplayWidth;
}

// get the total height of all displays 
unsigned int SceneManager::GetTotalDisplayHeight(void) {
	return totalDisplayHeight;
}


// accessor to number of pixles to pad between displays (e.g. a stitch line buffer or corner)
unsigned int SceneManager::GetPadPixelsBetweenDisplays(void) {
	return this->padPixelsBetweenDisplays;
}

// accessor indicates whether to pad for the boundaries
bool SceneManager::IsPadForBoundaries(void) {
	return this->padForBoundaries;
}


/////////////////////////////////////////////////////////////
// SceneDisplayGroup members - a group of related displays (a row or surface)
/////////////////////////////////////////////////////////////

SceneDisplayGroup::SceneDisplayGroup(SceneManager* sceneManager)
{
	this->sceneManager = sceneManager;
	//widthInPixels = 0;
	//heightInPixels = 0;
	xOrigin = 0;
	yOrigin = 0;
	xThru = 0;
	yThru = 0;
	groupName = "display group";
}

// SceneDisplayGroups destructor will clean SceneDisplays
SceneDisplayGroup::~SceneDisplayGroup(void)
{
	for (int i = 0; i < (int)sceneDisplays.size(); i++) {
		delete sceneDisplays[i];
	}
}


// Set the displays in this group to a certain display role
void SceneDisplayGroup::SetGroupToDisplayRole(SceneManager::DisplayRole displayRole){
	for (int i = 0; i < (int)sceneDisplays.size(); i++) {
		sceneDisplays[i]->SetDisplayRole(displayRole);
	}
}

// Setter for a displayable name for this group
void SceneDisplayGroup::SetGroupName(std::string groupName) {
	this->groupName = groupName;
}


// Bounce thru the display groups and calculate their stuff
void SceneDisplayGroup::CalculateDisplayGroup(void) {
	if (sceneDisplays.size() == 0) {
		throw SceneManagerException("no displays in display group");
	}

	for (int i = 0; i < (int)sceneDisplays.size(); i++) {
		sceneDisplays[i]->CalculateSceneDisplay();
	}
	// set the bounds for this scene group based on the constituent displays
	xOrigin = sceneDisplays.front()->GetXOrigin();
	yOrigin = sceneDisplays.front()->GetYOrigin();
	xThru = sceneDisplays.back()->GetXThru();
	yThru = sceneDisplays.back()->GetYThru();
}

std::vector<SceneDisplay*>* SceneDisplayGroup::GetSceneDisplays(void) {
	return &sceneDisplays;
}


// accessor for the x origin of the display (upper left is 0,0)
unsigned int SceneDisplayGroup::GetXOrigin(void) {
	return xOrigin;
}

// accessor for the y origin of the display (upper left is 0,0)
unsigned int SceneDisplayGroup::GetYOrigin(void) {
	return yOrigin;
}


// accessor for the last x pixel displayed
unsigned int SceneDisplayGroup::GetXThru(void) {
	return xThru;
}

// accessor for the last y pixel displayed
unsigned int SceneDisplayGroup::GetYThru(void) {
	return yThru;
}

SceneManager* SceneDisplayGroup::GetSceneManager(void) {
	return sceneManager;
}

/////////////////////////////////////////////////////////////
// SceneDisplay members - a projector or display that makes up a group
/////////////////////////////////////////////////////////////

//Scene Display constructor, initialze variables to defaults
SceneDisplay::SceneDisplay(SceneDisplayGroup* sceneDisplayGroup) {
	this->sceneDisplayGroup = sceneDisplayGroup;
	widthInPixels=0;
	heightInPixels=0;
	xOrigin=0;
	yOrigin=0;
	xOriginInScale = 0.0;
	yOriginInScale = 0.0;
	xThru=0;
	yThru=0;
	displayRole=SceneManager::SM_THUMBNAIL_DISPLAY;
}

// Scene Display destructor
SceneDisplay::~SceneDisplay(void) {
}

// calculate dimensions for this display, otehr settings
void SceneDisplay::CalculateSceneDisplay(void) {
	// calculate the absolute x,y boundaries based on the origin and dimensions

	xThru = xOrigin + widthInPixels - 1;
	yThru = yOrigin + heightInPixels - 1;
	if (xThru == 0 || yThru == 0) {
		throw SceneManagerException("cannot have a zero width display");
	}
}

// accessor for the SceneDisplayGroup that this SceneDisplay is in
SceneDisplayGroup* SceneDisplay::GetSceneDisplayGroup(void) {
	return sceneDisplayGroup;
}

void SceneDisplay::SetWidthInPixels(unsigned int widthInPixels) {
	this->widthInPixels = widthInPixels;
}
void SceneDisplay::SetHeightInPixels(unsigned int heightInPixels) {
	this->heightInPixels = heightInPixels;
}
void SceneDisplay::SetXOrigin(unsigned int xOrigin) {
	this->xOrigin = xOrigin;
}

void SceneDisplay::SetYOrigin(unsigned int yOrigin) {
	this->yOrigin = yOrigin;
}

void SceneDisplay::SetDisplayRole(SceneManager::DisplayRole displayRole) {
	this->displayRole = displayRole;
}

// accessor for the x origin of the display (upper left is 0,0)
unsigned int SceneDisplay::GetXOrigin(void) {
	return xOrigin;
}

// accessor for the x origin of the display (upper left is 0,0) with padding adjusted
unsigned int SceneDisplay::GetPaddedXOrigin(void) {
	if (sceneDisplayGroup->GetSceneManager()->IsPadForBoundaries()) {
		return xOrigin + sceneDisplayGroup->GetSceneManager()->GetPadPixelsBetweenDisplays();
	} else {
		return xOrigin;
	}

}


// accessor for the y origin of the display (upper left is 0,0) with padding adjusted
unsigned int SceneDisplay::GetPaddedYOrigin(void) {
	if (sceneDisplayGroup->GetSceneManager()->IsPadForBoundaries()) {
		return yOrigin + sceneDisplayGroup->GetSceneManager()->GetPadPixelsBetweenDisplays();
	} else {
		return yOrigin;
	}

}


// accessor for the scaled (versus pixels) x origin of the display (upper left is 0,0) with padding adjusted
double SceneDisplay::GetPaddedXOriginInScale(void) {
	if (sceneDisplayGroup->GetSceneManager()->IsPadForBoundaries()) {
		return (xOrigin + sceneDisplayGroup->GetSceneManager()->GetPadPixelsBetweenDisplays()) / sceneDisplayGroup->GetSceneManager()->GetTotalDisplayWidth();
	} else {
		return xOrigin / sceneDisplayGroup->GetSceneManager()->GetTotalDisplayWidth();
	}

}


// accessor for the scaled (versus pixels) y origin of the display (upper left is 0,0) with padding adjusted
double SceneDisplay::GetPaddedYOriginInScale(void) {
	if (sceneDisplayGroup->GetSceneManager()->IsPadForBoundaries()) {
		return (yOrigin + sceneDisplayGroup->GetSceneManager()->GetPadPixelsBetweenDisplays()) / sceneDisplayGroup->GetSceneManager()->GetTotalDisplayHeight();
	} else {
		return yOrigin / sceneDisplayGroup->GetSceneManager()->GetTotalDisplayHeight();
	}

}

// accessor for the scaled (versus pixels) x origin of the display (upper left is 0,0)
double SceneDisplay::GetXOriginInScale(void) {

	return xOrigin / sceneDisplayGroup->GetSceneManager()->GetTotalDisplayWidth();

}

// accessor for the scaled (versus pixels) y origin of the display (upper left is 0,0)
double SceneDisplay::GetYOriginInScale(void) {

	return yOrigin / sceneDisplayGroup->GetSceneManager()->GetTotalDisplayHeight();

}

// accessor for the y origin of the display (upper left is 0,0)
unsigned int SceneDisplay::GetYOrigin(void) {
	return yOrigin;
}


// accessor for the last x pixel displayed
unsigned int SceneDisplay::GetXThru(void) {
	return xThru;
}

// accessor for the last y pixel displayed
unsigned int SceneDisplay::GetYThru(void) {
	return yThru;
}

// accessor for the display role
SceneManager::DisplayRole SceneDisplay::GetDisplayRole(void) {
	return displayRole;
}

// accessor returns width of this display in pixels
unsigned int SceneDisplay::GetWidthInPixels(void) {
	return widthInPixels;
}

// accessor returns height of this display in pixels
unsigned int SceneDisplay::GetHeightInPixels(void) {
	return heightInPixels;
}

// accessor for the available width, taking into account any padding
unsigned int SceneDisplay::GetAvailableWidthInPixels(void) {
	unsigned int retWidth = 0;
	if (sceneDisplayGroup->GetSceneManager()->IsPadForBoundaries()) {
		retWidth = widthInPixels - (sceneDisplayGroup->GetSceneManager()->GetPadPixelsBetweenDisplays() * 2);
	} else {
		retWidth = widthInPixels;
	} 
	return retWidth;
}

// accessor for the available height, taking into account any padding
unsigned int SceneDisplay::GetAvailableHeightInPixels(void) {
	unsigned int retHeight = 0;
	if (sceneDisplayGroup->GetSceneManager()->IsPadForBoundaries()) {
		retHeight = heightInPixels - (sceneDisplayGroup->GetSceneManager()->GetPadPixelsBetweenDisplays() * 2);
	} else {
		retHeight = heightInPixels;
	} 
	return retHeight;
}

// compute the x,y center of this display
//	return:  Vec2D with the x,y coordinates of the absolute display center
Vec2 SceneDisplay::GetDisplayCenter(void) {
	double x = xOrigin + widthInPixels * .5;
	double y = yOrigin + heightInPixels * .5;
	return Vec2(x,y);
}


/////////////////////////////////////////////////////////////
// SceneManagerException - general exeption for scene manager processing
/////////////////////////////////////////////////////////////

SceneManagerException::SceneManagerException(std::string message) {
	this->message = message;
}

SceneManagerException::~SceneManagerException(void) {
}

std::string SceneManagerException::GetMessage(void) {
	return this->message;
}



