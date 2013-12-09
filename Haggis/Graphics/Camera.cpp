///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        Camera.cpp
//
// Author:      David Borland
//
// Description: OpenGL camera class.
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#include "Camera.h"

#include <glut.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>


Camera::Camera() {
    windowWidth = 1;
    windowHeight = 1;

    position.Set(0.0, 0.0, 1.0);
    lookAt.Set(0.0, 0.0, 0.0);
    up.Set(0.0, 1.0, 0.0);

    aspectRatio = (double)windowWidth / (double)windowHeight;

    fieldOfView = 45.0;

    nearDistance = 0.01;
    farDistance = 100.0;
    focalDistance = 5.0;
    halfWidth = nearDistance * tan(fieldOfView * M_PI / 180.0 / 2.0);
    eyeSeparation = focalDistance / 30.0;
}

Camera::~Camera() {
}


void Camera::Reshape(int width, int height) {
    windowWidth = width;
    windowHeight = height;

    // Parameters
    aspectRatio = (double)windowWidth / (double)windowHeight;
    halfWidth = nearDistance * tan(fieldOfView * M_PI / 180.0 / 2.0);
}


double Camera::GetFocalDistance() const {
    return focalDistance;
}

void Camera::SetFocalDistance(double distance) {
//    focalDistance = distance < 0.0 ? 0.0 : distance;
    focalDistance = distance;
    printf("focalDistance = %f\n", focalDistance);
}


double Camera::GetEyeSeparation() {
    return eyeSeparation;
}

void Camera::SetEyeSeparation(double separation) {
//    eyeSeparation = separation < 0.0 ? 0.0 : separation;
    eyeSeparation = separation;
    printf("eyeSeparation = %f\n", eyeSeparation);
}


void Camera::SetPosition(const Vec3& viewPosition) {
    position = viewPosition;
}

void Camera::SetLookAt(const Vec3& viewLookAt) {
    lookAt = viewLookAt;
}

void Camera::SetUp(const Vec3& viewUp) {
    up = viewUp;
}


Vec3 Camera::GetPosition() {
    return position;
}

Vec3 Camera::GetLookAt() {
    return lookAt;
}

Vec3 Camera::GetUp() {
    return up;
}


void Camera::SetView(double pitch, double yaw, double domePitch) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(fieldOfView, aspectRatio, nearDistance, farDistance);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(pitch, 1, 0, 0);
    glRotatef(yaw, 0, 1, 0);
    glRotatef(domePitch, 1, 0, 0);

    gluLookAt(position.X(), position.Y(), position.Z(),
              lookAt.X(), lookAt.Y(), lookAt.Z(),
              up.X(), up.Y(), up.Z());
}

void Camera::SetLeftView() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    double left = -aspectRatio * halfWidth + 0.5 * eyeSeparation * nearDistance / focalDistance;
    double right = aspectRatio * halfWidth + 0.5 * eyeSeparation * nearDistance / focalDistance;
    double top = halfWidth;
    double bottom = -halfWidth;

    glFrustum(left, right, bottom, top, nearDistance, farDistance);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(position.X() - eyeSeparation / 2.0, position.Y(), position.Z(), 
              lookAt.X(), lookAt.Y(), lookAt.Z(), 
              up.X(), up.Y(), up.Z());
}

void Camera::SetRightView() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    double left = -aspectRatio * halfWidth - 0.5 * eyeSeparation * nearDistance / focalDistance;
    double right = aspectRatio * halfWidth - 0.5 * eyeSeparation * nearDistance / focalDistance;
    double top = halfWidth;
    double bottom = -halfWidth;

    glFrustum(left, right, bottom, top, nearDistance, farDistance);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(position.X() + eyeSeparation / 2.0, position.Y(), position.Z(), 
              lookAt.X(), lookAt.Y(), lookAt.Z(), 
              up.X(), up.Y(), up.Z());
}


void Camera::SetFieldOfView(double fov) {
    fieldOfView = fov;
}

double Camera::GetFieldOfView() {
    return fieldOfView;
}

void Camera::SetAspectRatio(double aspect) {
    aspectRatio = aspect;
}

double Camera::GetAspectRatio() {
    return aspectRatio;
}