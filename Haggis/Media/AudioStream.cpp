///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        AudioStream.cpp
//
// Author:      David Borland
//
// Description: Plays back audio file using DirectShow
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#include "AudioStream.h"

#include <iostream>


bool AudioStream::initializedLibrary = false;


AudioStream::AudioStream() {
}

AudioStream::~AudioStream() {
    BASS_StreamFree(stream);
}


void AudioStream::SetFileName(const std::string& name) {
    fileName = name;
}


bool AudioStream::Initialize(bool doLoop, unsigned int channel) {
    // Check to see if BASS has been initialized.
    if (!initializedLibrary) {
        std::cout << "AudioStream::Initialize() : Library has not been initialized." << std::endl;
        return false;
    }


    loop = doLoop;


    DWORD flags = 0;
    
    // Select the channel to play back on
    switch (channel) {
    case 1 :
        flags = BASS_SPEAKER_FRONTLEFT;
        break;
    case 2 :
        flags = BASS_SPEAKER_FRONTRIGHT;
        break;
    case 3 :
        flags = BASS_SPEAKER_REARLEFT;
        break;
    case 4 :
        flags = BASS_SPEAKER_REARRIGHT;
        break;
    case 5 :
        flags = BASS_SPEAKER_CENTER;
        break;
    case 6 :
        flags = BASS_SPEAKER_LFE;
        break;
    case 7 :
        flags = BASS_SPEAKER_REAR2LEFT;
        break;
    case 8 :
        flags = BASS_SPEAKER_REAR2RIGHT;
        break;
    }


    // Set looping
    if (loop) {
        flags |= BASS_SAMPLE_LOOP;
    }


    // Initialize the file
    if (!(stream = BASS_StreamCreateFile(FALSE, fileName.c_str(), 0, 0, flags))) {
        std::cout << "AudioStream::Initialize() : Can't play file, BASS error code " << (int)BASS_ErrorGetCode() << std::endl;
        return false;
    }

    return true;
}


void AudioStream::Play() {
    BASS_ChannelPlay(stream, FALSE);
}

void AudioStream::Pause() {
    BASS_ChannelPause(stream);
}

void AudioStream::Stop() {
    BASS_ChannelStop(stream);
}

void AudioStream::Rewind() {
    BASS_ChannelSetPosition(stream, 0, BASS_POS_BYTE);
}

void AudioStream::Jump(float seconds) {
    BASS_ChannelSetPosition(stream, BASS_ChannelGetPosition(stream, BASS_POS_BYTE) + 
                                    BASS_ChannelSeconds2Bytes(stream, seconds), BASS_POS_BYTE);
}   


bool AudioStream::Stopped() {
    return (BASS_ChannelIsActive(stream) == BASS_ACTIVE_STOPPED);
}


void AudioStream::SetChannel(unsigned int channel) {
    DWORD flags = 0;
    
    // Select the channel to play back on
    switch (channel) {
    case 1 :
        flags = BASS_SPEAKER_FRONTLEFT;
        break;
    case 2 :
        flags = BASS_SPEAKER_FRONTRIGHT;
        break;
    case 3 :
        flags = BASS_SPEAKER_REARLEFT;
        break;
    case 4 :
        flags = BASS_SPEAKER_REARRIGHT;
        break;
    case 5 :
        flags = BASS_SPEAKER_CENTER;
        break;
    case 6 :
        flags = BASS_SPEAKER_LFE;
        break;
    case 7 :
        flags = BASS_SPEAKER_REAR2LEFT;
        break;
    case 8 :
        flags = BASS_SPEAKER_REAR2RIGHT;
        break;
    }


    // Set looping
    if (loop) {
        flags |= BASS_SAMPLE_LOOP;
    }

    BASS_ChannelFlags(stream, flags, 0);


// XXX : Below should work, but make sure with above code...
/*
    BASS_CHANNELINFO info;
    BASS_ChannelGetInfo(stream, &info);

    // Turn off all channels
    info.flags &= ~BASS_SPEAKER_FRONTLEFT;
    info.flags &= ~BASS_SPEAKER_FRONTRIGHT;
    info.flags &= ~BASS_SPEAKER_REARLEFT;
    info.flags &= ~BASS_SPEAKER_REARRIGHT;
    info.flags &= ~BASS_SPEAKER_CENTER;
    info.flags &= ~BASS_SPEAKER_LFE;
    info.flags &= ~BASS_SPEAKER_REAR2LEFT;
    info.flags &= ~BASS_SPEAKER_REAR2RIGHT;

    // Select the channel to play back on
    switch (channel) {
    case 1 :
        info.flags |= BASS_SPEAKER_FRONTLEFT;
        break;
    case 2 :
        info.flags |= BASS_SPEAKER_FRONTRIGHT;
        break;
    case 3 :
        info.flags |= BASS_SPEAKER_REARLEFT;
        break;
    case 4 :
        info.flags |= BASS_SPEAKER_REARRIGHT;
        break;
    case 5 :
        info.flags |= BASS_SPEAKER_CENTER;
        break;
    case 6 :
        info.flags |= BASS_SPEAKER_LFE;
        break;
    case 7 :
        info.flags |= BASS_SPEAKER_REAR2LEFT;
        break;
    case 8 :
        info.flags |= BASS_SPEAKER_REAR2RIGHT;
        break;
    }

    BASS_ChannelSetFlags(stream, info.flags);
*/
}


void AudioStream::SetVolume(float volume, float seconds) {
    BASS_ChannelSlideAttribute(stream, BASS_ATTRIB_VOL, (float)(volume * 100.0), (int)(seconds * 1000.0));
}


bool AudioStream::InitializeLibrary(HWND win, bool speakerAssignment) {
    // Check the correct BASS was loaded
    if (HIWORD(BASS_GetVersion()) != BASSVERSION) {
        std::cout << "AudioStream::InitializeLibrary() : An incorrect version of BASS was loaded" << std::endl;
        return false;
    }
    

    // Initialize BASS
    DWORD flags = 0;
    if (speakerAssignment) flags = BASS_DEVICE_SPEAKERS;
    if (!BASS_Init(1, 44100, flags, win, NULL)) {
        std::cout << "AudioStream::InitializeLibrary() : BASS initialization failed" << std::endl;
        return false;
    }


    initializedLibrary = true;

    return true;
}


void AudioStream::FreeLibrary() {
    BASS_Free();
}