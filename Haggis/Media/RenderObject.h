///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        RenderObject.h
//
// Author:      David Borland
//
// Description: Abstract base class for drawing an object with OpenGL
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H


#include "Vec3.h"
#include "Quat.h"

#include <GL/glew.h>


class RenderObject {
public:    
    RenderObject();
    virtual ~RenderObject();

    virtual void Update();
    virtual void Render();

    virtual void SetPosition(const Vec3& pos);
    virtual void SetQuaternion(const Quat& quat);
    virtual void SetScale(double scaleValue);

    const Vec3& GetPosition() const;
    const Quat& GetQuaternion() const;
    double GetScale() const;

protected:
    Vec3 position;
    Quat quaternion;
    double scale;

    virtual void PreRender() = 0;
    virtual void DoRender() = 0;
    virtual void PostRender() = 0;
};


#endif