#include "SCRSceneManager.h"

SCRSceneManager::SCRSceneManager(CollageGraphics* collageGraphics)
: SceneManager(collageGraphics) {
	padPixelsBetweenDisplays = 10;
	toroidalDisplay = true;
	padForBoundaries = true;
	SceneDisplayGroup* sceneDisplayGroup;
	SceneDisplay* sceneDisplay;
	
	// build the first display group, which is wall (1)
	sceneDisplayGroup = new SceneDisplayGroup(this);
	sceneDisplayGroup->SetGroupName("wall 1");
	

	// wall1 disp1
	sceneDisplay = new SceneDisplay(sceneDisplayGroup);
	sceneDisplay->SetDisplayRole(SceneManager::SM_THUMBNAIL_DISPLAY);
	sceneDisplay->SetXOrigin(0);
	sceneDisplay->SetYOrigin(0);
	sceneDisplay->SetWidthInPixels(1024);
	sceneDisplay->SetHeightInPixels(768);
	sceneDisplayGroup->GetSceneDisplays()->push_back(sceneDisplay);
	
	// wall1 disp2
	sceneDisplay = new SceneDisplay(sceneDisplayGroup);
	sceneDisplay->SetDisplayRole(SceneManager::SM_THUMBNAIL_DISPLAY);
	sceneDisplay->SetXOrigin(1024);
	sceneDisplay->SetYOrigin(0);
	sceneDisplay->SetWidthInPixels(1024);
	sceneDisplay->SetHeightInPixels(768);
	sceneDisplayGroup->GetSceneDisplays()->push_back(sceneDisplay);
	
	// wall1 disp3
	sceneDisplay = new SceneDisplay(sceneDisplayGroup);
	sceneDisplay->SetDisplayRole(SceneManager::SM_THUMBNAIL_DISPLAY);
	sceneDisplay->SetXOrigin(2048);
	sceneDisplay->SetYOrigin(0);
	sceneDisplay->SetWidthInPixels(1024);
	sceneDisplay->SetHeightInPixels(768);
	sceneDisplayGroup->GetSceneDisplays()->push_back(sceneDisplay);

	sceneDisplayGroups.push_back(sceneDisplayGroup);

	sceneDisplayGroup = new SceneDisplayGroup(this);
	sceneDisplayGroup->SetGroupName("wall 2");
	
	// wall2 disp1
	sceneDisplay = new SceneDisplay(sceneDisplayGroup);
	sceneDisplay->SetDisplayRole(SceneManager::SM_THUMBNAIL_DISPLAY);
	sceneDisplay->SetXOrigin(3072);
	sceneDisplay->SetYOrigin(0);
	sceneDisplay->SetWidthInPixels(1024);
	sceneDisplay->SetHeightInPixels(768);
	sceneDisplayGroup->GetSceneDisplays()->push_back(sceneDisplay);
	
	// wall2 disp2
	sceneDisplay = new SceneDisplay(sceneDisplayGroup);
	sceneDisplay->SetDisplayRole(SceneManager::SM_THUMBNAIL_DISPLAY);
	sceneDisplay->SetXOrigin(4096);
	sceneDisplay->SetYOrigin(0);
	sceneDisplay->SetWidthInPixels(1024);
	sceneDisplay->SetHeightInPixels(768);
	sceneDisplayGroup->GetSceneDisplays()->push_back(sceneDisplay);
	
	// wall2 disp3
	sceneDisplay = new SceneDisplay(sceneDisplayGroup);
	sceneDisplay->SetDisplayRole(SceneManager::SM_THUMBNAIL_DISPLAY);
	sceneDisplay->SetXOrigin(5120);
	sceneDisplay->SetYOrigin(0);
	sceneDisplay->SetWidthInPixels(1024);
	sceneDisplay->SetHeightInPixels(768);
	sceneDisplayGroup->GetSceneDisplays()->push_back(sceneDisplay);

	sceneDisplayGroups.push_back(sceneDisplayGroup);

	sceneDisplayGroup = new SceneDisplayGroup(this);
	sceneDisplayGroup->SetGroupName("wall 3");

	// wall3 disp1
	sceneDisplay = new SceneDisplay(sceneDisplayGroup);
	sceneDisplay->SetDisplayRole(SceneManager::SM_THUMBNAIL_DISPLAY);
	sceneDisplay->SetXOrigin(6144);
	sceneDisplay->SetYOrigin(0);
	sceneDisplay->SetWidthInPixels(1024);
	sceneDisplay->SetHeightInPixels(768);
	sceneDisplayGroup->GetSceneDisplays()->push_back(sceneDisplay);
	
	// wall3 disp2
	sceneDisplay = new SceneDisplay(sceneDisplayGroup);
	sceneDisplay->SetDisplayRole(SceneManager::SM_THUMBNAIL_DISPLAY);
	sceneDisplay->SetXOrigin(7168);
	sceneDisplay->SetYOrigin(0);
	sceneDisplay->SetWidthInPixels(1024);
	sceneDisplay->SetHeightInPixels(768);
	sceneDisplayGroup->GetSceneDisplays()->push_back(sceneDisplay);
	
	// wall3 disp3
	sceneDisplay = new SceneDisplay(sceneDisplayGroup);
	sceneDisplay->SetDisplayRole(SceneManager::SM_THUMBNAIL_DISPLAY);
	sceneDisplay->SetXOrigin(8192);
	sceneDisplay->SetYOrigin(0);
	sceneDisplay->SetWidthInPixels(1024);
	sceneDisplay->SetHeightInPixels(768);
	sceneDisplayGroup->GetSceneDisplays()->push_back(sceneDisplay);

	sceneDisplayGroups.push_back(sceneDisplayGroup);

	sceneDisplayGroup = new SceneDisplayGroup(this);
	sceneDisplayGroup->SetGroupName("wall 4");

	// wall4 disp1
	sceneDisplay = new SceneDisplay(sceneDisplayGroup);
	sceneDisplay->SetDisplayRole(SceneManager::SM_THUMBNAIL_DISPLAY);
	sceneDisplay->SetXOrigin(9216);
	sceneDisplay->SetYOrigin(0);
	sceneDisplay->SetWidthInPixels(1024);
	sceneDisplay->SetHeightInPixels(768);
	sceneDisplayGroup->GetSceneDisplays()->push_back(sceneDisplay);
	
	// wall4 disp2
	sceneDisplay = new SceneDisplay(sceneDisplayGroup);
	sceneDisplay->SetDisplayRole(SceneManager::SM_THUMBNAIL_DISPLAY);
	sceneDisplay->SetXOrigin(10240);
	sceneDisplay->SetYOrigin(0);
	sceneDisplay->SetWidthInPixels(1024);
	sceneDisplay->SetHeightInPixels(768);
	sceneDisplayGroup->GetSceneDisplays()->push_back(sceneDisplay);
	
	// wall4 disp3
	sceneDisplay = new SceneDisplay(sceneDisplayGroup);
	sceneDisplay->SetDisplayRole(SceneManager::SM_THUMBNAIL_DISPLAY);
	sceneDisplay->SetXOrigin(11264);
	sceneDisplay->SetYOrigin(0);
	sceneDisplay->SetWidthInPixels(1024);
	sceneDisplay->SetHeightInPixels(768);
	sceneDisplayGroup->GetSceneDisplays()->push_back(sceneDisplay);

	sceneDisplayGroups.push_back(sceneDisplayGroup);

	// take all of the dimensions and compute all aspects of the scene
	CalculateScene();

}

SCRSceneManager::~SCRSceneManager() {
}
