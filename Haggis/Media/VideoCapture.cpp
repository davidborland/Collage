///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        VideoCapture.cpp
//
// Author:      David Borland
//
// Description: Class for playing video from a video capture device using DirectShow
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#include "VideoCapture.h"

#include <iostream>



VideoCapture::VideoCapture() : VideoStream() {
    captureGraph = NULL;
    captureSource = NULL;
    dummyRenderer = NULL;

    name = "Decklink Video Capture";
}

VideoCapture::~VideoCapture() {
    if (captureGraph) captureGraph->Release();
    if (captureSource) captureSource->Release();
}


bool VideoCapture::CreateVideoStream() {
    // Create the capture graph builder
    if (CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void**)&captureGraph) != S_OK) {
        std::cout << "VideoCapture::Initialize() : Could not create captureGraph." << std::endl;
        return false;
    }

    captureGraph->SetFiltergraph(filterGraph);


    // Add capture device
    if (!AddCaptureDevice()) return false;


    // Add dummy renderer
    if (!AddDummyRenderer()) return false;


    // Render the graph
    if (captureGraph->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, captureSource, NULL, dummyRenderer) != S_OK) {
        std::cout << "VideoCapture::Initialize() : Could not render captureSource." << std::endl;
        return false;
    }

    return true;
}


bool VideoCapture::AddCaptureDevice() {
    IBaseFilter* temp = NULL;
    IMoniker* moniker = NULL;
    ICreateDevEnum* devEnum = NULL;
    IEnumMoniker* classEnum = NULL;

    // Create the system device enumerator
    if (CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC, IID_ICreateDevEnum, (void**)&devEnum) != S_OK) {
        std::cout << "VideoStream::AddCaptureDevice() : Could not create system enumerator." << std::endl;
        return false;
    }

    // Create an enumerator for the video capture devices
    if (devEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &classEnum, 0) != S_OK) {
        std::cout << "VideoStream::AddCaptureDevice() : Could not create class enumerator." << std::endl;
        return false;
    }

    // Check for the requested type
    if (classEnum == NULL) {
        std::cout << "VideoStream::AddCaptureDevice() : No video capture device detected." << std::endl;
        return false;
    }

    // Find the capture card
    bool foundCapture = false;
    while (classEnum->Next(1, &moniker, NULL) == S_OK) {
        IPropertyBag* propBag;
        if (moniker->BindToStorage(0, 0, IID_IPropertyBag, (void**)&propBag) != S_OK) {
            std::cout << "VideoStream::AddCaptureDevice() : Can't bind moniker to propBag." << std::endl;
            return false;
        }

        // Find the description or friendly name.
        VARIANT varName;
        VariantInit(&varName);
        if (propBag->Read(L"FriendlyName", &varName, 0) == S_OK) {
            char c[256];
            WideCharToMultiByte(CP_ACP, 0, varName.bstrVal, -1, c, 256, NULL, NULL);

            std::string s = c;

            if (s == name) {
                foundCapture = true;
                propBag->Release();
                break;
            }
        }
        propBag->Release();
    }


    // Check to see if found
    if (!foundCapture) {
        std::cout << "VideoStream::AddCaptureDevice() : Can't find " << name << std::endl;
        return false;
    }


    // Bind moniker to filter object
    if (moniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&captureSource) != S_OK) {
        std::cout << "VideoStream::AddCaptureDevice() : Can't bind moniker to filter object." << std::endl;
        return false;
    }


    // Add the capture device to the filter graph
    if (filterGraph->AddFilter(captureSource, L"Video Capture") != S_OK) {
        std::cout << "VideoStream::AddCaptureDevice() : Could not add captureSource to filter graph." << std::endl;
        return false;
    }

    captureSource->AddRef();

    moniker->Release();
    devEnum->Release();
    classEnum->Release();
    
    return true;
}


bool VideoCapture::AddDummyRenderer() {
    // Create a video renderer
    if (CoCreateInstance(CLSID_VideoRenderer, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&dummyRenderer) != S_OK) {
        std::cout << "VideoCapture::AddDummyRenderer() : Could not create dummyRenderer." << std::endl;
        return false;
    }

    // Add the video renderer to the filter graph
    if (filterGraph->AddFilter(dummyRenderer, L"Video Renderer") != S_OK) {
        std::cout << "VideoCapture::AddDummyRenderer() : Could not add dummyRenderer to filterGraph." << std::endl;
        return false;
    }

    return true;
}