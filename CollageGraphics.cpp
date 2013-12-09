///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        CollageGraphics.cpp
//
// Author:      David Borland
//
// Description: Graphics for Collage application.
//
///////////////////////////////////////////////////////////////////////////////////////////////


#include "CollageGraphics.h"
#include <VideoFile.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <algorithm>


CollageGraphics::CollageGraphics(Image::Behavior imageBehaviorType) 
: RenciGraphics(), imageBehavior(imageBehaviorType) {
	layoutManagerFactory = NULL;
	layoutManager = NULL;
    sceneManager = NULL;
	imageLoadCounter = 0;

    // OpenGL attributes
    attribList = new int[3];
    attribList[0] = WX_GL_RGBA;
    attribList[1] = WX_GL_DOUBLEBUFFER;
    attribList[2] = 0;

	// TODO:  font selection?  Add later when context menu is available
/*
	font = new FTPixmapFont("C:\\WINDOWS\\Fonts\\ARIAL.TTF");
	// If something went wrong, bail out. 
	if(font->Error()) {
		std::cout << "CollageGraphics::Constructor : font missing" << std::endl;
	} else {
		std::cout << "CollageGraphics::CollageGraphics() : font loaded" << std::endl;
	}
*/
}

CollageGraphics::~CollageGraphics() {
	// Clean up
	for (int i = 0; i < (int)images.size(); i++) {
		delete images[i];
	}

	for (int i = 0; i < (int)videos.size(); i++) {
		delete videos[i];
	}

	if (sceneManager) delete sceneManager;

    delete attribList;

//	delete font;
}


void CollageGraphics::SetSceneManager(SceneManager* manager) {
    if (sceneManager) {
        delete sceneManager;
    }
     
    sceneManager = manager;
}


int* CollageGraphics::GetAttribList() {
    return attribList;
}


void CollageGraphics::Update() {
    for (int i = 0; i < (int)videos.size(); i++) {
		videos[i]->Update();
	}
}


void CollageGraphics::PreRender() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void CollageGraphics::Render() {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	for (int i = 0; i < (int)images.size(); i++) {
		images[i]->Render();
	}

	glPopMatrix();
}

void CollageGraphics::RenderStereo() {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	for (int i = 0; i < (int)images.size(); i++) {
		images[i]->RenderStereo();
	}

	glPopMatrix();
}


void CollageGraphics::OnKey(wxKeyEvent& e) {
	int c = e.GetKeyCode();
	float translateIncrement = 0.05f;

	if (c == 9) {
		// Tab
		if (!images.empty()) {
			ClearCurrent();
			AddToCurrent(0);
		}
	}
	else if (c == 127) {
		// Delete
		Delete();
	}
	else if (c == 32) {
		// space bar - show image titles
		showTitle = !showTitle;
	}
	else if (c == 314) {
		// Left arrow

		for (int i = 0; i < (int)currentImages.size(); i++) {
			currentImages[i]->Translate(Vec2(-translateIncrement, 0.0));
		}

	}
	else if (c == 316) {
		// right arrow	
		for (int i = 0; i < (int)currentImages.size(); i++) {
			currentImages[i]->Translate(Vec2(translateIncrement, 0.0));
		}

	}
	else if (c == 315) {
		// Up arrow
		for (int i = 0; i < (int)currentImages.size(); i++) {
			currentImages[i]->Translate(Vec2(0.0, translateIncrement));
		}
	}
	else if (c == 317) {
		// Down arrow
		for (int i = 0; i < (int)currentImages.size(); i++) {
			currentImages[i]->Translate(Vec2(0.0, -translateIncrement));
		}
	}
	else if (c == '1') {
		// Layout method 1
		SetLayoutManager(CollageLayoutManagerFactory::LM_SIMPLE_SINGLE);
		DoLayout();
	}
	else if (c == '2') {
		// Layout method 1
		SetLayoutManager(CollageLayoutManagerFactory::LM_SIMPLE_DOUBLE);
		DoLayout();
	}
	else if (c == '3') {
		// Layout method 3
		SetLayoutManager(CollageLayoutManagerFactory::LM_SMART_SINGLE);
		DoLayout();
	}
    else if (c == '4') {
        // Layout method 4
        SetLayoutManager(CollageLayoutManagerFactory::LM_RANDOM);
        DoLayout();
    }
    else if (c == '5') {
        // Layout method 5
        SetLayoutManager(CollageLayoutManagerFactory::LM_FILL_ROOM);
        DoLayout();
    }
	else if (c == 'a') {
		// Select all
		for (int i = (int)images.size() - 1; i >= 0; i--) {
			AddToCurrent(i);
		}
	}
	else if (c == 'f') {
		// Fit to screen
		for (int i = 0; i < (int)currentImages.size(); i++) {
			currentImages[i]->FitToScreen();
		}
	}
	else if (c == 'n') {
		// Native resolution
		for (int i = 0; i < (int)currentImages.size(); i++) {
			currentImages[i]->NativeResolution();
		}
	}
    else if (c == 's') {
        // Stereo
        if (currentImages.size() == 1) {
            if (currentImages[0]->HasStereoImage()) {
                // Switch left and right
                CollageImage* image1 = currentImages[0];
                CollageImage* image2 = image1->RemoveStereoImage();

                // Switch in current images
                currentImages[0] = image2;

                // Switch in image list
                for (int i = 0; i < (int)images.size(); i++) {
                    if (images[i] == image1) {
                        images[i] = image2;
                        break;
                    }
                }

                // Make them a stereo pair
                image2->SetStereoImage(image1);
            }
        }
        else if (currentImages.size() == 2) {
            if (!currentImages[0]->HasStereoImage() && !currentImages[1]->HasStereoImage()) {
                // Make stereo
                CollageImage* image1 = currentImages[0];
                CollageImage* image2 = currentImages[1];

                // Remove image2 from current list
                RemoveFromCurrent(image2);

                // Remove image2 from image list
                for (int i = 0; i < (int)images.size(); i++) {
                    if (images[i] == image2) {
                        images.erase(images.begin() + i);
                        break;
                    }
                }

                // Make them a stereo pair
                image1->SetStereoImage(image2);
            }
        }
    }
    else if (c == 'u') {
        // Move up in stereo depth
        for (int i = 0; i < (int)currentImages.size(); i++) {
            currentImages[i]->IncreaseStereoDepth();
        }
    }
    else if (c == 'd') {
        // Move down in stereo depth
        for (int i = 0; i < (int)currentImages.size(); i++) {
            currentImages[i]->DecreaseStereoDepth();
        }
    }
	//*************** test keys *****************************
	// TODO:  i, o, p are for testing, figure out a better way to sort and remove keys (use context menu)
	else if (c == 'p') {
		// sort
		SortDisplay(metadataSortFilename);
		DoLayout();

	}
	else if (c == '[') {
		// sort
		SortDisplay(metadataSortTimestamp);
		DoLayout();

	}
	else if (c == ']') {
		// sort by path
		SortDisplay(metadataSortPath);
		DoLayout();
	}
	// ************** end test keys *************************
	else if (c == 'l') {
		DoLayout();
	}
}

void CollageGraphics::OnMouse(wxMouseEvent& e) {
    RenciGraphics::OnMouse(e);

	// Convert from window to view
	Vec2 position;
	position.X() = (float)e.GetX() / (float)(windowWidth - 1) * viewWidth;
	position.Y() = (1.0 - (float)e.GetY() / (float)(windowHeight - 1)) * viewHeight;



	if (e.LeftDown()) {
		oldEventPosition = position;

		// Collision detection
		for (int i = (int)images.size() - 1; i >= 0; i--) {
			if (images[i]->Intersect(position)) {
				if (!e.ControlDown() && !InCurrent(images[i])) {  
					// Clear the current selection
					ClearCurrent();
				}
				else if (e.ControlDown() && InCurrent(images[i])) {
					// Remove from current selection
					RemoveFromCurrent(images[i]);
					return;
				}

				AddToCurrent(i);
				return;
			}
		}

		// No collision
		ClearCurrent();

		// Start drawing selection rectangle
		// XXX
	}
	else if (e.Dragging()) {
		if (e.LeftIsDown()) {
			// Move the current images
			for (int i = 0; i < (int)currentImages.size(); i++) {
				currentImages[i]->Translate(position - oldEventPosition);
			}
			oldEventPosition = position;
		}
	}
	else if (abs(e.GetWheelRotation()) != 0) {
		// Get an integer proportional to the scroll wheel speed
		int scrollAmount = e.GetWheelRotation() / e.GetWheelDelta();

		for (int i = 0; i < (int)currentImages.size(); i++) {
			// Get the current scale
			double scale = currentImages[i]->GetScale();

			// Get the scale direction
			double scaleAmount = scrollAmount > 0 ? 1.0 / 1.1 : 1.1;

			// Scale multiple times based on scroll wheel speed
			for (int j = 0; j < abs(scrollAmount); j++) {
				scale *= scaleAmount;
			}

			// Set the new scale
			currentImages[i]->SetScale(scale);
		}
	}
}


// Public method to access the images array for manipulation (e.g. layout)
std::vector<CollageImage*> CollageGraphics::GetImages() {
	return images;
}


// using the current CollageLayoutManager, lay out the images using the specific algorithm.  The layout
// manager is selected in SetLayoutManager();
void CollageGraphics::DoLayout(unsigned int start) {
	// Layout the screen according to the currently selected layout.  
	// If no layout is specified, use the defualt scheme.
	try {
		if (this->layoutManager == NULL) {
			std::cout << "CollageGraphics::DoLayout() : No layout manager specified...defaulting" << std::endl;
			this->SetLayoutManager(CollageLayoutManagerFactory::LM_SIMPLE_SINGLE);
			layoutManager->Layout(start);
		} 
		else {
			std::cout << "CollageGraphics::DoLayout() : Doing layout" << std::endl;
			layoutManager->Layout(start);
		}
	} 
	catch(...) {
		std::cout << "CollageGraphics::DoLayout() : Exception doing layout" << std::endl;
	}
}


void CollageGraphics::LoadImage(const std::string& fileName) {

	std::cout << "CollageGraphics::LoadImage() : Loading " << fileName << std::endl;

	// Check to see if we can load this image type
	if (!wxImage::CanRead(fileName)) {
		std::cout << "CollageGraphics::LoadImage() : Cannot load this image type." << std::endl;
		return;
	}

	// Load the image
	wxImage image(fileName.c_str());

	// Check for validity
	if (!image.IsOk()) {
		std::cout << "CollageGraphics::LoadImage() : Could not load image." << std::endl;
		return;
	}

	// get the file name and parse out parts

	int posDot = fileName.find_last_of('.');
	int posFileSlash = fileName.find_last_of('\\');

	// extension is backwards to the '.'
	// file name is between the . and the first '\'
	// path is everything to the left of the last '\'

	std::string extension = fileName.substr(posDot + 1);
	std::string fileNamePart = fileName.substr(posFileSlash + 1, posDot);
	// need to trim off the slash(start at next pos)
	//std::string filePathPart = fileName.substr(0, posFileSlash + 1);

	// get the file timestamp
	wxFSFile* file = fs.OpenFile(wxString(fileName));
	wxDateTime fileTime = file->GetModificationTime();

	// Flip the image
	image = image.Mirror(false);


	// Get image info
	int width = image.GetWidth();
	int height = image.GetHeight();


	// Check the pixel format
	Image::PixelFormat pixelFormat;
	unsigned char* imageData;
	if (image.HasAlpha()) {
		// wxImage does not store alpha along with RGB, so need to insert it
		imageData = new unsigned char[width * height * 4];
		unsigned char* tempRGB = image.GetData();
		unsigned char* tempAlpha = image.GetAlpha();
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				imageData[i * width * 4 + j * 4 + 0] = tempRGB[i * width * 3 + j * 3 + 0];      // Red
				imageData[i * width * 4 + j * 4 + 1] = tempRGB[i * width * 3 + j * 3 + 1];      // Green
				imageData[i * width * 4 + j * 4 + 2] = tempRGB[i * width * 3 + j * 3 + 2];      // Blue
				imageData[i * width * 4 + j * 4 + 3] = tempAlpha[i * width + j];                // Alpha
			}
		}        
		pixelFormat = Image::RGBA;
	}
	else {
		// GL_TEXTURE_RECTANGLE_ARB does not appear to work correctly for non-RGBA images with odd dimensions, so pad with an alpha channel.
		imageData = new unsigned char[width * height * 4];
		unsigned char* tempRGB = image.GetData();
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				imageData[i * width * 4 + j * 4 + 0] = tempRGB[i * width * 3 + j * 3 + 0];      // Red
				imageData[i * width * 4 + j * 4 + 1] = tempRGB[i * width * 3 + j * 3 + 1];      // Green
				imageData[i * width * 4 + j * 4 + 2] = tempRGB[i * width * 3 + j * 3 + 2];      // Blue
				imageData[i * width * 4 + j * 4 + 3] = 255;                                     // Alpha
			}
		}        
		pixelFormat = Image::RGBA;
	}

	// Create the image
    images.push_back(new CollageImage(imageBehavior));
	if (!images.back()->SetTextureInfo(width, height, pixelFormat)) {
		std::cout << "CollageGraphics::LoadImage() : Could not create texture." << std::endl;

		delete images.back();
		images.pop_back();

		delete imageData;

		return;
	}

	if (!images.back()->SetTextureData(imageData)) {
		std::cout << "CollageGraphics::LoadImage() : Could not set texture data." << std::endl;


		delete images.back();
		images.pop_back();

		delete imageData;

		return;

	}

	images.back()->SetViewExtents(0.0, viewWidth, 0.0, viewHeight);
    images.back()->SetWindowHeight(windowHeight);
	images.back()->NativeResolution();

	// initialize image metadata
	CollageItemMetadata* metadata = images.back()->GetCollageItemMetadata();
	metadata->fileName = fileNamePart;
	metadata->fileNameExtension = extension;
	metadata->path = fileName;
	metadata->itemSetOrder = 0;
	metadata->itemLoadOrder = imageLoadCounter++;
	metadata->itemTimestamp = fileTime.GetTicks();

	// add a reference to CollageGraphics to the image
	images.back()->SetCollageGraphics(this);

	delete imageData;
}

void CollageGraphics::LoadVideo(const std::string& fileName, bool quickTime) {
	std::cout << "CollageGraphics::LoadVideo() : Loading " << fileName << std::endl;


	// Create an image
	images.push_back(new CollageImage(imageBehavior));

	// TODO:  add code to insert metadata

	// Create the video
	videos.push_back(new VideoFile());
	VideoStream::VideoType videoType = quickTime ? VideoStream::RGBA : VideoStream::RGB;
	if (!videos.back()->Initialize(fileName, images.back(), videoType)) {
		std::cout << "CollageGraphics::LoadVideo() : Video initialization failed." << std::endl;

		delete images.back();
		images.pop_back();

		delete videos.back();
		videos.pop_back();

		return;
	}
	static_cast<VideoFile*>(videos.back())->SetLoop(true);


	// Finish image setup
	images.back()->SetViewExtents(0.0, viewWidth, 0.0, viewHeight);    
    images.back()->SetWindowHeight(windowHeight);
	images.back()->NativeResolution();  


	// Play the image
	videos.back()->Play();
}


bool CollageGraphics::InCurrent(CollageImage* image) {
	for (int i = 0; i < (int)currentImages.size(); i++) {
		if (image == currentImages[i]) return true;
	}
	return false;
}

void CollageGraphics::ClearCurrent() {
	for (int i = 0; i < (int)currentImages.size(); i++) {
		currentImages[i]->BorderOff();
	}
	currentImages.clear();
}

void CollageGraphics::AddToCurrent(int i) {
	// If valid, add this to the current images
	if (i >= 0 && i < (int)images.size()) {
		CollageImage* temp = images[i];
		temp->BorderOn();

		// Reorder so it renders on top
		images.erase(images.begin() + i);
		images.push_back(temp);

		// If not in current images, add it
		if (!InCurrent(temp)) currentImages.push_back(temp);
	}
}

void CollageGraphics::RemoveFromCurrent(CollageImage* image) {
	for (int i = 0; i < (int)currentImages.size(); i++) {
		if (image == currentImages[i]) {
			image->BorderOff();
			currentImages.erase(currentImages.begin() + i);
			return;
		}
	} 
}

void CollageGraphics::Delete() {
	for (int i = 0; i < (int)images.size(); i++) {
		for (int j = 0; j < (int)currentImages.size(); j++) {
			// Check image
			if (images[i] == currentImages[j]) {
				// Search videos for this image
				for (int k = 0; k < (int)videos.size(); k++) {
					if (videos[k]->GetImage() == images[i]) {
						// Remove video
						delete videos[k];
						videos.erase(videos.begin() + k);
						break;
					}
				}

				delete images[i];
				images.erase(images.begin() + i);
				i--;
				currentImages.erase(currentImages.begin() + j);
				j--;
				break;
			}
		}
	}
}


bool CollageGraphics::InitGL() {    
	// Initialize Glew for checking OpenGL extensions.
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cout << "CollageGraphics::InitGL() : " << glewGetErrorString(err) << std::endl;
		return false;
	}

	// Check extensions   
	if (!GLEW_ARB_texture_rectangle) {
		std::cout << "CollageGraphics::InitGL() : GL_ARB_texture_rectangle not supported on this graphics card." << std::endl;
		return false;
	}

	// Turn off depth testing
	glDisable(GL_DEPTH_TEST);

	// Turn off lighting
	glDisable(GL_LIGHTING);

	// Turn on blending
	glEnable(GL_BLEND);

	// Set the blending function
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set the background
//	glClearColor(0.21f, 0.21f, 0.19f, 1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


    // Set projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0, viewWidth, 0.0, viewHeight, -1.0, 1.0);


	return true;
}

// Returns a handle to the SceneManager, which describes the display environment
SceneManager* CollageGraphics::GetSceneManager(void) {
	return this->sceneManager;
}


// Using the CollageLayoutManagerFactory, create a new layout manager based on the selected type.  The 
// collageLayoutManager is set to this factory, and subsequent calls to DoLayout() will be invoked on that
// manager
void CollageGraphics::SetLayoutManager(CollageLayoutManagerFactory::LayoutType layoutType) {
	try {
		if (this->layoutManagerFactory == NULL) {
			std::cout << "CollageGraphics::SetLayoutManager() : Creating layout manager factory." << std::endl;
			this->layoutManagerFactory = new CollageLayoutManagerFactory(this);
		} 

		//std::cout << "CollageGraphics::SetLayoutManager() : layout manager pointer for test: ", this->collageLayoutManager << std::endl;

		if (this->layoutManager != NULL) {
			// Delete the old layout manager, will switch to a new one
			std::cout << "CollageGraphics::SetLayoutManager() : Clearing old collage layout manager." << std::endl;
			delete this->layoutManager;
		}

		std::cout << "CollageGraphics::SetLayoutManager() : Setting new layout manager of type " << layoutType << std::endl;
		layoutManager = layoutManagerFactory->CreateLayoutManager(layoutType);
	} 
	catch(...) {
		std::cout << "CollageGraphics::SetLayoutManager() : Exception setting layout manager." << std::endl;
	}

}

// TODO: finish implementing all sort options in CollageItemMetadata
// Sort the images based on the sort option, using the metadata associated with the images
void CollageGraphics::SortDisplay(MetadataSortOption option) {

	switch(option) {
		case metadataSortFilename:
			std::sort(images.begin(), images.end(), CompareFileName);
			break;
		case metadataSortTimestamp:
			std::sort(images.begin(), images.end(), CompareTimestamp);
			break;
		case metadataSortPath:
			std::sort(images.begin(), images.end(), ComparePath);
			break;
	}

}

// TODO: implement load from text file
// Load collage based on a selected metadata text file
void CollageGraphics::LoadImagesFromMetadataTextFile(std::string metadataTextFileName) {
}


void CollageGraphics::SetBackgroundColor(float r, float g, float b) {
    glClearColor(r, g, b, 1.0);
}


/*
FTFont* CollageGraphics::GetFont() {
	return font;
}
*/

bool CollageGraphics::IsShowTitle() {
	return showTitle;
}
/*
// Method called when rendering for the 'right' display.  This method will set a flag indicating which canvas is being rendered.
// This flag is used in Collage to do rendering of fonts, which are calculated in screen space, versus the normal transformations for
// other graphic content in collage.
void CollageGraphics::RenderExitRight() {

	renderLeft = false;
}

// Method called when rendering for the 'right' display.  This method will set a flag indicating which canvas is being rendered.
// This flag is used in Collage to do rendering of fonts, which are calculated in screen space, versus the normal transformations for
// other graphic content in collage.
void CollageGraphics::RenderExitLeft() {
	renderLeft = true;
}

// Return a bool that indicates whether the left screen is being rendered
bool CollageGraphics::IsRenderLeft() {
	return renderLeft;
}
*/

// ***********************  Sort binary predicates *******************************************

bool CompareFileName(CollageImage* imageA, CollageImage* imageB) {
	return (imageA->GetCollageItemMetadata()->fileName < imageB->GetCollageItemMetadata()->fileName);
}

bool ComparePath(CollageImage* imageA, CollageImage* imageB) {
	return (imageA->GetCollageItemMetadata()->path < imageB->GetCollageItemMetadata()->path);
}

bool CompareTimestamp(CollageImage* imageA, CollageImage* imageB) {
	return (imageA->GetCollageItemMetadata()->itemTimestamp < imageB->GetCollageItemMetadata()->itemTimestamp);
}