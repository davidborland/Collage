///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        Image.cpp
//
// Author:      David Borland
//
// Description: Class for drawing an OpenGL quad textured with an image or video.
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#include "Image.h"


Image::Image(Behavior imageBehavior) : RenderObject(), behavior(imageBehavior) {
    texture = -1;
    pbo = -1;

    resolution[0] = resolution[1] = 0;
    aspectRatio = 1.0;

    SetPixelFormats(RGBA);
    textureType = TEXTURE_2D_MIPMAP;
    textureTarget = GL_TEXTURE_2D;

    bufferSize = 0;

    textureCreated = false;
}
 
Image::~Image() {
    if (textureCreated) CleanUp();
}


void Image::SetTexture(GLuint textureMap, unsigned int width, unsigned int height, PixelFormat type) {
    if (textureCreated) {
        CleanUp();
    }

    resolution[0] = width;
    resolution[1] = height;
    aspectRatio = (float)resolution[0] / (float)resolution[1];
    SetPixelFormats(type);

    texture = textureMap;

    textureCreated = false;
}


bool Image::SetTextureInfo(unsigned int width, unsigned int height, 
                           PixelFormat format, TextureType type) {
    // Check texture info
    if (!textureCreated || 
        resolution[0] != width || resolution[1] != height ||
        pixelFormat != format || textureType != type) {
        // Delete old texture
        if (textureCreated) CleanUp();

        // Set new texture information
        resolution[0] = width;
        resolution[1] = height;
        aspectRatio = (float)resolution[0] / (float)resolution[1];
        SetPixelFormats(format);

        // Set textureType and textureTarget
        textureType = type;
        if (textureType == TEXTURE_RECTANGLE ||
            textureType == TEXTURE_RECTANGLE_PBO) {
            textureTarget = GL_TEXTURE_RECTANGLE_ARB;
        }
        else {
            textureTarget = GL_TEXTURE_2D;
        }

        if (!CreateTexture()) {
            std::cout << "Image::SetTextureInfo() : Error.  Texture creation failed." << std::endl;
            return false;
        }
    }

    return true;
}


bool Image::SetTextureData(const unsigned char* data) {
    // Check that there is a texture
    if (!textureCreated) {
        std::cout << "Image::SetTextureData() : Error.  Texture not created yet." << std::endl;
        return false;
    }


    // Bind the texture
    glBindTexture(textureTarget, texture);


    // Set the texture data
    if (textureType == TEXTURE_2D || textureType == TEXTURE_RECTANGLE) {
        glTexSubImage2D(textureTarget, 0, 0, 0, resolution[0], resolution[1], glPixelFormat, GL_UNSIGNED_BYTE, data);
    }
    else if (textureType == TEXTURE_2D_MIPMAP) {
        if (gluBuild2DMipmaps(textureTarget, glInternalPixelFormat, resolution[0], resolution[1], glPixelFormat, GL_UNSIGNED_BYTE, data) != 0) {
            std::cout << "Image::SetTextureData() : Error.  Could not generate mipmaps." << std::endl;
            return false;
        }
    }
    else if (textureType == TEXTURE_2D_PBO || textureType == TEXTURE_RECTANGLE_PBO) {
        // Bind the pbo
        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pbo);

        // Copy the data to the pbo
        glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, bufferSize, NULL, GL_STATIC_DRAW_ARB);
        unsigned char* pixels = (unsigned char*)glMapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, GL_WRITE_ONLY_ARB);
        memcpy(pixels, data, bufferSize);
        glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB);

        // Copy from the pbo to the texture
        glTexSubImage2D(textureTarget, 0, 0, 0, resolution[0], resolution[1], glPixelFormat, GL_UNSIGNED_BYTE, NULL);

        // Unbind the pbo
        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0); 
    }
    else {
        // Should never be here, but just in case...
        std::cout << "Image::SetTextureData() : Error.  Invalid texture type." << std::endl;
        return false;
    }

    return true;
}


void Image::SetPosition(const Vec2& pos) {
    RenderObject::SetPosition(pos);

    float edgeAmount = 0.05f;

    // Behaviors
    if (behavior == Toroidal) {
        // Wrap in x
        if (position.X() < xMin) position.X() += (xMax - xMin);
        else if (position.X() > xMax) position.X() -= (xMax - xMin);

        // Check bounds in y
        float h = GetHeight();

        float b = yMin + edgeAmount;
        float t = yMin + yMax - edgeAmount;

        if (position.Y() + h * 0.5 < b) {
            // Bottom
		    position.Y() = b - h * 0.5;
	    }
	    else if (position.Y() - h * 0.5 > t) {
            // Top
		    position.Y() = t + h * 0.5;
	    }
    }
    else if (behavior == CheckBounds) {
        float w = GetWidth();
        float h = GetHeight();

        float l = xMin + edgeAmount;
        float r = xMin + xMax - edgeAmount;
        float b = yMin + edgeAmount;
        float t = yMin + yMax - edgeAmount;

	    if (position.X() + w * 0.5 < l) {
            // Left
		    position.X() = l - w * 0.5;
	    }
        else if (position.X() - w * 0.5 > r) {
            // Right
		    position.X() = r + w * 0.5;
	    }

        if (position.Y() + h * 0.5 < b) {
            // Bottom
		    position.Y() = b - h * 0.5;
	    }
	    else if (position.Y() - h * 0.5 > t) {
            // Top
		    position.Y() = t + h * 0.5;
	    }
    }
}

void Image::SetScale(double scaleValue) {
    RenderObject::SetScale(scaleValue);

    SetPosition(position);
}


void Image::SetViewExtents(float xMinVal, float xMaxVal, float yMinVal, float yMaxVal) {
    xMin = xMinVal;
    xMax = xMaxVal;
    yMin = yMinVal;
    yMax = yMaxVal;
}


float Image::GetWidth() {
	return aspectRatio * (float)scale;
}

float Image::GetHeight() {
	return (float)scale;
}


float Image::GetAspectRatio() {
    return aspectRatio;
}


void Image::PreRender() {
    // Enable texturing
    glEnable(textureTarget);

    // Bind the texture
    glBindTexture(textureTarget, texture);
}

void Image::DoRender() {
    // Push the modelview matrix stack
    glMatrixMode(GL_MODELVIEW);

    // Transform
    glPushMatrix();
    glTranslated(position.X(), position.Y(), position.Z());
    double angle;
    Vec3 axis;
    quaternion.GetAngleAxis(angle, axis);    
    glRotated(Quat::RadiansToDegrees(angle), axis.X(), axis.Y(), axis.Z());
    glScaled(scale, scale, 1.0);

    // Render the quad
    RenderQuad();

    // Pop the modelview matrix stack
    glPopMatrix();

    if (behavior == Toroidal) {
        // Render two more times
        double viewWidth = xMax = xMin;

        // Left image
        glPushMatrix();
        glTranslated(position.X() - viewWidth, position.Y(), position.Z());   
        glRotated(Quat::RadiansToDegrees(angle), axis.X(), axis.Y(), axis.Z());
        glScaled(scale, scale, 1.0);

        // Render the quad
        RenderQuad();

        // Pop the modelview matrix stack
        glPopMatrix();

        // Right image
        glPushMatrix();
        glTranslated(position.X() - viewWidth, position.Y(), position.Z());  
        glRotated(Quat::RadiansToDegrees(angle), axis.X(), axis.Y(), axis.Z());
        glScaled(scale, scale, 1.0);

        // Render the quad
        RenderQuad();

        // Pop the modelview matrix stack
        glPopMatrix();
    }
}

void Image::PostRender() {
    // Disable texturing
    glDisable(textureTarget);
}


bool Image::CreateTexture() {
    textureCreated = false;

    // Set the buffer size
    int numComponents;
    if (pixelFormat == LUMINANCE) numComponents = 1;
    else if (pixelFormat == RGB || pixelFormat == BGR) numComponents = 3;
    else if (pixelFormat == RGBA || pixelFormat == BGRA) numComponents = 4;

    bufferSize = resolution[0] * resolution[1] * numComponents;


    // Create the pixel buffer object for fast texture download to the graphics card.
    if (textureType == TEXTURE_2D_PBO || textureType == TEXTURE_RECTANGLE_PBO) {
        glGenBuffersARB(1, &pbo);
        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pbo);
        glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, bufferSize, NULL, GL_STATIC_DRAW_ARB);

        // XXX : Check for success here?
    }


    // Set up the texture
    glGenTextures(1, &texture);
    glBindTexture(textureTarget, texture);
    if (textureType == TEXTURE_2D_MIPMAP) {
        glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }
    else {
        glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


    // Create the texture
    if (textureType != TEXTURE_2D_MIPMAP) {
        if (!CheckTextureCreation()) {
            std::cout << "Image::CreateTexture() : Error.  Texture creation failed." << std::endl; 
            CleanUp();
            return false;
        }

        glTexImage2D(textureTarget, 0, glInternalPixelFormat, resolution[0], resolution[1], 0, glPixelFormat, GL_UNSIGNED_BYTE, NULL);
    }


    // Unbind the pixel buffer object
    if (textureType == TEXTURE_2D_PBO || textureType == TEXTURE_RECTANGLE_PBO) {
        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
    }


    textureCreated = true;

    return true;
}


bool Image::CheckTextureCreation() {
    glTexImage2D(GL_PROXY_TEXTURE_2D, 0, glInternalPixelFormat, resolution[0], resolution[1], 0, glPixelFormat, GL_UNSIGNED_BYTE, NULL);

    int widthTest;
    glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &widthTest);

    if (widthTest == 0) {
        std::cout << "Image::CheckTextureCreation() : Error.  OpenGL texture creation failed." << std::endl;
        return false;
    }

    return true;
}


void Image::CleanUp() {
    glDeleteTextures(1, &texture);

    if (textureType == TEXTURE_2D_PBO || textureType == TEXTURE_RECTANGLE_PBO) {
        glDeleteBuffersARB(1, &pbo);
    }
}


void Image::RenderQuad() {
    if (textureType == TEXTURE_RECTANGLE || textureType == TEXTURE_RECTANGLE_PBO) {
        RenderQuadTextureRectangle();
    }
    else {
        RenderQuadTexture2D();
    }
}

void Image::RenderQuadTexture2D() {
    // Draw the textured quad with a height of 1.0, preserving the aspect ratio
    glBegin(GL_QUADS);
        glTexCoord2d(0, 0);
        glVertex2d(-0.5 * aspectRatio, -0.5);

        glTexCoord2d(1, 0);
        glVertex2d(0.5 * aspectRatio, -0.5);

        glTexCoord2d(1, 1);
        glVertex2d(0.5 * aspectRatio, 0.5);

        glTexCoord2d(0, 1);
        glVertex2d(-0.5 * aspectRatio, 0.5);
    glEnd();
}

void Image::RenderQuadTextureRectangle() {    
    // Draw the textured quad with a height of 1.0, preserving the aspect ratio
    glBegin(GL_QUADS);
        glTexCoord2d(0, 0);
        glVertex2d(-0.5 * aspectRatio, -0.5);

        glTexCoord2d(resolution[0], 0);
        glVertex2d(0.5 * aspectRatio, -0.5);

        glTexCoord2d(resolution[0], resolution[1]);
        glVertex2d(0.5 * aspectRatio, 0.5);

        glTexCoord2d(0, resolution[1]);
        glVertex2d(-0.5 * aspectRatio, 0.5);
    glEnd();
}


void Image::SetPixelFormats(PixelFormat format) {
    // Set pixel formats
    pixelFormat = format;
    if (pixelFormat == LUMINANCE) {
        glInternalPixelFormat = GL_LUMINANCE;
        glPixelFormat = GL_LUMINANCE;
    }
    else if (pixelFormat == RGB) {
        glInternalPixelFormat = GL_RGB;
        glPixelFormat = GL_RGB;
    }
    else if (pixelFormat == BGR) {
        glInternalPixelFormat = GL_RGB;
        glPixelFormat = GL_BGR;
    }
    else if (pixelFormat == RGBA) {
        glInternalPixelFormat = GL_RGBA;
        glPixelFormat = GL_RGBA;
    }       
    else if (pixelFormat == BGRA) {
        glInternalPixelFormat = GL_RGBA;
        glPixelFormat = GL_BGRA;
    }
    else {
        std::cout << "Image::SetPixelFormats() : Error.  Invalid pixel format." << std::endl;
    }
}