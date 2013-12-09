///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        CollageFrame.h
//
// Author:      David Borland
//
// DeRenciiption: Frame for Collage application.
//
///////////////////////////////////////////////////////////////////////////////////////////////


#include "CollageFrame.h"

#include "CollageGraphics.h"

#include <wx/colordlg.h>

#include <string>
#include <vector>


BEGIN_EVENT_TABLE(CollageFrame, RenciFrame)
    EVT_CHAR(CollageFrame::OnKey)
    EVT_MOUSE_EVENTS(CollageFrame::OnMouse)
END_EVENT_TABLE()


CollageFrame::CollageFrame(const wxString& title, const wxSize& size, RenciGraphics* theGraphics, 
                           EnvironmentMode environmentMode, bool toroidalMode)
: RenciFrame(title, size, theGraphics, environmentMode, toroidalMode) {
}

 CollageFrame::~CollageFrame() {
}


void CollageFrame::OnKey(wxKeyEvent& e) {
    int c = e.GetKeyCode();
    if (c == 'q') {
        // Quit
// XXX : Using Close(), the log window doesn't always close...
//        Close(true);
        exit(0);
    }
    else if (c == 'o') {
        // Open media
        ChooseMedia(e.GetX(), e.GetY());
    }
    else if (c == 'b') {
        // Select background color
        ChooseBackgroundColor(e.GetX(), e.GetY());
    }
    else {
        graphics->OnKey(e);
    }
}


void CollageFrame::OnMouse(wxMouseEvent& e) {
    // Call the base class method to get mouse wraparound
    RenciFrame::OnMouse(e);

    // Send to the graphics
    graphics->OnMouse(e);
}


void CollageFrame::ChooseMedia(wxCoord x, wxCoord y) {
    // Dialogs can be created on the stack
    wxFileDialog fileDialog(this, "Open files", "", "", 
"All supported media files|*.avi;*.bmp;*.gif;*.jpg;*.jpeg;*.mov;*.mpg;*.mpeg;*.png;*.tga;*.tif;*.tiff;*.wmv|\
All supported image files|*.bmp;*.gif;*.jpg;*.jpeg;*.png;*.tga;*.tif;*.tiff|\
All supported video files|*.avi;*.mov;*.mpg;*.mpeg;*.wmv|\
AVI files (*.avi)|*.avi|\
BMP files (*.bmp)|*.bmp|\
GIF files (*.gif)|*.gif|\
JPG/JPEG files (*.jpg;*.jpeg)|*.jpg;*.jpeg|\
MOV files (*.mov)|*.mov|\
MPG/MPEG files (*.mpg;*.mpeg)|*.mpg;*.mpeg|\
PNG files (*.png)|*.png|\
TGA files (*.tga)|*.tga|\
TIF/TIFF files (*.tif;*.tiff)|*.tif;*.tiff|\
WMV files (*.wmv)|*.wmv",
    wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR | wxFD_MULTIPLE);


    // Set the position
    // XXX : This does not appear to work for the wxFileDialog.  
    //       Perhaps add it to a frame that we can move?
    fileDialog.SetPosition(wxPoint(x, y));


    if (fileDialog.ShowModal() == wxID_OK) {
        // Get the files
        std::string directory = fileDialog.GetDirectory().c_str();
        directory += "\\";

        wxArrayString fileList;
        fileDialog.GetFilenames(fileList);


        // Create full file paths
        std::vector<std::string> paths;
        for (int i = 0; i < (int)fileList.size(); i++) {
            paths.push_back(directory + fileList[i].c_str());
        }


        // Pass to the graphics
        CollageGraphics* cg = static_cast<CollageGraphics*>(graphics);
        int startImage = cg->GetImages().size();
        for (int i = 0; i < (int)paths.size(); i++) {
            std::string extension = paths[i].substr(paths[i].rfind('.') + 1);
            if (extension.compare("avi") == 0 ||
                extension.compare("mpg") == 0 ||
                extension.compare("mpeg") == 0 ||
                extension.compare("wmv") == 0) {
                cg->LoadVideo(paths[i]);
            }
            else if (extension.compare("mov") == 0) {
                cg->LoadVideo(paths[i], true);
            }
            else {
                cg->LoadImage(paths[i]);
            }
            cg->DoLayout(startImage);

            Render();
        }
    }
}


void CollageFrame::ChooseBackgroundColor(wxCoord x, wxCoord y) {
    // Dialogs can be created on the stack
    wxColourDialog colorDialog;

    // Set the position
    colorDialog.SetPosition(wxPoint(x, y));

    if (colorDialog.ShowModal() == wxID_OK) {
        // Set the color
        wxColour color = colorDialog.GetColourData().GetColour();
        ((CollageGraphics*)graphics)->SetBackgroundColor((float)color.Red()   / 255.0, 
                                                         (float)color.Green() / 255.0, 
                                                         (float)color.Blue()  / 255.0);
    }
}