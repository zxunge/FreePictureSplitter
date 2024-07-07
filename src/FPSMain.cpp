/***************************************************************
 * Purpose:   Code for Application Frame
 * Author:    Grit Clef (3396563372@qq.com)
 * Created:   2024-06-24
 * Copyright: Grit Clef (https://zxunge.github.io)
 * License:   GPL v3
 **************************************************************/

#include "FPSMain.h"
#include "SplitThread.h"
#include "version.h"
#include <wx/statline.h>
#include <wx/msgdlg.h>
#include <wx/log.h>
#include <wx/dirdlg.h>
#include <wx/wxhtml.h>

//(*InternalHeaders(FPSFrame)
#include <wx/bitmap.h>
#include <wx/font.h>
#include <wx/icon.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

// helper functions

//(*IdInit(FPSFrame)
const wxWindowID FPSFrame::ID_STATICTEXTINPUT = wxNewId();
const wxWindowID FPSFrame::ID_STATICTEXTOUTPUT = wxNewId();
const wxWindowID FPSFrame::ID_GAUGESPLIT = wxNewId();
const wxWindowID FPSFrame::ID_STATICTEXTROWS = wxNewId();
const wxWindowID FPSFrame::ID_SPINCTRLROWS = wxNewId();
const wxWindowID FPSFrame::ID_STATICTEXTCOLS = wxNewId();
const wxWindowID FPSFrame::ID_SPINCTRLCOLS = wxNewId();
const wxWindowID FPSFrame::ID_BUTTONSPLIT = wxNewId();
const wxWindowID FPSFrame::ID_BUTTONCANCEL = wxNewId();
const wxWindowID FPSFrame::ID_PANELMAIN = wxNewId();
const wxWindowID FPSFrame::idMenuOpenDir = wxNewId();
const wxWindowID FPSFrame::idMenuOpen = wxNewId();
const wxWindowID FPSFrame::idMenuQuit = wxNewId();
const wxWindowID FPSFrame::idMenuAbout = wxNewId();
const wxWindowID FPSFrame::ID_STATUSBARMAIN = wxNewId();
//*)

SplitThread *splitThread;

BEGIN_EVENT_TABLE(FPSFrame,wxFrame)
    //(*EventTable(FPSFrame)
    //*)
    EVT_THREAD(SPLITTER_EVENT, FPSFrame::OnUpdateGauge)
END_EVENT_TABLE()

FPSFrame::FPSFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(FPSFrame)
    wxBoxSizer* BackgroundSizer;
    wxBoxSizer* ButtonsSizer;
    wxBoxSizer* DirsSizer;
    wxBoxSizer* RowsColsSizer;
    wxMenu* FileMenu;
    wxMenu* HelpMenu;
    wxMenuItem* miAbout;
    wxMenuItem* miQuit;

    Create(parent, id, _("FreePictureSplitter"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
    SetClientSize(wxSize(750,500));
    wxFont thisFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Segoe UI Symbol"),wxFONTENCODING_DEFAULT);
    SetFont(thisFont);
    {
        wxIcon FrameIcon;
        FrameIcon.CopyFromBitmap(wxBitmap(wxImage(_T("resources/FPS.ico"))));
        SetIcon(FrameIcon);
    }
    MainPanel = new wxPanel(this, ID_PANELMAIN, wxPoint(264,104), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANELMAIN"));
    BackgroundSizer = new wxBoxSizer(wxVERTICAL);
    DirsSizer = new wxBoxSizer(wxHORIZONTAL);
    InputDirText = new wxStaticText(MainPanel, ID_STATICTEXTINPUT, _("(Input Directory)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXTINPUT"));
    DirsSizer->Add(InputDirText, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    DirsSizer->Add(123,8,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    OutputDirText = new wxStaticText(MainPanel, ID_STATICTEXTOUTPUT, _("(Output Directory)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXTOUTPUT"));
    DirsSizer->Add(OutputDirText, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BackgroundSizer->Add(DirsSizer, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SplitGauge = new wxGauge(MainPanel, ID_GAUGESPLIT, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_GAUGESPLIT"));
    BackgroundSizer->Add(SplitGauge, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    RowsColsSizer = new wxBoxSizer(wxHORIZONTAL);
    RowsText = new wxStaticText(MainPanel, ID_STATICTEXTROWS, _("Row Number:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXTROWS"));
    RowsColsSizer->Add(RowsText, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    RowsSpin = new wxSpinCtrl(MainPanel, ID_SPINCTRLROWS, _T("0"), wxDefaultPosition, wxDefaultSize, 0, 0, 100, 0, _T("ID_SPINCTRLROWS"));
    RowsSpin->SetValue(_T("0"));
    RowsColsSizer->Add(RowsSpin, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ColsText = new wxStaticText(MainPanel, ID_STATICTEXTCOLS, _("Column Number:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXTCOLS"));
    RowsColsSizer->Add(ColsText, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ColsSpin = new wxSpinCtrl(MainPanel, ID_SPINCTRLCOLS, _T("0"), wxDefaultPosition, wxDefaultSize, 0, 0, 100, 0, _T("ID_SPINCTRLCOLS"));
    ColsSpin->SetValue(_T("0"));
    RowsColsSizer->Add(ColsSpin, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BackgroundSizer->Add(RowsColsSizer, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonsSizer = new wxBoxSizer(wxHORIZONTAL);
    SplitBtn = new wxButton(MainPanel, ID_BUTTONSPLIT, _("Split Now!"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONSPLIT"));
    ButtonsSizer->Add(SplitBtn, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    CancelBtn = new wxButton(MainPanel, ID_BUTTONCANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONCANCEL"));
    ButtonsSizer->Add(CancelBtn, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BackgroundSizer->Add(ButtonsSizer, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    MainPanel->SetSizer(BackgroundSizer);
    MainMenuBar = new wxMenuBar();
    FileMenu = new wxMenu();
    miOpen = new wxMenu();
    miOpenDir = new wxMenuItem(miOpen, idMenuOpenDir, _("Directory\tCtrl-O"), _("Open a directory containing picture files."), wxITEM_NORMAL);
    miOpen->Append(miOpenDir);
    FileMenu->Append(idMenuOpen, _("&Open..."), miOpen, _("Open picture files."));
    miQuit = new wxMenuItem(FileMenu, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    FileMenu->Append(miQuit);
    MainMenuBar->Append(FileMenu, _("&File"));
    HelpMenu = new wxMenu();
    miAbout = new wxMenuItem(HelpMenu, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    HelpMenu->Append(miAbout);
    MainMenuBar->Append(HelpMenu, _("Help"));
    SetMenuBar(MainMenuBar);
    MainStatusBar = new wxStatusBar(this, ID_STATUSBARMAIN, 0, _T("ID_STATUSBARMAIN"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    MainStatusBar->SetFieldsCount(1,__wxStatusBarWidths_1);
    MainStatusBar->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(MainStatusBar);

    Bind(wxEVT_COMMAND_BUTTON_CLICKED, &FPSFrame::OnSplitBtnClick, this, ID_BUTTONSPLIT);
    Bind(wxEVT_COMMAND_BUTTON_CLICKED, &FPSFrame::OnCancelBtnClick, this, ID_BUTTONCANCEL);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &FPSFrame::OnmiOpenDirSelected, this, idMenuOpenDir);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &FPSFrame::OnQuit, this, idMenuQuit);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &FPSFrame::OnAbout, this, idMenuAbout);
    //*)
}

FPSFrame::~FPSFrame()
{
    //(*Destroy(FPSFrame)
    //*)
}

void FPSFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close();
}

void FPSFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxBoxSizer   *topsizer;
    wxHtmlWindow *html;
    wxDialog dlg(this, wxID_ANY, wxString(_("About FreePictureSplitter")));

    topsizer = new wxBoxSizer(wxVERTICAL);

    html = new wxHtmlWindow(&dlg, wxID_ANY, wxDefaultPosition, wxSize(380, 160), wxHW_SCROLLBAR_NEVER);
    html -> SetBorders(0);
    html -> SetPage(wxString(_("                                          \
        <html>                                                            \
        <head>                                                            \
            <meta charset=\"utf-8\" lang=\"en\">                          \
        </head>                                                           \
        <body>                                                            \
            <h1>FreePictureSplitter - A fast picture splitter!</h1>       \
            <p>Author: Grit Clef</p>                                      \
            <p>Contact: 3396563372@qq.com</p>                             \
            <p>Personal Webpage: https://zxunge.github.io</p>             \
            <h3>Version: ") +
    FPSVersion::FULLVERSION_STRING +
    FPSVersion::STATUS +
    _T("</h3>") +
    _T("</body>                                                           \
        </html>                                                           \
    ")));
    html -> SetInitialSize(wxSize(html -> GetInternalRepresentation() -> GetWidth(),
                                  html -> GetInternalRepresentation() -> GetHeight()));

    topsizer -> Add(html, 1, wxALL, 10);

    wxButton *OKBtn = new wxButton(&dlg, wxID_OK, _("OK"));
    OKBtn -> SetDefault();

    topsizer -> Add(OKBtn, 0, wxALL | wxALIGN_RIGHT, 15);
    topsizer -> Add(new wxStaticLine(&dlg, wxID_ANY), 0, wxEXPAND | wxLEFT | wxRIGHT, 10);

    dlg.SetSizer(topsizer);
    topsizer -> Fit(&dlg);

    dlg.ShowModal();
}

void FPSFrame::OnmiOpenDirSelected(wxCommandEvent& WXUNUSED(event))
{
    wxDirDialog dlgInput(nullptr, _("Choose the input directory."), _T("/"));
    if (dlgInput.ShowModal() == wxID_OK)
        InputDirText->SetLabel(dlgInput.GetPath());
}

void FPSFrame::OnSplitBtnClick(wxCommandEvent& WXUNUSED(event))
{
    wxDirDialog dlgOutput(nullptr, _("Choose the output directory"), _T("/"));

    if (dlgOutput.ShowModal() != wxID_OK)
        return;

    OutputDirText->SetLabel(dlgOutput.GetPath());
    splitThread = new SplitThread(InputDirText->GetLabel(), OutputDirText->GetLabel(),
                                  RowsSpin->GetValue(), ColsSpin->GetValue(), this);
    if (splitThread->Create() != wxTHREAD_NO_ERROR)
    {
        wxLogError(_("Can't create thread!"));
        return;
    }
    splitThread->Run();
}

void FPSFrame::OnUpdateGauge(wxThreadEvent &event)
{
    static int count,
               sgRange   {SplitGauge->GetRange()},
               fileCount {splitThread->GetAllFilesCount()};

    switch (event.GetInt())
    {
    case ID_STARTED:
        count = 0;
        break;

    case ID_ENDED_ONE_FILE:
        ++count;
        SplitGauge -> SetValue(int(double(count) / fileCount * sgRange));
        break;

    case ID_ENDED_ALL_FILES:
        wxMessageBox(_("Splitting successfully finished!"), _("FreePictureSplitter Note"), wxICON_INFORMATION, this);
        SplitGauge -> SetValue(0);
        break;

    case ID_CANCELED:
        wxMessageBox(_("Splitting canceled."), _("FreePictureSplitter Note"), wxICON_INFORMATION, this);
        SplitGauge -> SetValue(0);
        break;
    }
}

void FPSFrame::OnCancelBtnClick(wxCommandEvent& WXUNUSED(event))
{
    splitThread->Delete();
}

