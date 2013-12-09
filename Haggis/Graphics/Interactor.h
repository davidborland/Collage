///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        Interactor.h
//
// Author:      David Borland
//
// Description: Class that implements mouse controls from window/mouse inputs.  Designed to
//              match GLUT.
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#ifndef INTERACTOR_H
#define INTERACTOR_H


#include <Quat.h>
#include <Vec3.h>


class Interactor {
public:
    Interactor();
    ~Interactor();

    void Reshape(int width, int height);

    void LeftButtonEvent(bool buttonDown, int x, int y);
    void RightButtonEvent(bool buttonDown, int x, int y);
    void MiddleButtonEvent(bool buttonDown, int x, int y);
    void MotionEvent(int x, int y);

    void UpdateRotation();

    const Vec3& GetTranslation();
    const Quat& GetRotation();

    void SetTranslation(const Vec3& t); 
    void SetRotation(const Quat& q);

    void SetZoomSensitivity(double sensitivity);
    void SetRotateSensitivity(double sensitivity);

    // Use half window when both stereo views are rendered to the window
    void UseWholeWindow();
    void UseHalfWindow();

protected:
    int windowWidth;
    int windowHeight;

    bool leftButtonDown;
    bool rightButtonDown;
    bool middleButtonDown;

    int xAnchor;
    int yAnchor;

/*
    int oldX;
    int oldY;
*/

    Vec3 translation;
    Quat quat;

    Vec3 oldTranslation;
    Quat oldQuat;

    Quat deltaQuat;
    Quat oldSetQuat;

    bool updateRotation;

    double zoomSensitivity;
    double translationSensitivity;
    double rotateSensitivity;

    bool useHalfWindow;

    void Pan(int x, int y);     
    void Zoom(int x, int y);   
    void Rotate(Quat& quat, int x0, int y0, int x1, int y1);

    double ProjectToSphere(double x, double y, double radius);
};


#endif