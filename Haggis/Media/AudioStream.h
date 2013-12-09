///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        AudioStream.h
//
// Author:      David Borland
//
// Description: Plays back audio file using BASS
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#ifndef AUDIOSTREAM_H
#define AUDIOSTREAM_H


#include <bass.h>
#include <string>


class AudioStream {
public:
    AudioStream();
    virtual ~AudioStream();

    void SetFileName(const std::string& name);

    bool Initialize(bool doLoop = false, unsigned int channel = 0);
   
    void Play();
    void Pause();
    void Stop();
    void Rewind();
    void Jump(float seconds);
    
    bool Stopped();

    void SetChannel(unsigned int channel);

    // Set the volume (0...1) and the amount of time in seconds to reach that volume
    void SetVolume(float volume, float seconds);

    static bool InitializeLibrary(HWND win = 0, bool speakerAssignment = false);
    static void FreeLibrary();

protected:
    DWORD stream;

    std::string fileName;

    bool loop;
    
    static bool initializedLibrary;
};


#endif