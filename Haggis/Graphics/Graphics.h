///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        Graphics.cpp
//
// Author:      David Borland
//
// Description: Abstract class that should be derived from to implement OpenGL rendering.
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#ifndef GRAPHICS_H
#define GRAPHICS_H


#include "Camera.h"


class Graphics {
public:
    Graphics();
    virtual ~Graphics();

    virtual bool Init();

    void Reshape(int width, int height);

    void SetRotation(const Quat& q);
    void SetTranslation(const Vec3& t);

    Camera* GetCamera();

    typedef enum RenderType {
        LeftRightStereo,
        LeftRightDTIStereo,        
        Dome
    };
    RenderType GetRenderType();
    void SetRenderType(RenderType type);

    bool GetUseRenderType();
    void SetUseRenderType(bool flag);

    virtual void Render();

protected:
    Camera* camera;

    int windowWidth;
    int windowHeight;

    Quat quaternion;
    Vec3 translation;

    RenderType renderType;
    bool useRenderType;

    virtual bool InitGL() = 0;
    virtual void Draw() = 0;
};


#endif