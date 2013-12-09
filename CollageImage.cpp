///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        CollageImage.cpp
//
// Author:      David Borland
//
// Description: Includes Image behavior for Collage
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#include "CollageImage.h"
#include "CollageGraphics.h"


CollageImage::CollageImage(Behavior imageBehavior) : Image(imageBehavior) {
	border = false;
    windowHeight = 768;
    stereoOffset = 0.0f;

    stereoImage = NULL;
}

CollageImage::~CollageImage() {
    if (stereoImage) delete stereoImage;
}


void CollageImage::SetWindowHeight(int height) {
    windowHeight = height;
}


void CollageImage::SetPosition(const Vec2& pos) {
	Image::SetPosition(pos);

    if (stereoImage) {
        stereoImage->SetPosition(pos);
    }
}

void CollageImage::SetScale(float scaleValue) {
	Image::SetScale(scaleValue);

    if (stereoImage) {
        stereoImage->SetScale(scaleValue);
    }
}


bool CollageImage::Intersect(const Vec2& point) {
	// Extent of the image
	float left, right, bottom, top;
	GetExtent(left, right, bottom, top);

	// Check for intersection
	if (point.X() >= left &&
		point.X() <= right &&
		point.Y() >= bottom &&
		point.Y() <= top) {
			return true;
	}

	// View width
	float viewWidth = xMax - xMin;

	// Left coyp
	if (point.X() >= left - viewWidth &&
		point.X() <= right - viewWidth &&
		point.Y() >= bottom &&
		point.Y() <= top) {
			return true;
	}

	// Right copy
	if (point.X() >= left + viewWidth &&
		point.X() <= right + viewWidth &&
		point.Y() >= bottom &&
		point.Y() <= top) {
			return true;
	}

	return false;
}


void CollageImage::Translate(const Vec2& translation) {
	SetPosition(position + translation);
}


void CollageImage::FitToScreen() {
    SetPosition(Vec2(position.X(), 0.5));
    SetScale(1.0);
}

void CollageImage::NativeResolution() {
    SetPosition(Vec2(position.X(), 0.5));
    SetScale((float)resolution[1] / (float)windowHeight);
}


void CollageImage::GetExtent(float& left, float& right, float& bottom, float& top) {
	left = position.X() - aspectRatio * scale * 0.5;
	right = position.X() + aspectRatio * scale * 0.5;
	bottom = position.Y() - scale * 0.5;
	top = position.Y() + scale * 0.5;
}


void CollageImage::BorderOn() {
	border = true;

    if (stereoImage) stereoImage->BorderOn();
}

void CollageImage::BorderOff() {
	border = false;

    if (stereoImage) stereoImage->BorderOff();
}


void CollageImage::SetStereoImage(CollageImage* image) {
    stereoImage = image;

    stereoImage->SetPosition(position);
    stereoImage->SetScale(scale);

    if (border) stereoImage->BorderOn();
    else stereoImage->BorderOff();
}

bool CollageImage::HasStereoImage() {
    return stereoImage != NULL;
}

CollageImage* CollageImage::RemoveStereoImage() {
    CollageImage* temp = stereoImage;
    stereoImage = NULL;

    return temp;
}

void CollageImage::IncreaseStereoDepth() {
    stereoOffset -= 0.001f;

    if (stereoImage) stereoImage->SetStereoOffset(stereoOffset);
}

void CollageImage::DecreaseStereoDepth() {
    stereoOffset += 0.001f;

    if (stereoImage) stereoImage->SetStereoOffset(stereoOffset);
}

void CollageImage::SetStereoOffset(float offset) {
    stereoOffset = offset;
}


void CollageImage::RenderStereo() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    float offset = stereoOffset;

    float depth = 0.02f;
    if (!stereoImage) {
        if (border) {
            // Pop out
            offset -= depth;
        }
        else {
            // Recede
            offset += depth;
        }
    }

    glTranslatef(offset, 0.0f, 0.0f);

    if (stereoImage) stereoImage->Render();
    else Render();

    glPopMatrix();
}

// Code to display a 'legend' on each image, currently this is the file name, but will be later expanded to provide other metadata
/*
void CollageImage::ShowLegend(float translateX, float translateY) {
	// if I don't have a font loaded, don't do this
	FTFont * font = collageGraphics->GetFont();
	if (font->Error()) return;

	if (collageGraphics->IsShowTitle()) {

		glPushAttrib(GL_TEXTURE_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		// draw a legend box

		glColor3f(0.5, 0.5, 1.0) ;

		glBegin(GL_QUADS); 
		glVertex2d(-0.5 * aspectRatio, -0.5);
		glVertex2d(0.5 * aspectRatio , -0.5);
		glVertex2d(0.5 * aspectRatio, -0.2);
		glVertex2d(-0.5 * aspectRatio, -0.2);
		glEnd();

		// here, I'm either rendering right or left.  Adding fonts is done using screen space, not the scaling that was set
		// in glOrtho.  There are actually two displays, and they share one context.  When the second display is swapped in, I will need
		// to render the text positioned at 6144-12288 as if it was 0-6143.  

		font->FaceSize(72 * scale);
		float xBegin = translateX - (aspectRatio * scale * .5);
		float xToPixel = xBegin * 768;

		if (collageGraphics->IsRenderLeft() && xToPixel < 6144) {
			// if i'm rendering to the 0-6143 screen, I'm rendering
			FTPoint point = FTPoint(xToPixel, ((position.Y() - scale * 0.5) * 768) + (20 * scale));
			font->Render(collageItemMetadata.fileName.c_str(), -1, point);
		} else if (!collageGraphics->IsRenderLeft() && xToPixel >= 6144) {
			// if i'm rendering right and the postion is greater than or equal to 6144, then render, but adjust the x so it's based
			// on a 0-6143 range
			xToPixel -= 6144;
			FTPoint point = FTPoint(xToPixel, ((position.Y() - scale * 0.5) * 768) + (20 * scale));
			font->Render(collageItemMetadata.fileName.c_str(), -1, point);
		}

		glPopAttrib();
	}
}
*/

void CollageImage::DoRender() {
	// Render 3 times to get a toroidal wraparound
	glMatrixMode(GL_MODELVIEW);

	// Center image
	glPushMatrix();
	glTranslatef((GLfloat)position.X(), (GLfloat)position.Y(), 0.0);
	glScalef((GLfloat)scale, (GLfloat)scale, 1.0);

	RenderQuad();
	if (border) RenderBorder();
//	ShowLegend(position.X(), position.Y());

	glPopMatrix();

    if (behavior == Toroidal) {
        // Render two more times
	    float viewWidth = xMax - xMin;

	    // Left image
	    glPushMatrix();
	    glTranslatef((GLfloat)(position.X() - viewWidth), (GLfloat)position.Y(), 0.0);
	    glScalef((GLfloat)scale, (GLfloat)scale, 1.0);

	    RenderQuad();
	    if (border) RenderBorder();
//        ShowLegend(position.X() - viewWidth, position.Y());

	    glPopMatrix();

	    // Right image
	    glPushMatrix();
	    glTranslatef((GLfloat)(position.X() + viewWidth), (GLfloat)position.Y(), 0.0);
	    glScalef((GLfloat)scale, (GLfloat)scale, 1.0);

	    RenderQuad();
	    if (border) RenderBorder();
//        ShowLegend(position.X() + viewWidth, position.Y());

	    glPopMatrix(); 
    }
}


void CollageImage::RenderBorder() {
	glPushAttrib(GL_ENABLE_BIT);

	glDisable(GL_TEXTURE_RECTANGLE_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex2f((GLfloat)(-0.5 * aspectRatio), -0.5);
	glVertex2f((GLfloat)(0.5 * aspectRatio), -0.5);

	glVertex2f((GLfloat)(0.5 * aspectRatio), -0.5);
	glVertex2f((GLfloat)(0.5 * aspectRatio), 0.5);

	glVertex2f((GLfloat)(0.5 * aspectRatio), 0.5);
	glVertex2f((GLfloat)(-0.5 * aspectRatio), 0.5);

	glVertex2f((GLfloat)(-0.5 * aspectRatio), 0.5);
	glVertex2f((GLfloat)(-0.5 * aspectRatio), -0.5);
	glEnd();

	glPopAttrib();
}

CollageItemMetadata* CollageImage::GetCollageItemMetadata() {
	return &collageItemMetadata;
}

void CollageImage::SetCollageGraphics(CollageGraphics* collageGraphics) {
	this->collageGraphics = collageGraphics;
}

CollageGraphics* CollageImage::GetCollageGraphics() {
	return this->collageGraphics;
}
