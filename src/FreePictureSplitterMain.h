#ifndef HEADER_B42029D1FF2EE580
#define HEADER_B42029D1FF2EE580

/***************************************************************
 * Name:      FreePictureSplitterMain.h
 * Purpose:   Defines Application Frame
 * Author:    Grit Clef (3396563372@qq.com)
 * Created:   2024-01-25
 * Copyright: Grit Clef (zxunge.github.io)
 * License:
 **************************************************************/

#ifndef FREEPICTURESPLITTERMAIN_H
#define FREEPICTURESPLITTERMAIN_H

//(*Headers(FreePictureSplitterFrame)
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

class FreePictureSplitterFrame: public wxFrame
{
    public:

        FreePictureSplitterFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~FreePictureSplitterFrame();
        // 分割主函数，rows为分割行数，cols为分割列数，InputDir为欲分割图片所在文件夹
        // 返回0为正常，-1为异常
        friend int fpsSplit(FreePictureSplitterFrame*fpsf, int rows, int cols, const wxString InputDir);

    private:

        //(*Handlers(FreePictureSplitterFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnOpenDirectorySelected(wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);
        void OnbtnBeginSplittingClick(wxCommandEvent& event);
        //*)

        //(*Identifiers(FreePictureSplitterFrame)
        static const wxWindowID ID_STATICTEXT3;
        static const wxWindowID ID_GAUGE1;
        static const wxWindowID ID_STATICTEXT1;
        static const wxWindowID ID_SPINCTRL1;
        static const wxWindowID ID_STATICTEXT2;
        static const wxWindowID ID_SPINCTRL2;
        static const wxWindowID ID_BUTTON1;
        static const wxWindowID ID_PANEL1;
        static const wxWindowID idMenuOpen;
        static const wxWindowID idMenuQuit;
        static const wxWindowID idMenuAbout;
        static const wxWindowID ID_STATUSBAR1;
        //*)

        //(*Declarations(FreePictureSplitterFrame)
        wxButton* btnBeginSplitting;
        wxGauge* SplitGauge;
        wxMenuItem* MenuItem3;
        wxPanel* Panel1;
        wxSpinCtrl* scCols;
        wxSpinCtrl* scRows;
        wxStaticText* StaticText1;
        wxStaticText* StaticText2;
        wxStaticText* stxtPath;
        wxStatusBar* MainStatusBar;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // FREEPICTURESPLITTERMAIN_H
#endif // header guard

