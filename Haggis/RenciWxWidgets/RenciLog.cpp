///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        RenciLog.cpp
//
// Author:      David Borland
//
// Description:  Uses a wxTextCtrl and wxStreamToTextRedirector to enable printing of messages
//               sent to C++ ostreams
//
///////////////////////////////////////////////////////////////////////////////////////////////


#include "RenciLog.h"


RenciLog::RenciLog(wxWindow* parent, const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(parent, wxID_ANY, title, pos, size) {    
    // Add a panel to hold the widget
    wxPanel* panel = new wxPanel(this);

    // Add a text control for showing output
    wxTextCtrl* text = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(512, 256), 
                                      wxTE_MULTILINE | wxTE_READONLY);

    // Grab standard out
    wxStreamToTextRedirector* textRedirector = new wxStreamToTextRedirector(text);

    // Add to a sizer
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(text, 1, wxEXPAND | wxALL, 5);

    panel->SetSizer(sizer);
    sizer->SetSizeHints(this);
}

RenciLog::~RenciLog() {
}