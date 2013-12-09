///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        RenciGraphics.h
//
// Author:      David Borland
//
// Description: Abstract base class for rendering in one of the RENCI visualization 
//              environments.
//
///////////////////////////////////////////////////////////////////////////////////////////////


#ifndef RENCIGRAPHICS_H
#define RENCIGRAPHICS_H


#include <wx/wx.h>

// This needs to be included before glcanvas.h
#include <GL/glew.h>

#include <wx/glcanvas.h>


class RenciGraphics {
public:
    RenciGraphics();
    virtual ~RenciGraphics();

    // Initialize
    virtual bool Initialize(unsigned int width, unsigned int height);

    // Get required render attributes
    virtual int* GetAttribList() = 0;

    // Update state
    virtual void Update();

    // Render the scene
    virtual void PreRender();
    virtual void Render();
    virtual void RenderStereo();
    virtual void PostRender();

    void RenderLeftSCR();
    void RenderRightSCR();
    void RenderLeftTeleImmersion();
    void RenderRightTeleImmersion();

    float GetViewWidth();
    float GetViewHeight();

    // This is not a wxWidgets class, but we can resuse their events
    virtual void OnKey(wxKeyEvent& e);
    virtual void OnMouse(wxMouseEvent& e);

protected:
    unsigned int windowWidth;
    unsigned int windowHeight;

    float viewWidth;
    float viewHeight;

    virtual bool InitGL() = 0;
};


#endif