///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        VideoStream.h
//
// Author:      David Borland
//
// Description: Abstract class for playing a video stream using DirectShow
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#ifndef VIDEOSTREAM_H
#define VIDEOSTREAM_H


#include "Image.h"

// Hack to deal with the fact that qedit.h is not included in recent versions of the Windows or DirectX SDK
#pragma include_alias( "dxtrans.h", "qedit.h" )
#define __IDxtCompositor_INTERFACE_DEFINED__
#define __IDxtAlphaSetter_INTERFACE_DEFINED__
#define __IDxtJpeg_INTERFACE_DEFINED__
#define __IDxtKey_INTERFACE_DEFINED__
#include "qedit.h"

#include <DShow.h>
#include <string>


/*
class SampleGrabberCB : public ISampleGrabberCB {
public:
    SampleGrabberCB() : ISampleGrabberCB() {}
    ~SampleGrabberCB() {}


    STDMETHODIMP SampleCB(double SampleTime, IMediaSample* pSample) { printf("Here\n"); 
                                                                      BYTE* data;
                                                                      pSample->GetPointer(&data);
                                                                      return TRUE;}
    STDMETHODIMP BufferCB(double SampleTime, BYTE* pBuffer, long BufferLen) { return FALSE; }


    // Implement COM reference counting
    STDMETHODIMP_(ULONG) AddRef() { return (++refCount); }
    STDMETHODIMP_(ULONG) Release() { return (--refCount); }

    // Implement COM querying
    STDMETHODIMP QueryInterface(REFIID riid, void ** ppv) {
        *ppv = this;
        this->AddRef();
        return S_OK;
    }

private:
    ULONG refCount;
};
*/



class VideoStream {
public:
    VideoStream();
    virtual ~VideoStream();

    enum VideoType {
        RGB,
        RGBA
    };

    bool Initialize(const std::string& sourceName, Image* renderImage = NULL, VideoType type = RGB);
    virtual void Update();

    void SetImage(Image* renderImage);
    Image* GetImage();

    int GetWidth() const;
    int GetHeight() const;
    VideoType GetVideoType() const;  
    
    const unsigned char* GetBuffer();

    const std::string& GetName() const;

    void Play();
    void Stop();
    
    bool Stopped();

protected:
    IGraphBuilder* filterGraph;
    IBaseFilter* sampleGrabber;
    ISampleGrabber* sampleGrabberInterface;
    IBaseFilter* nullRenderer;
    IMediaControl* mediaControl;
    IMediaSeeking* mediaSeeking;

    unsigned int width, height;
    long bufferSize;
    unsigned char* buffer;
    
    VideoType videoType;

    // An optional image used to render the video
    Image* image;

    std::string name;

    bool stopped;

    // Pure virtual function that must be implemented by base classes
    virtual bool CreateVideoStream() = 0;

    bool AddSampleGrabber();
    bool AddNullRenderer();
    bool InsertFilters();
    IPin* GetPin(IBaseFilter* filter, PIN_DIRECTION pinDir);

    bool SetDimensions();
};


#endif