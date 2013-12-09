///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        VideoCapture.h
//
// Author:      David Borland
//
// Description: Class for playing video from a video capture device using DirectShow
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#ifndef VIDEOCAPTURE_H
#define VIDEOCAPTURE_H


#include "VideoStream.h"


class VideoCapture : public VideoStream {
public:
    VideoCapture();
    virtual ~VideoCapture();

protected:
    ICaptureGraphBuilder2* captureGraph;
    IBaseFilter* captureSource;
    IBaseFilter* dummyRenderer;

    std::string name;

    virtual bool CreateVideoStream();

    bool AddCaptureDevice();
    bool AddDummyRenderer();
};


#endif