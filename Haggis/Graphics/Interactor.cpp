///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        Interactor.cpp
//
// Author:      David Borland
//
// Description: Class that implements rotate/zoom/translate controls from window/mouse inputs.
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#include "Interactor.h"

#include <iostream>


Interactor::Interactor() {
    windowWidth = 1;
    windowHeight = 1;

    leftButtonDown = false;
    rightButtonDown = false;
    middleButtonDown = false;

    xAnchor = 0;
    yAnchor = 0;

    // Initial pose
    SetTranslation(Vec3(0.0, 0.0, 0.0));
    Quat q;
    SetRotation(q);
    oldQuat = quat;
    oldTranslation = translation;

    updateRotation = true;

    zoomSensitivity = 0.01;
    translationSensitivity = 0.001;
    rotateSensitivity = 2.0;

    useHalfWindow = false;
}

Interactor::~Interactor() {
}
    

void Interactor::Reshape(int width, int height) {
    windowWidth = useHalfWindow ? width / 2 : width;
    windowHeight = height;
}


void Interactor::LeftButtonEvent(bool buttonDown, int x, int y) {
    leftButtonDown = buttonDown;

    if (leftButtonDown) {
        xAnchor = x;
        yAnchor = y;  

        oldQuat = quat;
        deltaQuat.MakeIdentity();
    }
}

void Interactor::RightButtonEvent(bool buttonDown, int x, int y) {
    rightButtonDown = buttonDown;

    if (rightButtonDown) {
        xAnchor = x;
        yAnchor = y;
        
        oldTranslation = translation;
    }
}

void Interactor::MiddleButtonEvent(bool buttonDown, int x, int y) {
    middleButtonDown = buttonDown;

    if (middleButtonDown) {
        xAnchor = x;
        yAnchor = y;

        oldTranslation = translation;
    }
}


void Interactor::MotionEvent(int x, int y) {
    if (leftButtonDown) {
        // Store for delta quat
        deltaQuat = quat;

        // Rotate
        Rotate(quat, xAnchor, yAnchor, x, y);

        // Compose with previous rotation
// XXX : CHANGE THIS FOR OBJECT VS. CAMERA
        quat = quat * oldQuat;
        quat.Normalize();

        // Compute delta quat
        deltaQuat = !deltaQuat;
        deltaQuat = quat * deltaQuat;
        deltaQuat.Normalize();
    }
    else if (rightButtonDown) {
        // Zoom
        Zoom(x, y);
    }
    else if (middleButtonDown) {
        // Pan
        Pan(x, y);
    }
}


void Interactor::UpdateRotation() {
    if (!leftButtonDown && updateRotation) {
        // Keep rotating
        quat = deltaQuat * quat;
        quat.Normalize();
    } 
}

const Quat& Interactor::GetRotation() {
    return quat;
}

const Vec3& Interactor::GetTranslation() {
    return translation;
}


void Interactor::SetRotation(const Quat& q) {
    // Store for delta quat
    deltaQuat = oldSetQuat;

    // Set quat
    quat = q;

    oldSetQuat = quat;

    // Compute delta quat
    deltaQuat = !deltaQuat;
    deltaQuat = quat * deltaQuat;
    deltaQuat.Normalize();
}

void Interactor::SetTranslation(const Vec3& t) {
    translation = t;
}


void Interactor::SetZoomSensitivity(double sensitivity) {
    zoomSensitivity = sensitivity;
}

void Interactor::SetRotateSensitivity(double sensitivity) {
    rotateSensitivity = sensitivity;
}

void Interactor::UseHalfWindow() {
    if (!useHalfWindow) {
        windowWidth /= 2;
        useHalfWindow = true;
    }
}

void Interactor::UseWholeWindow() {
    if (useHalfWindow) {
        windowWidth *= 2;
        useHalfWindow = false;
    }
}


void Interactor::Rotate(Quat& q, int x0, int y0, int x1, int y1) {
    // Normalize to window coordinates   
    Vec3 v0(x0, y0, 0.0);

    if (useHalfWindow) {
        v0.X() = (int)v0.X() % windowWidth;
    }

    v0.X() -= windowWidth / 2.0;
    v0.Y() -= windowHeight / 2.0;
    v0.X() *= -2.0 / windowWidth;
    v0.Y() *= -2.0 / windowHeight;

    Vec3 v1(x1, y1, 0.0);

    if (useHalfWindow) {
        v1.X() = (int)v1.X() % windowWidth;
    }

    v1.X() -= windowWidth / 2.0;
    v1.Y() -= windowHeight / 2.0;
    v1.X() *= -2.0 / windowWidth;
    v1.Y() *= -2.0 / windowHeight;


    // Project to sphere
    double tbSize = 1.0;
    v1.Z() = ProjectToSphere(v1.X(), v1.Y(), tbSize);
    v0.Z() = ProjectToSphere(v0.X(), v0.Y(), tbSize);

    // Get the cross product
    Vec3 cross = v1 * v0;

    // Get the rotation
    Vec3 sub = v0 - v1;

    double val = sub.Magnitude() / (2.0 * tbSize);

    val = val > 1.0 ? 1.0 : val;
    val = val < -1.0 ? -1.0 : val;

    double theta = 2.0 * asin(val) * rotateSensitivity;

    // Create the quaternion

// XXX : CHANGE THIS FOR OBJECT VS. CAMERA
    cross.X() *= -1.0;
//    cross.Y() *= -1.0;
    q.Set(theta, cross);
}


void Interactor::Pan(int x, int y) {
    int dx = xAnchor - x;
    int dy = yAnchor - y;

// XXX : CHANGE THIS FOR OBJECT VS. CAMERA
    translation.X() = (double)-dx * translationSensitivity;
    translation.Y() = (double)dy * translationSensitivity;

    translation.X() += oldTranslation.X();
    translation.Y() += oldTranslation.Y();
}

void Interactor::Zoom(int x, int y) {
    int dy = y - yAnchor;

    translation.Z() = dy * zoomSensitivity;
    translation.Z() += oldTranslation.Z();

    translation.Z() = translation.Z() > 20.0 ? 20.0 : translation.Z();
    translation.Z() = translation.Z() < -50.0 ? -50.0 : translation.Z();
}

double Interactor::ProjectToSphere(double x, double y, double radius) {
    double d, t, z;

    d = sqrt(x * x + y * y);

    if (d < radius * 0.70710678118654752440) {
        // Inside sphere
        z = sqrt(radius * radius - d * d);
    } 
    else {           
        // On hyperbola
        t = radius / 1.41421356237309504880;
        z = t * t / d;
    }
    return z;
}