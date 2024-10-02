#include "MainFrame.hpp"
#include <wx/app.h>
#include <wx/event.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/stdpaths.h>

// Define the MainApp
class MainApp : public wxApp
{
private:
    wxLocale m_locale;
    
public:
    MainApp() {}
    virtual ~MainApp() {}

    virtual bool OnInit()
    {
        wxInitAllImageHandlers();
        
        m_locale.Init(wxLANGUAGE_DEFAULT);
#ifdef __WXMSW__
        m_locale.AddCatalogLookupPathPrefix(wxFileName(wxStandardPaths::Get().GetExecutablePath()).GetPath() + wxT("\\share\\locale"));
#endif // __WXMSW__
        m_locale.AddCatalog(wxT("wxstd"));
        m_locale.AddCatalog(wxT("fps")); 
        
        MainFrame* mainFrame = new MainFrame{ nullptr };
        SetTopWindow(mainFrame);
        return GetTopWindow()->Show();
    }
};

DECLARE_APP(MainApp)
IMPLEMENT_APP(MainApp)
