///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        VideoStream.cpp
//
// Author:      David Borland
//
// Description: Abstract class for playing a video stream using DirectShow
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#include "VideoStream.h"

#include <iostream>


VideoStream::VideoStream() {
    filterGraph = NULL;
    sampleGrabber = NULL;
    sampleGrabberInterface = NULL;
    nullRenderer = NULL;
    mediaControl = NULL;
    mediaSeeking = NULL;

    width = height = bufferSize = 0;
    buffer = NULL;

    image = NULL;

    videoType = RGB;

    stopped = true;
}

VideoStream::~VideoStream() {
/*
    // Clean up
    IEnumFilters *pEnum = NULL;
    HRESULT hr = filterGraph->EnumFilters(&pEnum);
    if (SUCCEEDED(hr)) {
        IBaseFilter *pFilter = NULL;
        while (S_OK == pEnum->Next(1, &pFilter, NULL)) {
            // Remove the filter.
            filterGraph->RemoveFilter(pFilter);
            // Reset the enumerator.
            pEnum->Reset();
            pFilter->Release();
        }
   }
   pEnum->Release();
*/

    if (mediaSeeking) mediaSeeking->Release();
    if (mediaControl) mediaControl->Release();
    if (nullRenderer) nullRenderer->Release();
    if (sampleGrabberInterface) sampleGrabberInterface->Release();
    if (sampleGrabber) sampleGrabber->Release();
    if (filterGraph) filterGraph->Release();

    if (buffer) delete [] buffer;
}


bool VideoStream::Initialize(const std::string& sourceName, Image* renderImage, VideoType type) {
    // It is assumed that COM has been initialized here...
    name = sourceName;
    image = renderImage;
    videoType = type;

    // Create the filter graph manager
    if (CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&filterGraph) != S_OK) {
        std::cout << "VideoStream::Initialize() : Could not create filterGraph." << std::endl;
        return false;
    }

    // Create the video stream
    if (!CreateVideoStream()) return false;


    // Add the sample grabber
    if (!AddSampleGrabber()) return false;


    // Add the null renderer
    if (!AddNullRenderer()) return false;


    // Insert the sample grabber and null renderer
    if (!InsertFilters()) return false;


    // Set the video dimensions
    if (!SetDimensions()) return false;

    if (videoType == RGB) {
        bufferSize = width * height * 3;
    }
    else if (videoType == RGBA) {
        bufferSize = width * height * 4;
    }
    else {
        std::cout << "VideoCapture::Initialize() : Invalid videoType." << std::endl;
        return false;
    }
    buffer = new unsigned char[bufferSize];


    // Get a media control interface
    if (filterGraph->QueryInterface(IID_IMediaControl, (void**)&mediaControl) != S_OK) {
        std::cout << "VideoCapture::Initialize() : Could not create mediaControl interface." << std::endl;
        return false;
    }

    // Get a media seeking interface
    if (filterGraph->QueryInterface(IID_IMediaSeeking, (void**)&mediaSeeking) != S_OK) {        
        std::cout << "VideoCapture::Initialize() : Could not create mediaSeeking interface." << std::endl;
        return false;
    }


    // If there is an image, initialize it
    if (image) {
        // Get the pixel format.  DirectShow uses BGR and BGRA.
        Image::PixelFormat pixelFormat;
        if (videoType == VideoStream::RGB) {
            pixelFormat = Image::BGR;
        }
        else if (videoType == VideoStream::RGBA) {
            pixelFormat = Image::BGRA;
        }
        else {
            std::cout << "VideoCapture::Initialize() : Error. Invalid video type." << std::endl;
            return false;
        }

        // Create the image
        if (!image->SetTextureInfo(width, height, pixelFormat, Image::TEXTURE_RECTANGLE_PBO)) {
            std::cout << "VideoCapture::Initialize() : Error.  Could not create texture." << std::endl;
            return false;
        }
        if (!image->SetTextureData(buffer)) {
            std::cout << "VideoCapture::Initialize() : Error.  Could not set texture data." << std::endl;
            return false;
        }
    }


    return true;    
}

void VideoStream::Update() {
    if (!stopped && image) {
        image->SetTextureData(GetBuffer());
    }
}


void VideoStream::SetImage(Image* renderImage) {
    image = renderImage;
}

Image* VideoStream::GetImage() {
    return image;
}


int VideoStream::GetWidth() const {
    return width;
}

int VideoStream::GetHeight() const {
    return height;
}

VideoStream::VideoType VideoStream::GetVideoType() const {
    return videoType;
}


const unsigned char* VideoStream::GetBuffer() {
    sampleGrabberInterface->GetCurrentBuffer(&bufferSize, (long*)buffer);
    return buffer;
}


const std::string& VideoStream::GetName() const {
    return name;
}


void VideoStream::Play() {
    mediaControl->Run();
    stopped = false;
}


void VideoStream::Stop() {
    mediaControl->Stop();
    stopped = true;
}


bool VideoStream::Stopped() {
    return stopped;
}


bool VideoStream::AddSampleGrabber() {
    // Create the sample grabber
    if (CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&sampleGrabber) != S_OK) {
        std::cout << "VideoStream::AddSampleGrabber() : Could not create sampleGrabber.\n";
        return false;
    }

    // Query the sample grabber for the sample grabber interface
    sampleGrabberInterface = NULL;
    if (sampleGrabber->QueryInterface(IID_ISampleGrabber, (void**)&sampleGrabberInterface) != S_OK) {
        std::cout << "VideoStream::AddSampleGrabber() : Could not create sampleGrabber interface.\n";   
        return false;
    }

    // Specify the media type to process
    AM_MEDIA_TYPE mt;
    ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
    mt.majortype = MEDIATYPE_Video;
    if (videoType == RGB) {
        mt.subtype = MEDIASUBTYPE_RGB24;
    }
    else if (videoType == RGBA) {
        mt.subtype = MEDIASUBTYPE_RGB32;
    }
    else {
        std::cout << "VideoStream::AddSampleGrabber() : Invalid videoType." << std::endl;
        return false;
    }
    mt.formattype = FORMAT_VideoInfo;
    if (sampleGrabberInterface->SetMediaType(&mt) != S_OK) {
        std::cout << "VideoStream::AddSampleGrabber() : Could not set sampleGrabber interface media type." << std::endl;
        return false;
    }

    // Set working mode as continuous with a buffer
    sampleGrabberInterface->SetOneShot(FALSE);
    sampleGrabberInterface->SetBufferSamples(TRUE);    
    
    // Add the sample grabber to the filter graph
    if (filterGraph->AddFilter(sampleGrabber, L"Sample Grabber") != S_OK) {
        std::cout << "VideoStream::AddSampleGrabber() : Could not add sampleGrabber to filter graph." << std::endl;
        return false;
    }

/*
    // Set up the callback
    sampleGrabberCB = new SampleGrabberCB();
    sampleGrabberInterface->SetCallback(sampleGrabberCB, 0);
*/

    return true;
}


bool VideoStream::AddNullRenderer() {
    // Create a null renderer
    nullRenderer = NULL;
    if (CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&nullRenderer) != S_OK) {
        std::cout << "VideoStream::AddNullRenderer() : Could not create nullRenderer." << std::endl;
        return false;
    }

    // Add the null renderer to the filter graph
    if (filterGraph->AddFilter(nullRenderer, L"Null Renderer") != S_OK) {
        std::cout << "VideoStream::AddNullRenderer() : Could not add nullRenderer to filterGraph." << std::endl;
        return false;
    }

    return true;
}


bool VideoStream::InsertFilters() {
    // Locate default video renderer
    IBaseFilter* videoRenderer = NULL;
    filterGraph->FindFilterByName(L"Video Renderer", &videoRenderer);
    if (videoRenderer) {
        // Get input pin of video renderer.
        IPin* ipin = GetPin(videoRenderer, PINDIR_INPUT);
        IPin* opin = NULL;

        // Find out who the renderer is connected to and disconnect from them
        ipin->ConnectedTo(&opin);
        ipin->Disconnect();
        opin->Disconnect();

        ipin->Release();

        // Remove the default renderer from the graph		
        filterGraph->RemoveFilter(videoRenderer);
        videoRenderer->Release();

        // See if the video renderer was originally connected to a color space converter
        IBaseFilter* colorConverter = NULL;
        filterGraph->FindFilterByName(L"Color Space Converter", &colorConverter);

        if (colorConverter) {
	        opin->Release();

	        // Remove the converter from the graph
	        ipin = GetPin(colorConverter, PINDIR_INPUT);

	        ipin->ConnectedTo(&opin);
	        ipin->Disconnect();
	        opin->Disconnect();

	        ipin->Release();
        	
	        filterGraph->RemoveFilter(colorConverter);
	        colorConverter->Release();
        }

        // Get the input pin of the sample grabber
        ipin = GetPin(sampleGrabber, PINDIR_INPUT);

        // Connect the filter that was originally connected to the default renderer to the sample grabber
        filterGraph->Connect(opin, ipin);
        ipin->Release();
        opin->Release();

        // Get output pin of sample grabber
        opin = GetPin(sampleGrabber, PINDIR_OUTPUT);

        // Get input pin of null renderer
        ipin = GetPin(nullRenderer, PINDIR_INPUT);

        // Connect them
        filterGraph->Connect(opin, ipin);
        ipin->Release();
        opin->Release();
    }

    return true;
}

IPin* VideoStream::GetPin(IBaseFilter* filter, PIN_DIRECTION pinDir) {
    BOOL found = FALSE;
    IEnumPins* enumPins;
    IPin* pin;

    if (filter->EnumPins(&enumPins) != S_OK) {
        return NULL;
    }

    while (enumPins->Next(1, &pin, 0) == S_OK) {
        PIN_DIRECTION thisPinDir;
        pin->QueryDirection(&thisPinDir);
        found = (pinDir == thisPinDir);
        if (found) break;
        pin->Release();
    }

    enumPins->Release();
    return (found ? pin : NULL);  
}


bool VideoStream::SetDimensions() {
    AM_MEDIA_TYPE mediaType;
    
    if (sampleGrabberInterface->GetConnectedMediaType(&mediaType) != S_OK) {
        std::cout << "VideoStream::SetDimensions() : Could not get connected media type." << std::endl;
        return false;
    }

    VIDEOINFOHEADER* videoInfo = (VIDEOINFOHEADER*)mediaType.pbFormat;

    width = videoInfo->bmiHeader.biWidth;
    height = videoInfo->bmiHeader.biHeight;

    return true;
}