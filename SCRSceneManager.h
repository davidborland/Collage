///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        SCRSceneManager.h
//
// Author:      Mike Conway
//
// DeSCRiption: Scene Manager that describes the environment in the Social Computing Room
//              each wall is 3072x768, torioidal, one horizontal band consisting of a display 
//              group per wall, 3 displays per  display group.
//
///////////////////////////////////////////////////////////////////////////////////////////////


#ifndef SCRSCENEMANAGER_H
#define SCRSCENEMANAGER_H


#include "SceneManager.h"


class SCRSceneManager : public SceneManager {
public:
	SCRSceneManager(CollageGraphics* collageGraphics);
	~SCRSceneManager();
};


#endif