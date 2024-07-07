/***************************************************************
 * Purpose:   Code for Application Class
 * Author:    Grit Clef (3396563372@qq.com)
 * Created:   2024-06-24
 * Copyright: Grit Clef (https://zxunge.github.io)
 * License:   GPL v3
 **************************************************************/

#include "FPSApp.h"

#if !wxUSE_THREADS
    #error "This application requires thread support!"
#endif // wxUSE_THREADS

//(*AppHeaders
#include "FPSMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(FPSApp);

bool FPSApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
        FPSFrame* Frame = new FPSFrame(0);
        Frame->Show();
        SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
