///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        RenciFrame.cpp
//
// Author:      David Borland
//
// DeRenciiption: An abstract base class for a wxWidgets frame to be used in one of the Renci
//                visualization environments.
//
///////////////////////////////////////////////////////////////////////////////////////////////


#include "RenciFrame.h"
#include "RenciLog.h"


BEGIN_EVENT_TABLE(RenciFrame, wxFrame)
    EVT_TIMER(RenderTimerId, RenciFrame::OnTimer)
    EVT_CHAR(RenciFrame::OnKey)
    EVT_MOUSE_EVENTS(RenciFrame::OnMouse)
END_EVENT_TABLE()


RenciFrame::RenciFrame(const wxString& title, const wxSize& size, RenciGraphics* theGraphics,
                       EnvironmentMode environmentMode, bool toroidalMode)
: wxFrame((wxFrame*)NULL, wxID_ANY, title, wxPoint(0, 0), size, wxBORDER_NONE | wxSYSTEM_MENU), 
  graphics(theGraphics), environment(environmentMode), toroidal(toroidalMode) {
    // Create a log window for printing messages
    RenciLog* log = new RenciLog(this);
    log->Show();

    
    // Create a timer for rendering
    renderTimer = new wxTimer(this, RenderTimerId);


    // Initalize the old mouse x position
    oldMouseX = -1;
}

RenciFrame::~RenciFrame() {
    delete graphics;
}

bool RenciFrame::Initialize(unsigned int renderInterval) {
    // Set up the OpenGL canvases
    CreateCanvases();

    // Initialize the graphics
    int width, height;
    GetClientSize(&width, &height);
    
    if (environment == TeleImmersion) width /= 2;
        
    if (!graphics->Initialize(width, height)) {
        std::cout << "RenciFrame::Initialize() : Graphics initialization failed." << std::endl;
        return false;
    }

    // Start the timer
    if (!renderTimer->Start(renderInterval)) {
        std::cout << "RenciFrame::Initialize() : Could not start render timer." << std::endl;
        return false;
    }

    return true;
}


void RenciFrame::OnTimer(wxTimerEvent& e) {
    if (e.GetId() == RenderTimerId) {
        graphics->Update();
        Render();
    }
}

void RenciFrame::OnKey(wxKeyEvent& e) {
    int c = e.GetKeyCode();
    if (c == 'q') {
        // Quit
        Close();
    }
}

void RenciFrame::OnMouse(wxMouseEvent& e) {
    if (e.Moving() || e.Dragging()) {
        // Control mouse behavior for different environments
        int w, h;
        GetSize(&w, &h);

        int x = e.GetX();
        int y = e.GetY();

        if (environment == TeleImmersion) {
            // Only use half of the window due to overlapping projectors
            w = w / 2;

            if (!toroidal) {
                if (x > w - 1) {
                    WarpPointer(w - 1, y);
                }
                return;
            }
        }

        if (toroidal) {
            // Wrap the mouse
            if (x < oldMouseX && x == 0) {
                // Wrap to right edge
                WarpPointer(w - 1, y);
            }
            else if (x > oldMouseX && x == w - 1) {
                // Wrap to left edge
                WarpPointer(0, y);
            }
            oldMouseX = x;
        }
    }
}


void RenciFrame::CreateCanvases() {
    int* attribList = graphics->GetAttribList();

    int w, h;
    GetSize(&w, &h);

    if (environment == Normal || environment == TeleImmersion) {
        // Just one canvas
        canvases.push_back(new RenciGLCanvas(this, attribList,
                                             wxPoint(0, 0), wxSize(w, h)));
    }
    else if (environment == SCR) {
        // Need two canvases
        canvases.push_back(new RenciGLCanvas(this, attribList, 
                                             wxPoint(0, 0), wxSize(w / 2, h)));
        canvases.push_back(new RenciGLCanvas(this, attribList,
                                             wxPoint(w / 2, 0), wxSize(w / 2, h)));
    }

    // Create a single OpenGL context
    context = new wxGLContext(canvases[0]);
    context->SetCurrent(*canvases[0]);
}


void RenciFrame::Render() {
    if (environment == SCR) {
        // Render the left image
        context->SetCurrent(*canvases[0]);
        graphics->PreRender();
        graphics->RenderLeftSCR();
        graphics->PostRender();

        // Render the right image
        context->SetCurrent(*canvases[1]);
        graphics->PreRender();
        graphics->RenderRightSCR();
        graphics->PostRender();

        // Swap buffers
        canvases[0]->SwapBuffers();
        canvases[1]->SwapBuffers();
    }
    else if (environment == TeleImmersion) {
        // Render left half, then any stereo stuff (to right half)
        graphics->PreRender();
        graphics->RenderLeftTeleImmersion();
        graphics->RenderRightTeleImmersion();
        graphics->PostRender();

        // Swap buffer
        canvases[0]->SwapBuffers();
    }
    else {
        // Normal render
        graphics->PreRender();
        graphics->Render();
        graphics->PostRender();

        // Swap buffer
        canvases[0]->SwapBuffers();
    }
}