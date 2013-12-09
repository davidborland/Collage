///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        Camera.h
//
// Author:      David Borland
//
// Description: OpenGL camera class.
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#ifndef CAMERA_H
#define CAMERA_H


#include <Vec3.h>
#include <Quat.h>


class Camera {
public:
    Camera();
    ~Camera();

    void Reshape(int width, int height);

    double GetFocalDistance() const;
    void SetFocalDistance(double distance);

    double GetEyeSeparation();
    void SetEyeSeparation(double separation);

    void SetPosition(const Vec3& viewPosition);
    void SetLookAt(const Vec3& viewLookAt);
    void SetUp(const Vec3& viewUp);

    Vec3 GetPosition();
    Vec3 GetLookAt();
    Vec3 GetUp();

    void SetView(double pitch = 0.0, double yaw = 0.0, double domePitch = 0.0);
    void SetLeftView();
    void SetRightView();

    void SetFieldOfView(double fov);
    double GetFieldOfView();

    void SetAspectRatio(double aspect);
    double GetAspectRatio();

protected:
    int windowWidth;
    int windowHeight;

    Vec3 position;
    Vec3 lookAt;
    Vec3 up;

    double aspectRatio;
    double fieldOfView;
    double halfWidth;
    double nearDistance;
    double farDistance;
    double focalDistance;
    double eyeSeparation;
};


#endif

