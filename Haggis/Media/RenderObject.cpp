///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        RenderObject.cpp
//
// Author:      David Borland
//
// Description: Abstract base class for drawing an object with OpenGL
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#include "RenderObject.h"


   
RenderObject::RenderObject() {
    scale = 1.0;
}

RenderObject::~RenderObject() {
}


void RenderObject::Update() {
}

void RenderObject::Render() {
    PreRender();
    DoRender();
    PostRender();
}


void RenderObject::SetPosition(const Vec3& pos) {
    position = pos;
}

void RenderObject::SetQuaternion(const Quat& quat) {
    quaternion = quat;
}

void RenderObject::SetScale(double scaleValue) {
    scale = scaleValue;
}


const Vec3& RenderObject::GetPosition() const {
    return position;
}

const Quat& RenderObject::GetQuaternion() const {
    return quaternion;
}

double RenderObject::GetScale() const {
    return scale;
}
