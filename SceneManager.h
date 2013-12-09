///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        SceneManager.h
//
// Author:      Mike Conway
//
// Description: Top level class that defines a scene manager.  The scene manager is in 
//              charge of describing the display environment, including corners, stitch lines, 
//              and aspects of the display that can effect the display of images, such as 
//              thumbnail walls and magnification walls.
//
//              SceneManager also defines a SceneDisplayGroup, which controls a group of 
//              SceneDisplays.  Consider an individual projector in a display wall as a 
//              SceneDisplay, while a functional group of projectors makes a SceneDisplayGroup.
//
///////////////////////////////////////////////////////////////////////////////////////////////


#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H


#include <string>
#include <vector>
#include <iterator>
#include <Vec2.h>


// Forward declarations
class CollageGraphics;
class SceneDisplayGroup;
class SceneDisplay;


class SceneManager {
public:
	// Constructor takes a handle to a CollageGraphics
	enum DisplayRole  {
        SM_THUMBNAIL_DISPLAY,
        SM_MAGNIFICATION_DISPLAY
    };

	SceneManager(CollageGraphics* collageGraphics);
	virtual ~SceneManager(void);

	std::vector<SceneDisplayGroup*> GetSceneDisplayGroups(void);

	bool IsToroidalDisplay(void);
	bool IsPadForBoundaries(void);
	bool IsCalculated(void);

	unsigned int GetPadPixelsBetweenDisplays(void);
	SceneManager::DisplayRole GetRoleByCollision(unsigned int originX, unsigned int originY, unsigned int width, unsigned int height);
	SceneManager::DisplayRole GetRoleByCentroid(unsigned int centroidX, unsigned int centroidY);

	void CalculateScene(void);

	unsigned int GetNumberOfDisplaysByRole(SceneManager::DisplayRole);
	unsigned int GetAvailableSingleDisplayWidth(void);
	unsigned int GetAvailableSingleDisplayHeight(void);
	unsigned int GetTotalDisplayWidth(void);
	unsigned int GetTotalDisplayHeight(void);

protected:
	CollageGraphics* collageGraphics;

	std::vector<SceneDisplayGroup*> sceneDisplayGroups;

	bool toroidalDisplay;
	bool padForBoundaries;
	bool calculated;

	unsigned int padPixelsBetweenDisplays;
	unsigned int availableSingleDisplayWidth;
	unsigned int availableSingleDisplayHeight;
	unsigned int totalDisplayWidth;
	unsigned int totalDisplayHeight;
};


// SceneDisplayGroup is a functional group of displays, such as a wall in the SCR
class SceneDisplayGroup {
public: 
	SceneDisplayGroup(SceneManager* sceneManager);
	virtual ~SceneDisplayGroup(void);

	unsigned int GetXOrigin(void);
	unsigned int GetYOrigin(void);

	std::vector<SceneDisplay*>* GetSceneDisplays(void);
	std::string GetGroupName(void);

	void SetGroupName(std::string);
	void SetGroupToDisplayRole(SceneManager::DisplayRole displayRole);

	void CalculateDisplayGroup(void);

	unsigned int GetXThru(void);
	unsigned int GetYThru(void);

	SceneManager* GetSceneManager(void);

protected:
	std::vector<SceneDisplay*> sceneDisplays;
	std::string groupName;

	SceneManager* sceneManager;

	unsigned int xOrigin;
	unsigned int yOrigin;
	unsigned int yThru;
	unsigned int xThru;
};


// SceneDisplay is one physical display in a group, such as one projector in the SCR belonging to a particular
// SceneDisplayGroup, e.g. a wall in the SCR
class SceneDisplay {
public: 
	SceneDisplay(SceneDisplayGroup* sceneDisplayGroup);
	virtual ~SceneDisplay(void);

	void SetWidthInPixels(unsigned int widthInPixels);
	void SetHeightInPixels(unsigned int heightInPixels);

	unsigned int GetAvailableWidthInPixels(void);
	unsigned int GetAvailableHeightInPixels(void);
	unsigned int GetWidthInPixels(void);
	unsigned int GetHeightInPixels(void);

	void SetXOrigin(unsigned int xOrigin);
	void SetYOrigin(unsigned int yOrigin);
	unsigned int GetXOrigin(void);
	unsigned int GetYOrigin(void);
	double GetXOriginInScale(void);
	double GetYOriginInScale(void);
	unsigned int GetXThru(void);
	unsigned int GetYThru(void);

	void SetDisplayRole(SceneManager::DisplayRole displayRole);
	SceneManager::DisplayRole GetDisplayRole(void);
	SceneDisplayGroup* GetSceneDisplayGroup(void);
	void CalculateSceneDisplay(void);

	unsigned int GetPaddedXOrigin(void);
	unsigned int GetPaddedYOrigin(void);
	double GetPaddedXOriginInScale(void);
	double GetPaddedYOriginInScale(void);

	Vec2 GetDisplayCenter(void);
	
protected:
	unsigned int widthInPixels;
	unsigned int heightInPixels;
	unsigned int xOrigin;
	unsigned int yOrigin;
	double xOriginInScale;
	double yOriginInScale;
	
	unsigned int yThru;
	unsigned int xThru;
	SceneManager::DisplayRole displayRole;
	SceneDisplayGroup* sceneDisplayGroup;
};


class SceneManagerException {
public:
	SceneManagerException(std::string message);
	virtual ~SceneManagerException(void);
	std::string GetMessage(void);

protected:
	std::string message;
};


#endif