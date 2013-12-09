///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        VideoFile.h
//
// Author:      David Borland
//
// Description: Class for playing video from a file using DirectShow
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#ifndef VIDEOFILE_H
#define VIDEOFILE_H


#include "VideoStream.h"


class VideoFile : public VideoStream {
public:
    VideoFile();

    virtual void Update();

    void SetLoop(bool doLoop);

    void Rewind();
    void Jump(float seconds);

protected:
    bool loop;

    virtual bool CreateVideoStream();
};


#endif