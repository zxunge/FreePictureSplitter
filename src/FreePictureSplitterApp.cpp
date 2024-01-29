/***************************************************************
 * Name:      FreePictureSplitterApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Grit Clef (3396563372@qq.com)
 * Created:   2024-01-25
 * Copyright: Grit Clef (zxunge.github.io)
 * License:
 **************************************************************/

#include "FreePictureSplitterApp.h"

//(*AppHeaders
#include "FreePictureSplitterMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(FreePictureSplitterApp);

bool FreePictureSplitterApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
        FreePictureSplitterFrame* Frame = new FreePictureSplitterFrame(0);
        Frame->Show();
        SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
