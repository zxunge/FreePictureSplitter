/***************************************************************
 * Name:      FreePictureSplitterMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Grit Clef (3396563372@qq.com)
 * Created:   2024-01-25
 * Copyright: Grit Clef (zxunge.github.io)
 * License:
 **************************************************************/

#include "FreePictureSplitterMain.h"
#include "FreePictureSplitter.h"
#include <wx/msgdlg.h>
#include <wx/dirdlg.h>
#include <wx/wxhtml.h>
#include <wx/statline.h>
#include "FPS.xpm"


//(*InternalHeaders(FreePictureSplitterFrame)
#include <wx/bitmap.h>
#include <wx/icon.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(FreePictureSplitterFrame)
const wxWindowID FreePictureSplitterFrame::ID_STATICTEXT3 = wxNewId();
const wxWindowID FreePictureSplitterFrame::ID_GAUGE1 = wxNewId();
const wxWindowID FreePictureSplitterFrame::ID_STATICTEXT1 = wxNewId();
const wxWindowID FreePictureSplitterFrame::ID_SPINCTRL1 = wxNewId();
const wxWindowID FreePictureSplitterFrame::ID_STATICTEXT2 = wxNewId();
const wxWindowID FreePictureSplitterFrame::ID_SPINCTRL2 = wxNewId();
const wxWindowID FreePictureSplitterFrame::ID_BUTTON1 = wxNewId();
const wxWindowID FreePictureSplitterFrame::ID_PANEL1 = wxNewId();
const wxWindowID FreePictureSplitterFrame::idMenuOpen = wxNewId();
const wxWindowID FreePictureSplitterFrame::idMenuQuit = wxNewId();
const wxWindowID FreePictureSplitterFrame::idMenuAbout = wxNewId();
const wxWindowID FreePictureSplitterFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(FreePictureSplitterFrame,wxFrame)
    //(*EventTable(FreePictureSplitterFrame)
    //*)
END_EVENT_TABLE()

FreePictureSplitterFrame::FreePictureSplitterFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(FreePictureSplitterFrame)
    wxBoxSizer* BoxSizer1;
    wxBoxSizer* BoxSizer2;
    wxMenu* HelpMenu;
    wxMenu* Menu1;
    wxMenuBar* MainMenuBar;
    wxMenuItem* MenuItem1;
    wxMenuItem* miAbout;

    Create(parent, id, _("FreePictureSplitter"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
    SetClientSize(wxSize(506,189));
    SetIcon(wxIcon(FPS_xpm));
    Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(272,176), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    BoxSizer1 = new wxBoxSizer(wxVERTICAL);
    stxtPath = new wxStaticText(Panel1, ID_STATICTEXT3, _("无"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    BoxSizer1->Add(stxtPath, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SplitGauge = new wxGauge(Panel1, ID_GAUGE1, 100, wxDefaultPosition, wxSize(387,42), 0, wxDefaultValidator, _T("ID_GAUGE1"));
    BoxSizer1->Add(SplitGauge, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT1, _("行数："), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    BoxSizer2->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    scRows = new wxSpinCtrl(Panel1, ID_SPINCTRL1, _T("0"), wxDefaultPosition, wxDefaultSize, 0, 0, 100, 0, _T("ID_SPINCTRL1"));
    scRows->SetValue(_T("0"));
    BoxSizer2->Add(scRows, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText2 = new wxStaticText(Panel1, ID_STATICTEXT2, _("列数："), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    BoxSizer2->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    scCols = new wxSpinCtrl(Panel1, ID_SPINCTRL2, _T("0"), wxDefaultPosition, wxDefaultSize, 0, 0, 100, 0, _T("ID_SPINCTRL2"));
    scCols->SetValue(_T("0"));
    BoxSizer2->Add(scCols, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    btnBeginSplitting = new wxButton(Panel1, ID_BUTTON1, _("开始分割"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    BoxSizer2->Add(btnBeginSplitting, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1->Add(BoxSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Panel1->SetSizer(BoxSizer1);
    MainMenuBar = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem3 = new wxMenuItem(Menu1, idMenuOpen, _("打开文件夹"), _("打开一个包含图片的文件夹"), wxITEM_NORMAL);
    Menu1->Append(MenuItem3);
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("退出\tAlt-F4"), _("退出FreePictureSplitter"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MainMenuBar->Append(Menu1, _("文件(&F)"));
    HelpMenu = new wxMenu();
    miAbout = new wxMenuItem(HelpMenu, idMenuAbout, _("关于\tF1"), _("显示应用程序信息"), wxITEM_NORMAL);
    HelpMenu->Append(miAbout);
    MainMenuBar->Append(HelpMenu, _("帮助"));
    SetMenuBar(MainMenuBar);
    MainStatusBar = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    MainStatusBar->SetFieldsCount(1,__wxStatusBarWidths_1);
    MainStatusBar->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(MainStatusBar);

    Connect(ID_BUTTON1, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&FreePictureSplitterFrame::OnbtnBeginSplittingClick);
    Connect(idMenuOpen, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&FreePictureSplitterFrame::OnOpenDirectorySelected);
    Connect(idMenuQuit, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&FreePictureSplitterFrame::OnQuit);
    Connect(idMenuAbout, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&FreePictureSplitterFrame::OnAbout);
    Connect(wxID_ANY, wxEVT_CLOSE_WINDOW, (wxObjectEventFunction)&FreePictureSplitterFrame::OnClose);
    //*)
}

FreePictureSplitterFrame::~FreePictureSplitterFrame()
{
    //(*Destroy(FreePictureSplitterFrame)
    //*)
}

void FreePictureSplitterFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void FreePictureSplitterFrame::OnAbout(wxCommandEvent& event)
{
    wxBoxSizer *topsizer;
    wxHtmlWindow *html;
    wxDialog dlg(this, wxID_ANY, wxString(_("FreePictureSplitter帮助")));

    topsizer = new wxBoxSizer(wxVERTICAL);

    html = new wxHtmlWindow(&dlg, wxID_ANY, wxDefaultPosition, wxSize(380, 160), wxHW_SCROLLBAR_NEVER);
    html -> SetBorders(0);
    html -> LoadPage("data/about.html");
    html -> SetHTMLBackgroundImage(wxBitmapBundle::FromSVGFile("data/bg.svg", wxSize(65, 45)));
    html -> SetInitialSize(wxSize(html -> GetInternalRepresentation() -> GetWidth(),
                                  html -> GetInternalRepresentation() -> GetHeight()));

    topsizer -> Add(html, 1, wxALL, 10);

#if wxUSE_STATLINE
    topsizer -> Add(new wxStaticLine(&dlg, wxID_ANY), 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
#endif // wxUSE_STATLINE

    wxButton *bu1 = new wxButton(&dlg, wxID_OK, _("OK"));
    bu1 -> SetDefault();

    topsizer -> Add(bu1, 0, wxALL | wxALIGN_RIGHT, 15);

    dlg.SetSizer(topsizer);
    topsizer -> Fit(&dlg);

    dlg.ShowModal();
}

void FreePictureSplitterFrame::OnOpenDirectorySelected(wxCommandEvent& event)
{
    wxDirDialog ChooseInputDir(nullptr, _("选择图片所在文件夹"), _("/"));

    if (ChooseInputDir.ShowModal() == wxID_OK)
        stxtPath -> SetLabel(ChooseInputDir.GetPath());
}

void FreePictureSplitterFrame::OnClose(wxCloseEvent& event)
{
    event.Skip(true);
}

void FreePictureSplitterFrame::OnbtnBeginSplittingClick(wxCommandEvent& event)
{
    // TODO (Grit Clef#1#2024/1/27): 另外创建一个线程调用fpsSplit()，之后，禁用“开始分割”按钮，使主窗口仍能响应输入。
    if (fpsSplit(this, scRows -> GetValue(), scCols -> GetValue(), stxtPath -> GetLabel()))
        wxMessageBox(_("抱歉，分割出现错误，请重试！"), _("FreePictureSplitter"));
}
