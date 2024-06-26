/************************************
 *  Copyright (c) 2024, Grit Clef
 */

#include <wx/app.h>
#include <wx/dir.h>
#include "SplitThread.h"
#include "fpsSplit.h"
#include "FPSApp.h"

DECLARE_APP(FPSApp)

SplitThread::~SplitThread()
{
    //dtor
}

int SplitThread::GetFileCount()
{
    // 本函数只考虑当前目录下文件，不递归子文件夹
    int fileCount = 0;
    wxDir dir(m_InputDir);
    wxString filePath;

    bool cont = dir.GetFirst(&filePath, fileSpec, wxDIR_FILES);
    while (cont)
    {
        ++fileCount;
        cont = dir.GetNext(&filePath);
    }

    return fileCount;
}


void *SplitThread::Entry()
{
    wxString inFilePath, inFileName;
    wxDir dirIn(m_InputDir),
          dirOut(m_OutputDir);
    int fileCount = GetFileCount();

    wxThreadEvent eventBeg(wxEVT_THREAD, WORKER_EVENT);
    eventBeg.SetInt(ID_STARTED);
    wxQueueEvent(m_Frame, eventBeg.Clone());

    bool cont = dirIn.GetFirst(&inFileName, fileSpec, wxDIR_FILES);

    for (int i = 1; i <= fileCount && cont; ++i)
    {
        inFilePath = m_InputDir + _T("\\") + inFileName;     // 获取文件名和路径
        fpsSplit(inFilePath, m_OutputDir, m_Rows, m_Cols);

        wxThreadEvent eventOne(wxEVT_THREAD, WORKER_EVENT);
        eventOne.SetInt(ID_ENDED_ONE_FILE);
        wxQueueEvent(m_Frame, eventOne.Clone());

        if (TestDestroy())
        {
            dirIn.Close();
            dirOut.Close();
            wxThreadEvent event(wxEVT_THREAD, WORKER_EVENT);
            event.SetInt(ID_ENDED_ALL_FILES);
            wxQueueEvent(m_Frame, event.Clone());
            return nullptr;
        }

        cont = dirIn.GetNext(&inFileName);
    }
    wxThreadEvent event(wxEVT_THREAD, WORKER_EVENT);
    event.SetInt(ID_ENDED_ALL_FILES);
    wxQueueEvent(m_Frame, event.Clone());

    dirIn.Close();
    dirOut.Close();

    return nullptr;
}
