///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        RenciGLCanvas.h
//
// Author:      David Borland
//
// Description: OpenGL canvas for use with RenciFrame.
//
///////////////////////////////////////////////////////////////////////////////////////////////


#ifndef RENCIGLCANVAS_H
#define RENCIGLCANVAS_H


#include <wx/wx.h>

// This needs to be included before glcanvas.h
#include <GL/glew.h>

#include <wx/glcanvas.h>


class RenciGLCanvas : public wxGLCanvas {
public:    
    RenciGLCanvas(wxWindow *parent, int* attribList, 
                  const wxPoint& pos, const wxSize& size);
    virtual ~RenciGLCanvas();

    // Event handlers should not be virtual
    void OnPaint(wxPaintEvent& e);
    void OnEraseBackground(wxEraseEvent& e);
    void OnKey(wxKeyEvent& e);
    void OnMouse(wxMouseEvent& e);

protected:
    DECLARE_EVENT_TABLE()
};


#endif