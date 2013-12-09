///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        TeleImmersionSceneManager.h
//
// Author:      Mike Conway
//
// DeTeleImmersioniption: Scene Manager that describes the environment in the 
//                        TeleImmersion environment
//
///////////////////////////////////////////////////////////////////////////////////////////////


#ifndef TELEIMMERSIONSCENEMANAGER_H
#define TELEIMMERSIONSCENEMANAGER_H


#include "SceneManager.h"


class TeleImmersionSceneManager : public SceneManager {
public:
	TeleImmersionSceneManager(CollageGraphics* collageGraphics);
	~TeleImmersionSceneManager();
};


#endif