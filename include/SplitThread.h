/***************************************************************
 * Purpose:   Code for Split Thread
 * Author:    Grit Clef (3396563372@qq.com)
 * Created:   2024-06-24
 * Copyright: Grit Clef (https://zxunge.github.io)
 * License:   GPL v3
 **************************************************************/
#ifndef SPLITTHREAD_H
#define SPLITTHREAD_H

#include <wx/thread.h>
#include <wx/string.h>
#include "FPSMain.h"

const wxArrayString fileSpecs {_T("*.jpg"), _T("*.png"), _T("*.bmp"), _T("*.jpeg"),
                               _T("*.gif"), _T("*.pcx"), _T("*.pnm"), _T("*.tiff"),
                               _T("*.tga"), _T("*.xpm"), _T("*.ico"), _T("*.cur")};

enum{SPLITTER_EVENT = wxID_HIGHEST + 1,
     ID_ENDED_ONE_FILE,
     ID_ENDED_ALL_FILES,
     ID_STARTED,
     ID_CANCELED};

class SplitThread : public wxThread
{
    public:
        SplitThread(wxString inputDir, wxString outputDir, int rows, int cols, FPSFrame *frame)
            : m_Rows(rows), m_Cols(cols), m_InputDir(inputDir), m_OutputDir(outputDir), m_Frame(frame) {}
        virtual ~SplitThread();
        virtual void *Entry() wxOVERRIDE;

        int GetRows() { return m_Rows; }
        void SetRows(int val) { m_Rows = val; }
        int GetCols() { return m_Cols; }
        void SetCols(int val) { m_Cols = val; }
        wxString GetInputDir() { return m_InputDir; }
        void SetInputDir(wxString val) { m_InputDir = val; }
        int GetFileCount(const wxString &fileSpec);
        int GetAllFilesCount();

    protected:

    private:
        int m_Rows;
        int m_Cols;
        wxString m_InputDir;
        wxString m_OutputDir;
        FPSFrame *m_Frame;
};

#endif // SPLITTHREAD_H
