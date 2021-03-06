/******************************************************************************
*
*  Udacity Nanodegree in C++
*  Memory Management ChatBot - Project 3 
*
*  Work done by Nicol Carstens, February/March 2020
*  Baseline code provided by udacity.com
*
*  Status: ready to submit (7 March 2020)
*
*  Copyright: Nicol Carstens & Udacity 2020
*
******************************************************************************
*
*  TASK 1: Exclusive Ownership 1
*  
*  In file chatgui.h / chatgui.cpp, _chatLogic is made an exclusive 
*  resource to class ChatbotPanelDialog using an appropriate smart pointer. 
*  Where required, changes are made to the code such that data structures 
*  and function parameters reflect the new structure.
*
******************************************************************************/

#ifndef CHATGUI_H_
#define CHATGUI_H_

#include <wx/wx.h>
#include <memory>

class ChatLogic; // forward declaration

// middle part of the window containing the dialog between user and chatbot
class ChatBotPanelDialog : public wxScrolledWindow
{
private:
    // control elements
    wxBoxSizer *_dialogSizer;
    wxBitmap _image;

    //// STUDENT CODE - Task 1: Exclusive Ownership 1
    ////

    // REPLACED NC: ChatLogic *_chatLogic;
    std::unique_ptr<ChatLogic> _chatLogic;

    ////
    //// EOF STUDENT CODE

public:
    // constructor / destructor
    ChatBotPanelDialog(wxWindow *parent, wxWindowID id);
    ~ChatBotPanelDialog();

    // getter / setter
    // Task 1: Exclusive Ownership 1
    // Is this safe to do? Works but doesn't look safe?
    //  -> Well, raw pointer and reference access to object managed using smart 
    //     pointers is unsafe if you are careless. Pay attention to ownership.
    // ChatBotPanelDialog exclusively owns _chatLogic
    //  -> this function inferface makes it clear that ownership is not transferred
    // Return a reference not a pointer... right? Could return pointer or reference.    
    // REPLACED NC: ChatLogic *GetChatLogicHandle() { return _chatLogic.get(); }     
    // ALTERNATIVE: ChatLogic &GetChatLogicHandle() { return *_chatLogic; }
    ChatLogic &GetChatLogicHandle() { return *_chatLogic; }

    // events
    void paintEvent(wxPaintEvent &evt);
    void paintNow();
    void render(wxDC &dc);

    // proprietary functions
    void AddDialogItem(wxString text, bool isFromUser = true);
    void PrintChatbotResponse(std::string response);

    DECLARE_EVENT_TABLE()
};

// dialog item shown in ChatBotPanelDialog
class ChatBotPanelDialogItem : public wxPanel
{
private:
    // control elements
    wxStaticBitmap *_chatBotImg;
    wxStaticText *_chatBotTxt;

public:
    // constructor / destructor
    ChatBotPanelDialogItem(wxPanel *parent, wxString text, bool isFromUser);
};

// frame containing all control elements
class ChatBotFrame : public wxFrame
{
private:
    // control elements
    ChatBotPanelDialog *_panelDialog;
    wxTextCtrl *_userTextCtrl;

    // events
    void OnEnter(wxCommandEvent &WXUNUSED(event));

public:
    // constructor / desctructor
    ChatBotFrame(const wxString &title);
};

// control panel for background image display
class ChatBotFrameImagePanel : public wxPanel
{
    // control elements
    wxBitmap _image;

public:
    // constructor / desctructor
    ChatBotFrameImagePanel(wxFrame *parent);

    // events
    void paintEvent(wxPaintEvent &evt);
    void paintNow();
    void render(wxDC &dc);

    DECLARE_EVENT_TABLE()
};

// wxWidgets app that hides main()
class ChatBotApp : public wxApp
{
public:
    // events
    virtual bool OnInit();
};

#endif /* CHATGUI_H_ */
