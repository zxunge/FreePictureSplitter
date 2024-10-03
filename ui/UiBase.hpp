//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: FreePictureSplitter.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef _FREEPICTURESPLITTER_FREEPICTURESPLITTER_FREEPICTURESPLITTER_BASE_CLASSES_HPP
#define _FREEPICTURESPLITTER_FREEPICTURESPLITTER_FREEPICTURESPLITTER_BASE_CLASSES_HPP

// clang-format off
#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/frame.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/pen.h>
#include <wx/aui/auibar.h>
#include <map>
#include <wx/menu.h>
#include <wx/toolbar.h>
#include <wx/panel.h>
#include <wx/radiobut.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/statusbr.h>
#include <wx/dialog.h>
#include <wx/scrolwin.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

#ifdef WXC_FROM_DIP
#undef WXC_FROM_DIP
#endif
#if wxVERSION_NUMBER >= 3100
#define WXC_FROM_DIP(x) wxWindow::FromDIP(x, NULL)
#else
#define WXC_FROM_DIP(x) x
#endif

// clang-format on

class MainFrameBase : public wxFrame
{
protected:
    wxAuiToolBar* m_mainAuibar;
    wxPanel* m_mainPanel;
    wxBoxSizer* mainSizer;
    wxPanel* m_byNumPanel;
    wxRadioButton* m_numRadio;
    wxStaticText* m_rowsTxt;
    wxSpinCtrl* m_rowSpin;
    wxStaticText* m_colTxt;
    wxSpinCtrl* m_colSpin;
    wxStaticLine* m_staticLine;
    wxPanel* m_byPixelPanel;
    wxRadioButton* m_pixelRadio;
    wxStaticText* m_heightTxt;
    wxSpinCtrl* m_heightSpin;
    wxStaticText* m_widthTxt;
    wxSpinCtrl* m_widthSpin;
    wxButton* m_splitBtn;
    wxButton* m_saveBtn;
    wxMenuBar* m_mainMenuBar;
    wxMenu* m_fileMenu;
    wxMenuItem* m_openItem;
    wxMenuItem* m_quitItem;
    wxMenu* m_helpMenu;
    wxMenuItem* m_aboutItem;
    wxStatusBar* m_mainStatusBar;

protected:
    virtual void OnOpenFileToolClicked(wxCommandEvent& event) { event.Skip(); }
    virtual void OnBatchSplitToolClicked(wxCommandEvent& event) { event.Skip(); }
    virtual void OnNumRadioButtonSelected(wxCommandEvent& event) { event.Skip(); }
    virtual void OnPixelRadioButtonSelected(wxCommandEvent& event) { event.Skip(); }
    virtual void OnSplitBtnClicked(wxCommandEvent& event) { event.Skip(); }
    virtual void OnSaveBtnClicked(wxCommandEvent& event) { event.Skip(); }
    virtual void OnOpenItemMenu(wxCommandEvent& event) { event.Skip(); }
    virtual void OnExit(wxCommandEvent& event) { event.Skip(); }
    virtual void OnAbout(wxCommandEvent& event) { event.Skip(); }

public:
    wxAuiToolBar* GetMainAuibar() { return m_mainAuibar; }
    wxRadioButton* GetNumRadio() { return m_numRadio; }
    wxStaticText* GetRowsTxt() { return m_rowsTxt; }
    wxSpinCtrl* GetRowSpin() { return m_rowSpin; }
    wxStaticText* GetColTxt() { return m_colTxt; }
    wxSpinCtrl* GetColSpin() { return m_colSpin; }
    wxPanel* GetByNumPanel() { return m_byNumPanel; }
    wxStaticLine* GetStaticLine() { return m_staticLine; }
    wxRadioButton* GetPixelRadio() { return m_pixelRadio; }
    wxStaticText* GetHeightTxt() { return m_heightTxt; }
    wxSpinCtrl* GetHeightSpin() { return m_heightSpin; }
    wxStaticText* GetWidthTxt() { return m_widthTxt; }
    wxSpinCtrl* GetWidthSpin() { return m_widthSpin; }
    wxPanel* GetByPixelPanel() { return m_byPixelPanel; }
    wxButton* GetSplitBtn() { return m_splitBtn; }
    wxButton* GetSaveBtn() { return m_saveBtn; }
    wxPanel* GetMainPanel() { return m_mainPanel; }
    wxMenuBar* GetMainMenuBar() { return m_mainMenuBar; }
    wxStatusBar* GetMainStatusBar() { return m_mainStatusBar; }
    MainFrameBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("FreePictureSplitter"),
                  const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(1000, 800),
                  long style = wxCAPTION | wxRESIZE_BORDER | wxMAXIMIZE_BOX | wxMINIMIZE_BOX | wxSYSTEM_MENU |
                               wxCLOSE_BOX | wxFULL_REPAINT_ON_RESIZE);
    virtual ~MainFrameBase();
};

class fpsBatchSplitDialogBase : public wxDialog
{
protected:
    wxPanel* m_mainPanel;
    wxPanel* m_byNumPanel;
    wxRadioButton* m_numRadio;
    wxStaticText* m_rowsTxt;
    wxSpinCtrl* m_rowSpin;
    wxStaticText* m_colsTxt;
    wxSpinCtrl* m_colSpin;
    wxStaticLine* m_staticLine;
    wxPanel* m_byPixelPanel;
    wxRadioButton* m_pixelRadio;
    wxStaticText* m_heightTxt;
    wxSpinCtrl* m_heightSpin;
    wxStaticText* m_widthTxt;
    wxSpinCtrl* m_widthSpin;
    wxScrolledWindow* m_scPicsWnd;
    wxPanel* m_filePanel;
    wxStaticText* m_pathTxt;
    wxTextCtrl* m_pathCtrl;
    wxButton* m_browseBtn;
    wxCheckBox* m_indDirChkBox;

protected:
public:
    wxRadioButton* GetNumRadio() { return m_numRadio; }
    wxStaticText* GetRowsTxt() { return m_rowsTxt; }
    wxSpinCtrl* GetRowSpin() { return m_rowSpin; }
    wxStaticText* GetColsTxt() { return m_colsTxt; }
    wxSpinCtrl* GetColSpin() { return m_colSpin; }
    wxPanel* GetByNumPanel() { return m_byNumPanel; }
    wxStaticLine* GetStaticLine() { return m_staticLine; }
    wxRadioButton* GetPixelRadio() { return m_pixelRadio; }
    wxStaticText* GetHeightTxt() { return m_heightTxt; }
    wxSpinCtrl* GetHeightSpin() { return m_heightSpin; }
    wxStaticText* GetWidthTxt() { return m_widthTxt; }
    wxSpinCtrl* GetWidthSpin() { return m_widthSpin; }
    wxPanel* GetByPixelPanel() { return m_byPixelPanel; }
    wxScrolledWindow* GetScPicsWnd() { return m_scPicsWnd; }
    wxStaticText* GetPathTxt() { return m_pathTxt; }
    wxTextCtrl* GetPathCtrl() { return m_pathCtrl; }
    wxButton* GetBrowseBtn() { return m_browseBtn; }
    wxCheckBox* GetIndDirChkBox() { return m_indDirChkBox; }
    wxPanel* GetFilePanel() { return m_filePanel; }
    wxPanel* GetMainPanel() { return m_mainPanel; }
    fpsBatchSplitDialogBase(wxWindow* parent, wxWindowID id = wxID_ANY,
                            const wxString& title = _("FreePictureSplitter - Batch Split"),
                            const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(800, 600),
                            long style = wxDEFAULT_DIALOG_STYLE);
    virtual ~fpsBatchSplitDialogBase();
};

#endif
