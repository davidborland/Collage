///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        RenciFrame.h
//
// Author:      David Borland
//
// Description: An class for a wxWidgets frame to be used in one of the RENCI
//              visualization environments.
//
///////////////////////////////////////////////////////////////////////////////////////////////


#ifndef RENCIFRAME_H
#define RENCIFRAME_H


#include <wx/wx.h>

#include <vector>

#include "RenciGLCanvas.h"
#include "RenciGraphics.h"


class RenciFrame : public wxFrame {
public:
    // Enumeration for different environments
    enum EnvironmentMode {
        Normal,
        SCR,
        TeleImmersion
    };

    RenciFrame(const wxString& title, const wxSize& size, RenciGraphics* theGraphics, 
               EnvironmentMode environmentMode = Normal, bool toroidalMode = false);
    virtual ~RenciFrame();

    // Initialize the frame, with renderInterval given in milliseconds
    virtual bool Initialize(unsigned int renderInterval = 10);

    // Event handlers should not be virtual
    void OnTimer(wxTimerEvent& e);
    void OnKey(wxKeyEvent& e);
    void OnMouse(wxMouseEvent& e);

protected:
    // Created outside this class, but should be destroyed here
    RenciGraphics* graphics;

    // OpenGL canvases and context
    std::vector<RenciGLCanvas*> canvases;
    wxGLContext* context;

    // Behavior flags
    EnvironmentMode environment;
    bool toroidal;

    // Enumeration for the render timer
    enum {
        RenderTimerId
    };
    wxTimer* renderTimer;

    // For controlling the mouse position in x
    int oldMouseX;

    virtual void CreateCanvases();
    virtual void Render();
	
    DECLARE_EVENT_TABLE()
};


#endif