/************************************
 *  Copyright (c) 2024, Grit Clef
 */

#ifndef SPLITTHREAD_H
#define SPLITTHREAD_H

#include <wx/thread.h>
#include <wx/string.h>
#include "FPSMain.h"

enum{WORKER_EVENT = wxID_HIGHEST + 1};

class SplitThread : public wxThread
{
    public:
        SplitThread(wxString inputDir, wxString outputDir, int rows, int cols)
            : m_Rows(rows), m_Cols(cols), m_InputDir(inputDir), m_OutputDir(outputDir) {}
        virtual ~SplitThread();
        virtual void *Entry() wxOVERRIDE;

        int GetRows() { return m_Rows; }
        void SetRows(int val) { m_Rows = val; }
        int GetCols() { return m_Cols; }
        void SetCols(int val) { m_Cols = val; }
        wxString GetInputDir() { return m_InputDir; }
        void SetInputDir(wxString val) { m_InputDir = val; }
        int GetFileCount();

    protected:

    private:
        int m_Rows;
        int m_Cols;
        wxString m_InputDir;
        wxString m_OutputDir;

    public:
        FPSFrame *m_Frame;
};

#define ID_ENDED_ONE_FILE  1000
#define ID_ENDED_ALL_FILES 1001
#define ID_STARTED         1002

#endif // SPLITTHREAD_H
