#include "TeleImmersionSceneManager.h"

TeleImmersionSceneManager::TeleImmersionSceneManager(CollageGraphics* collageGraphics)
: SceneManager(collageGraphics) {
	padPixelsBetweenDisplays = 0;
	toroidalDisplay = false;
	padForBoundaries = false;
	SceneDisplayGroup* sceneDisplayGroup;
	SceneDisplay* sceneDisplay;
	
	// Build the only display group
	sceneDisplayGroup = new SceneDisplayGroup(this);
	sceneDisplayGroup->SetGroupName("wall 1");
	

	// Set the only display
	sceneDisplay = new SceneDisplay(sceneDisplayGroup);
	sceneDisplay->SetDisplayRole(SceneManager::SM_THUMBNAIL_DISPLAY);
	sceneDisplay->SetXOrigin(0);
	sceneDisplay->SetYOrigin(0);
	sceneDisplay->SetWidthInPixels(1920 * 2);
	sceneDisplay->SetHeightInPixels(1080 * 2);
	sceneDisplayGroup->GetSceneDisplays()->push_back(sceneDisplay);

    sceneDisplayGroups.push_back(sceneDisplayGroup);


	// Take all of the dimensions and compute all aspects of the scene
	CalculateScene();

}

TeleImmersionSceneManager::~TeleImmersionSceneManager() {
}
