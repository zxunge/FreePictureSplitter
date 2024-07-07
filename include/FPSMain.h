/***************************************************************
 * Purpose:   Defines Application Frame
 * Author:    Grit Clef (3396563372@qq.com)
 * Created:   2024-06-24
 * Copyright: Grit Clef (https://zxunge.github.io)
 * License:   GPL v3
 **************************************************************/

#ifndef FPSMAIN_H
#define FPSMAIN_H

//(*Headers(FPSFrame)
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/gauge.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/statusbr.h>
//*)

class FPSFrame: public wxFrame
{
    public:

        FPSFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~FPSFrame();

    private:

        //(*Handlers(FPSFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnmiOpenDirSelected(wxCommandEvent& event);
        void OnSplitBtnClick(wxCommandEvent& event);
        void OnCancelBtnClick(wxCommandEvent& event);
        //*)
        void OnUpdateGauge(wxThreadEvent &event);

        //(*Identifiers(FPSFrame)
        static const wxWindowID ID_STATICTEXTINPUT;
        static const wxWindowID ID_STATICTEXTOUTPUT;
        static const wxWindowID ID_GAUGESPLIT;
        static const wxWindowID ID_STATICTEXTROWS;
        static const wxWindowID ID_SPINCTRLROWS;
        static const wxWindowID ID_STATICTEXTCOLS;
        static const wxWindowID ID_SPINCTRLCOLS;
        static const wxWindowID ID_BUTTONSPLIT;
        static const wxWindowID ID_BUTTONCANCEL;
        static const wxWindowID ID_PANELMAIN;
        static const wxWindowID idMenuOpenDir;
        static const wxWindowID idMenuOpen;
        static const wxWindowID idMenuQuit;
        static const wxWindowID idMenuAbout;
        static const wxWindowID ID_STATUSBARMAIN;
        //*)

        //(*Declarations(FPSFrame)
        wxButton* CancelBtn;
        wxButton* SplitBtn;
        wxGauge* SplitGauge;
        wxMenu* miOpen;
        wxMenuBar* MainMenuBar;
        wxMenuItem* miOpenDir;
        wxPanel* MainPanel;
        wxSpinCtrl* ColsSpin;
        wxSpinCtrl* RowsSpin;
        wxStaticText* ColsText;
        wxStaticText* InputDirText;
        wxStaticText* OutputDirText;
        wxStaticText* RowsText;
        wxStatusBar* MainStatusBar;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // FPSMAIN_H
