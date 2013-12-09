///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        RenciGLCanvas.cpp
//
// Author:      David Borland
//
// Description: OpenGL canvas for use with SCRFrame.
//
///////////////////////////////////////////////////////////////////////////////////////////////


#include "RenciGLCanvas.h"


BEGIN_EVENT_TABLE(RenciGLCanvas, wxGLCanvas)
    EVT_PAINT(RenciGLCanvas::OnPaint)
    EVT_ERASE_BACKGROUND(RenciGLCanvas::OnEraseBackground)
    EVT_CHAR(RenciGLCanvas::OnKey)
    EVT_MOUSE_EVENTS(RenciGLCanvas::OnMouse)
END_EVENT_TABLE()


RenciGLCanvas::RenciGLCanvas(wxWindow *parent, int* attribList, 
                             const wxPoint& pos, const wxSize& size) 
: wxGLCanvas(parent, wxID_ANY, attribList, pos, size) {;
}

RenciGLCanvas::~RenciGLCanvas() {
}


void RenciGLCanvas::OnPaint(wxPaintEvent& e) {
    // Make this the current device context
    wxPaintDC(this);
}

void RenciGLCanvas::OnEraseBackground(wxEraseEvent& e) {
    // Do nothing, to avoid flickering
}

void RenciGLCanvas::OnKey(wxKeyEvent& e) {
    // By default, key presses are not propagated to parents
    // Set the propagation level to 1 to reach the parent, and then skip to induce propagation
    e.ResumePropagation(1);
    e.Skip();
}

void RenciGLCanvas::OnMouse(wxMouseEvent& e) {
    // Need to add the position of the canvas for correct mouse position
    int x, y;
    GetPosition(&x, &y);
    e.m_x += x;
    e.m_y += y;


    // By default, mouse events are not propagated to parents
    // Set the propagation level to 1 to reach the parent, and then skip to induce propagation
    e.ResumePropagation(1);
    e.Skip();
}
