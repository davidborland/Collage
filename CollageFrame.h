///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        CollageFrame.h
//
// Author:      David Borland
//
// Description: Frame for Collage application.
//
///////////////////////////////////////////////////////////////////////////////////////////////


#ifndef COLLAGEFRAME_H
#define COLLAGEFRAME_H


#include <RenciFrame.h>


class CollageFrame : public RenciFrame {
public:
    CollageFrame(const wxString& title, const wxSize& size, RenciGraphics* theGraphics, 
                 EnvironmentMode environmentMode = Normal, bool toroidalMode = false);
    virtual ~CollageFrame();

    void OnKey(wxKeyEvent& e);
    void OnMouse(wxMouseEvent& e);

    void ChooseMedia(wxCoord x, wxCoord y);
    void ChooseBackgroundColor(wxCoord x, wxCoord y);

protected:
    DECLARE_EVENT_TABLE()
};


#endif