///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        Image.h
//
// Author:      David Borland
//
// Description: Class for drawing an OpenGL quad textured using GL_texture_rectangle_arb
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#ifndef IMAGE_H
#define IMAGE_H


#include "RenderObject.h"


class Image : public RenderObject {
public:   
    // Enumeration for different behaviors
    enum Behavior {
        Normal,
        Toroidal,
        CheckBounds
    };

    Image(Behavior imageBehavior = Normal);
    virtual ~Image();


    // XXX : Looks like there is a bug with RGB textures and GL_TEXTURE_RECTANGLE_ARB, where textures with
    //       odd dimensions do not render correctly.  It is advised to use RGBA textures.
    enum PixelFormat {
        LUMINANCE,
        RGB,
        BGR,        // Direct show RGB video
        RGBA,
        BGRA,       // Direct show RGBA video
    };

    // Various types of textures
    enum TextureType {
        TEXTURE_2D,                             // Standard 2D OpenGL texture
        TEXTURE_2D_MIPMAP,                      // Standard 2D OpenGL texture with mipmapping
        TEXTURE_2D_PBO,                         // Standard 2D OpenGL texture that uses pixel buffer objects
                                                //      for fast transfer of texture data

        TEXTURE_RECTANGLE,                      // Uses GL_TEXTURE_RECTANGLE_ARB
        TEXTURE_RECTANGLE_PBO                   // Uses GL_TEXTURE_RECTANGLE_ARB and pixel buffer objects
                                                //      for fast transfer of texture data
    };


    // Set the texture to use
    virtual void SetTexture(GLuint textureMap, unsigned int width, unsigned int height, PixelFormat format);


    // Set the texture information and create a textures and pbo if necessary
    bool SetTextureInfo(unsigned int width, unsigned int height, 
                        PixelFormat format, TextureType type = TEXTURE_2D_MIPMAP);

    // Set the texture data using the current texture informaton
    bool SetTextureData(const unsigned char* data);

    // Include different behaviors
    virtual void SetPosition(const Vec2& pos);
    virtual void SetScale(double scaleValue);

    // Set the view extents, assuming an orthographic projection
    void SetViewExtents(float xMinVal, float xMaxVal, float yMinVal, float yMaxVal);

    // Get the current width and height of the image
    float GetWidth();
    float GetHeight();

    // Get the aspect ratio of the image
    float GetAspectRatio();

protected:
    // The texture
    GLuint texture;

    // The pixel buffer object used for copying data to the texture
    GLuint pbo;

    // Resolution and aspect ratio
    unsigned int resolution[2];
    float aspectRatio;

    // View Extents
    float xMin, xMax, yMin, yMax;

    // Behavior
    Behavior behavior;

    // Format of the pixels
    PixelFormat pixelFormat;
    GLint glInternalPixelFormat;
    GLenum glPixelFormat;

    // Type of texture
    TextureType textureType;
    
    // Use GL_TEXTURE_2D or GL_TEXTURE_RECTANGLE_ARB
    GLenum textureTarget;

    // Size of the buffer for copying data
    unsigned int bufferSize;

    // Was the texture created here or not
    bool textureCreated;

    virtual void PreRender();
    virtual void DoRender();
    virtual void PostRender();

    virtual bool CreateTexture();
    bool CheckTextureCreation();
    virtual void CleanUp();

    void RenderQuad();
    void RenderQuadTexture2D();
    void RenderQuadTextureRectangle();

    void SetPixelFormats(PixelFormat format);
};


#endif