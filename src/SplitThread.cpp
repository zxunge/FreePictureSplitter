/***************************************************************
 * Purpose:   Code for Split Thread
 * Author:    Grit Clef (3396563372@qq.com)
 * Created:   2024-06-24
 * Copyright: Grit Clef (https://zxunge.github.io)
 * License:   GPL v3
 **************************************************************/

#include <wx/app.h>
#include <wx/dir.h>
#include "SplitThread.h"
#include "fpsSplit.h"
#include "FPSApp.h"

DECLARE_APP(FPSApp)

static inline void SendEvent(FPSFrame *frame, int id)
{
    wxThreadEvent event(wxEVT_THREAD, SPLITTER_EVENT);
    event.SetInt(id);
    wxQueueEvent(frame, event.Clone());
}

SplitThread::~SplitThread()
{
    //dtor
}

int SplitThread::GetFileCount(const wxString &fileSpec)
{
    // 本函数只考虑当前目录下文件，不递归子文件夹
    int fileCount {0};
    wxDir dir(m_InputDir);
    wxString filePath;

    bool cont {dir.GetFirst(&filePath, fileSpec, wxDIR_FILES)};
    while (cont)
    {
        ++fileCount;
        cont = dir.GetNext(&filePath);
    }

    return fileCount;
}

int SplitThread::GetAllFilesCount()
{
    int fileCount {0};
    for (const wxString &fileSpec : fileSpecs)
        fileCount += GetFileCount(fileSpec);
    return fileCount;
}

void *SplitThread::Entry()
{
    wxString inFilePath, inFileName;
    wxDir    dirIn(m_InputDir),
             dirOut(m_OutputDir);
    int      fileCount {0};
    bool     cont      {false};

    SendEvent(m_Frame, ID_STARTED);

    for (const wxString &fileSpec : fileSpecs)
    {
        fileCount = GetFileCount(fileSpec);
        cont = dirIn.GetFirst(&inFileName, fileSpec, wxDIR_FILES);

        for (int i {1}; i <= fileCount && cont; ++i)
        {
            inFilePath = m_InputDir + _T("\\") + inFileName;     // 获取文件名和路径
            fpsSplit(inFilePath, m_OutputDir, m_Rows, m_Cols);

            SendEvent(m_Frame, ID_ENDED_ONE_FILE);

            if (TestDestroy())
            {
                dirIn.Close();
                dirOut.Close();
                SendEvent(m_Frame, ID_CANCELED);
                return nullptr;
            }

            cont = dirIn.GetNext(&inFileName);
        }
    }
    dirIn.Close();
    dirOut.Close();

    SendEvent(m_Frame, ID_ENDED_ALL_FILES);

    return nullptr;
}
