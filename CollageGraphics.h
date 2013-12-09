///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        CollageGraphics.h
//
// Author:      David Borland
//
// Description: Graphics for Collage application.
//
///////////////////////////////////////////////////////////////////////////////////////////////
//
//                         Mod History
//
//--------------------------------------------------------------------------
//	Mike Conway 7/10/2008
//	Added layout manager factory and simple layouts
//--------------------------------------------------------------------------
//
///////////////////////////////////////////////////////////////////////////////////////////////


#ifndef COLLAGEGRAPHICS_H
#define COLLAGEGRAPHICS_H

#include <RenciGraphics.h>
#include <VideoStream.h>

#include "CollageImage.h"
#include "CollageLayoutManagerFactory.h"
#include "SceneManager.h"
#include "CollageItemMetadata.h"

#include <string>
#include <vector>
#include <wx/filesys.h>
//#include <FTGL/ftgl.h>
//#include <ft2build.h>


// Forward declarations
class CollageLayoutManager;


class CollageGraphics : public RenciGraphics {
public:
    CollageGraphics(Image::Behavior imageBehaviorType = Image::Normal);
    virtual ~CollageGraphics();

    void SetSceneManager(SceneManager* manager);

    // Get required render attributes
    virtual int* GetAttribList();

    // Update videos
    virtual void Update();

    // Render the scene
    virtual void PreRender();
    virtual void Render();
    virtual void RenderStereo();

    virtual void OnKey(wxKeyEvent& e);
    virtual void OnMouse(wxMouseEvent& e);

    void LoadImage(const std::string& fileName);
    void LoadVideo(const std::string& fileName, bool quickTime = false);
	bool IsShowTitle();
	bool IsRenderLeft();

	std::vector<CollageImage*> GetImages();

	void DoLayout(unsigned int start = 0);
	SceneManager* GetSceneManager();
	void SetLayoutManager(CollageLayoutManagerFactory::LayoutType layoutType);
	
	void LoadImagesFromMetadataTextFile(std::string metadataTextFileName);

    void SetBackgroundColor(float r, float g, float b);

//	FTFont* GetFont();

private:
    // The last images will be the most current image
    std::vector<CollageImage*> images;
    std::vector<CollageImage*> currentImages;
    std::vector<VideoStream*> videos;
	wxFileSystem fs;
//    FTFont* font;
	unsigned int imageLoadCounter;

    Image::Behavior imageBehavior;

    // For wxGLCanvas
    int* attribList;

    Vec2 oldEventPosition;
	bool showTitle;
	bool renderLeft;

	CollageLayoutManagerFactory* layoutManagerFactory;
	CollageLayoutManager* layoutManager;
    
    // Not created here, but should be deleted here
    SceneManager* sceneManager;

    bool checkForClear;

    virtual bool InitGL();

    // Handle currently selected image list
    bool InCurrent(CollageImage* image);
    void ClearCurrent();
    void AddToCurrent(int i);
    void RemoveFromCurrent(CollageImage* image);
    void Delete();

	void SortDisplay(MetadataSortOption option);

	// methods are called by collage when rendering the left and right screens, facilitating any code that 
	// needs to act on individual openGL canvases
//    virtual void RenderExitRight();
//    virtual void RenderExitLeft();
};

// image sort binary predicates
bool CompareFileName(CollageImage* imageA, CollageImage* imageB);
bool ComparePath(CollageImage* imageA, CollageImage* imageB);
bool CompareTimestamp(CollageImage* imageA, CollageImage* imageB);


#endif