///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        CollageImage.h
//
// Author:      David Borland
//
// Description: Includes Image behavior for Collage
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#ifndef COLLAGEIMAGE_H
#define COLLAGEIMAGE_H


#include <Image.h>
#include "CollageItemMetadata.h"
//#include <FTGL/ftgl.h>


// Forward declarations
class CollageGraphics;



class CollageImage : public Image {
public:    
    CollageImage(Behavior imageBehavior = Normal);
    virtual ~CollageImage();

    void SetWindowHeight(int height);

    virtual void SetPosition(const Vec2& pos);
    virtual void SetScale(float scaleValue);

    bool Intersect(const Vec2& point);

    void Translate(const Vec2& translation);

    void FitToScreen();
    void NativeResolution();

    void GetExtent(float& left, float& right, float& bottom, float& top);
	CollageItemMetadata* GetCollageItemMetadata(void);

    void BorderOn();
    void BorderOff();
	void SetCollageGraphics(CollageGraphics* collageGraphics);
	CollageGraphics* GetCollageGraphics(void);
	void ShowLegendText(float viewXStart, float translateX, float translateY);

    void SetStereoImage(CollageImage* image);
    bool HasStereoImage();
    CollageImage* RemoveStereoImage();

    void IncreaseStereoDepth();
    void DecreaseStereoDepth();

    void RenderStereo();

protected:
    bool border;

    int windowHeight;

	CollageGraphics* collageGraphics;
	// TODO: add bool showMetadata and support

	CollageItemMetadata collageItemMetadata;

    CollageImage* stereoImage;

    float stereoOffset;

    virtual void DoRender();
//	void ShowLegend(float translateX, float translateY); 
    void RenderBorder();
    void SetStereoOffset(float offset);
};


#endif