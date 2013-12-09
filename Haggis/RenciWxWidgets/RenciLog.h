///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        RenciLog.h
//
// Author:      David Borland
//
// Description: Uses a wxTextCtrl and wxStreamToTextRedirector to enable printing of messages
//              sent to C++ ostreams
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#ifndef RENCILOG_H
#define RENCILOG_H


#include <wx/wx.h>


class RenciLog : public wxFrame {
public:
    RenciLog(wxWindow* parent, const wxString& title = "Log Window", 
           const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
    virtual ~RenciLog();
};


#endif
