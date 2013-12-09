///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        RenciGraphics.cpp
//
// Author:      David Borland
//
// Description: Abstract base class for rendering in one of the RENCI visualization 
//              environments.
//
///////////////////////////////////////////////////////////////////////////////////////////////


#include "RenciGraphics.h"


#include <GL/glew.h>

#include <wx/log.h>


RenciGraphics::RenciGraphics() {
}

RenciGraphics::~RenciGraphics() {
}


bool RenciGraphics::Initialize(unsigned int width, unsigned int height) {
    // Set the window width
    windowWidth = width;
    windowHeight = height;

    // Set size of the screen
    viewWidth = (float)windowWidth / (float)windowHeight;
    viewHeight = 1.0;

    // Initialize OpenGL
    if (!InitGL()) {
        std::cout << "RenciGraphics::Initialize() : OpenGL initialization failed." << std::endl;
        return false;
    }

    return true;
}


void RenciGraphics::Update() {
    // Update
}


void RenciGraphics::PreRender() {
    // Pre-Render
    glClear(GL_COLOR_BUFFER_BIT);
}

void RenciGraphics::Render() {
    // Render 
}

void RenciGraphics::RenderStereo() {
    // Render Stereo
}

void RenciGraphics::PostRender() {
    // Post-Render
}

void RenciGraphics::RenderLeftSCR() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0, viewWidth / 2.0f, 0.0, viewHeight, -1.0, 1.0);

    Render();
}

void RenciGraphics::RenderRightSCR() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(viewWidth / 2.0f, viewWidth, 0.0, viewHeight, -1.0, 1.0);

    Render();
}

void RenciGraphics::RenderLeftTeleImmersion() {
    glViewport(0, 0, windowWidth, windowHeight);

    Render();
}

void RenciGraphics::RenderRightTeleImmersion() {
    glViewport(windowWidth, 0, windowWidth, windowHeight);

    RenderStereo();
}


float RenciGraphics::GetViewWidth() {
    return viewWidth;
}

float RenciGraphics::GetViewHeight() {
    return viewHeight;
}


void RenciGraphics::OnKey(wxKeyEvent& e) {
    // Keyboard handling
}

void RenciGraphics::OnMouse(wxMouseEvent& e) {
    // Mouse handling
}