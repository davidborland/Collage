///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        VideoFile.cpp
//
// Author:      David Borland
//
// Description: Class for playing video from a file using DirectShow
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#include "VideoFile.h"

#include <iostream>



VideoFile::VideoFile() : VideoStream() {
    loop = false;
}


void VideoFile::Update() {
    // Check video position
    LONGLONG position;
    LONGLONG stopPosition;

    mediaSeeking->GetCurrentPosition(&position);
    mediaSeeking->GetStopPosition(&stopPosition);

    if (position == stopPosition) {
        if (loop) {
            // Rewind
            position = 0;
            mediaSeeking->SetPositions(&position, AM_SEEKING_AbsolutePositioning, &stopPosition, AM_SEEKING_NoPositioning);
        }
        else {
            stopped = true;
        }   
    }

    VideoStream::Update();
}


void VideoFile::SetLoop(bool doLoop) {
    loop = doLoop;
}


void VideoFile::Rewind() {
    // Get video position
    LONGLONG position;
    LONGLONG stopPosition;

    mediaSeeking->GetStopPosition(&stopPosition);

    // Rewind
    position = 0;
    mediaSeeking->SetPositions(&position, AM_SEEKING_AbsolutePositioning, &stopPosition, AM_SEEKING_NoPositioning);
}


void VideoFile::Jump(float seconds) {
    // Get video position
    LONGLONG position;
    LONGLONG stopPosition;

    mediaSeeking->GetCurrentPosition(&position);
    mediaSeeking->GetStopPosition(&stopPosition);

    // Jump, converting from seconds to 100 nanoseconds
    position += LONGLONG(seconds * 1e7);
    if (position < 0) position = 0;
    if (position > stopPosition) position = stopPosition;
    mediaSeeking->SetPositions(&position, AM_SEEKING_AbsolutePositioning, &stopPosition, AM_SEEKING_NoPositioning);
}


bool VideoFile::CreateVideoStream() {
    // Convert the filename to wide char
    BSTR wideString;
    int length = MultiByteToWideChar(CP_ACP, 0, name.c_str(), lstrlenA(name.c_str()), NULL, 0);
    if (length <= 0) {
        std::cout << "VideoFile::Initialize() : Could not convert fileName." << std::endl;
        return false;
    }
    wideString = SysAllocStringLen(0, length);
    MultiByteToWideChar(CP_ACP, 0, name.c_str(), lstrlenA(name.c_str()), wideString, length);

    // RenderFile automatically generates the filters needed to render the video to the screen.  Do this before adding the null renderer
    // or the filters will not be generated correctly
    filterGraph->RenderFile(wideString, NULL);

    SysFreeString(wideString);

    return true;
}