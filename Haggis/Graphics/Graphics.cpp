///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        Graphics.cpp
//
// Author:      David Borland
//
// Description: Abstract class that should be derived from to implement OpenGL rendering.
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#include "Graphics.h"

#include <GL/glew.h>


Graphics::Graphics() {
    camera = new Camera();
    camera->SetPosition(Vec3(0, 0, 2));

    windowWidth = 1;
    windowHeight = 1;

    renderType = LeftRightStereo;
    useRenderType = false;
}

Graphics::~Graphics() {
    delete camera;
}


bool Graphics::Init() {
    // Initialize Glew for checking OpenGL extensions
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cout << "Graphics::Initialize() : " << glewGetErrorString(err) << "\n";
        return false;
    }

    return InitGL();
}


void Graphics::Reshape(int width, int height) {
    windowWidth = width;
    windowHeight = height;

    if (useRenderType && renderType == LeftRightStereo) {
        camera->Reshape(windowWidth / 2, windowHeight);
    }
    else {
        camera->Reshape(windowWidth, windowHeight);
    }
}


void Graphics::SetTranslation(const Vec3& t) {
    translation = t;

//    camera->Pan(Vec2(translation.X(), translation.Y()));
//    camera->Zoom(translation.Z());
}

void Graphics::SetRotation(const Quat& q) {
    quaternion = q;

//    camera->Rotate(quaternion);
}


Camera* Graphics::GetCamera() {
    return camera;
}


Graphics::RenderType Graphics::GetRenderType() {
    return renderType;
}

void Graphics::SetRenderType(RenderType type) {
    renderType = type;
}

bool Graphics::GetUseRenderType() {
    return useRenderType;
}

void Graphics::SetUseRenderType(bool flag) {
    useRenderType = flag;

    if (renderType == LeftRightStereo) {
        if (useRenderType) {
            camera->Reshape(windowWidth / 2, windowHeight);
        }
        return;
    }  
    camera->Reshape(windowWidth, windowHeight);
}

void Graphics::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (useRenderType) {
        if (renderType == LeftRightStereo) {
            // Left Eye
            glViewport(0, 0, windowWidth / 2, windowHeight);
            camera->SetLeftView();
            Draw();

            // Right Eye 
            glViewport(windowWidth / 2, 0, windowWidth / 2, windowHeight);
            camera->SetRightView();
            Draw();
        }
        else if (renderType == LeftRightDTIStereo) {            
            // Left Eye
            glViewport(0, 0, windowWidth / 2, windowHeight);
            camera->SetLeftView();
            Draw();

            // Right Eye 
            glViewport(windowWidth / 2, 0, windowWidth / 2, windowHeight);
            camera->SetRightView();
            Draw();
        }
        else if (renderType == Dome) {
            camera->SetFieldOfView(86.0);
            camera->SetAspectRatio(1.58985199);

            // Channel 1
            glViewport(0, 0, windowWidth / 2, windowHeight / 2);
            camera->SetView(-52.5, -45.0, 90.0);
            Draw();

            // Channel 2
            glViewport(windowWidth / 2, 0, windowWidth / 2, windowHeight / 2);
            camera->SetView(-52.5, 45.0, 90.0);
            Draw();

            // Channel 3
            glViewport(0, windowHeight / 2, windowWidth / 2, windowHeight / 2);
            camera->SetView(-52.5, -135.0, 90.0);
            Draw();

            // Channel 4
            glViewport(windowWidth / 2, windowHeight / 2, windowWidth / 2, windowHeight / 2);
            camera->SetView(-52.5, 135.0, 90.0);
            Draw();
        }
    }
    else {
        glViewport(0, 0, windowWidth, windowHeight);
        camera->SetView();
        Draw();
    }
}