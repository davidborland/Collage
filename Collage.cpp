///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        Collage.cpp
//
// Author:      David Borland
//
// Description: Opens multiple images for viewing in the SCR.
//
///////////////////////////////////////////////////////////////////////////////////////////////


#include "Collage.h"

#include "CollageGraphics.h"
#include "CollageFrame.h"
#include "SceneManager.h"
#include "SCRSceneManager.h"
#include "TeleImmersionSceneManager.h"


IMPLEMENT_APP(Collage)


bool Collage::OnInit() {
    int mode = 0;
    if (argc == 2) {
        if (std::string(argv[1]) == "-SCR") mode = 1;
        else if (std::string(argv[1]) == "-TeleImmersion") mode = 2;
        else if (std::string(argv[1]) == "-DukeWall") mode = 3;
    }

    CollageGraphics* graphics;
    SceneManager* sceneManager;
    CollageFrame* frame;

    if (mode == 0) {
        // Testing
        graphics = new CollageGraphics(Image::CheckBounds);
        sceneManager = new TeleImmersionSceneManager(graphics);
        graphics->SetSceneManager(sceneManager);
        graphics->SetLayoutManager(CollageLayoutManagerFactory::LM_RANDOM);
        frame = new CollageFrame("Collage", wxSize(1920 * 2, 1200), graphics);
    }
    else if (mode == 1) {
        // SCR
        graphics = new CollageGraphics(Image::Toroidal);
        sceneManager = new SCRSceneManager(graphics);
        graphics->SetSceneManager(sceneManager);
        frame = new CollageFrame("Collage", wxSize(1024 * 12, 768), graphics, RenciFrame::SCR, true);
    }
    else if (mode == 2) {
        // TeleImmersion
        graphics = new CollageGraphics(Image::CheckBounds);
        sceneManager = new TeleImmersionSceneManager(graphics);        
        graphics->SetSceneManager(sceneManager);
        graphics->SetLayoutManager(CollageLayoutManagerFactory::LM_RANDOM);
        frame = new CollageFrame("Collage", wxSize(1920 * 4, 1080 * 2), graphics, RenciFrame::TeleImmersion);
    }
    else if (mode == 3) {
        // DukeWall
        graphics = new CollageGraphics(Image::CheckBounds);
        sceneManager = new TeleImmersionSceneManager(graphics);        
        graphics->SetSceneManager(sceneManager);
        graphics->SetLayoutManager(CollageLayoutManagerFactory::LM_RANDOM);
        frame = new CollageFrame("Collage", wxSize(5760, 2160), graphics);
    }

    // Tell wxWidgets that this is the main frame
    SetTopWindow(frame);

    // Check initialization
    if (!frame->Initialize()) {
        std::cout << "Collage::OnInit() : Frame initialization failed.  Exiting..." << std::endl;
        return false;
    }

    // Show it
    frame->Show();

    // Want to be able to load all supported image formats
    wxInitAllImageHandlers();

    // Open initial images
    int w, h;
    frame->GetSize(&w, &h);
    frame->ChooseMedia(w / 8, h / 4);


    // Success:  wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned false here, the
    // application would exit immediately.
    return true;
}