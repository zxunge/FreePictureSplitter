//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: FreePictureSplitter.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "UiBase.hpp"

// Declare the bitmap loading function
extern void wxC9ED9InitBitmapResources();

namespace
{
// return the wxBORDER_SIMPLE that matches the current application theme
wxBorder get_border_simple_theme_aware_bit()
{
#if wxVERSION_NUMBER >= 3300 && defined(__WXMSW__)
    return wxSystemSettings::GetAppearance().IsDark() ? wxBORDER_SIMPLE : wxBORDER_STATIC;
#else
    return wxBORDER_DEFAULT;
#endif
} // DoGetBorderSimpleBit
bool bBitmapLoaded = false;
} // namespace

MainFrameBase::MainFrameBase(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos,
                             const wxSize& size, long style)
    : wxFrame(parent, id, title, pos, size, style)
{
    if(!bBitmapLoaded) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxC9ED9InitBitmapResources();
        bBitmapLoaded = true;
    }
    // Set icon(s) to the application/dialog
    wxIconBundle app_icons;
    {
        wxBitmap iconBmp = wxXmlResource::Get()->LoadBitmap(wxT("FPS"));
        wxIcon icn;
        icn.CopyFromBitmap(iconBmp);
        app_icons.AddIcon(icn);
    }
    SetIcons(app_icons);

    this->SetFocus();

    wxBoxSizer* panelSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(panelSizer);

    m_mainAuibar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)),
                                    wxAUI_TB_PLAIN_BACKGROUND | wxAUI_TB_HORZ_TEXT | wxAUI_TB_HORZ_LAYOUT |
                                        wxAUI_TB_NO_AUTORESIZE | wxAUI_TB_TEXT);
    m_mainAuibar->SetToolBitmapSize(wxSize(24, 24));

    panelSizer->Add(m_mainAuibar, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_mainAuibar->AddTool(wxID_OPEN, _("Open a picture.."),
                          wxXmlResource::Get()->LoadBitmap(wxT("ic_fluent_image_add_24_regular")), wxNullBitmap,
                          wxITEM_NORMAL, _("Open a picture.."), _("Open a picture.."), NULL);

    m_mainAuibar->AddTool(wxID_ANY, _("Batch Split >>>"),
                          wxXmlResource::Get()->LoadBitmap(wxT("ic_fluent_image_multiple_24_regular")), wxNullBitmap,
                          wxITEM_NORMAL, _("Batch Split"), wxT(""), NULL);
    m_mainAuibar->Realize();

    m_mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), wxTAB_TRAVERSAL);

    panelSizer->Add(m_mainPanel, 1, wxEXPAND, WXC_FROM_DIP(5));

    mainSizer = new wxBoxSizer(wxHORIZONTAL);
    m_mainPanel->SetSizer(mainSizer);

    wxBoxSizer* optionSizer = new wxBoxSizer(wxVERTICAL);

    mainSizer->Add(optionSizer, 0, wxALL, WXC_FROM_DIP(5));

    m_byNumPanel = new wxPanel(m_mainPanel, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_mainPanel, wxSize(-1, -1)),
                               wxTAB_TRAVERSAL | wxBORDER_RAISED);
    m_byNumPanel->SetBackgroundColour(wxColour(wxT("rgb(255,255,236)")));

    optionSizer->Add(m_byNumPanel, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    wxBoxSizer* numOptionSizer = new wxBoxSizer(wxVERTICAL);
    m_byNumPanel->SetSizer(numOptionSizer);

    m_numRadio = new wxRadioButton(m_byNumPanel, wxID_ANY, _("Split picture by row and column counts"),
                                   wxDefaultPosition, wxDLG_UNIT(m_byNumPanel, wxSize(-1, -1)), 0);
    m_numRadio->SetValue(1);

    numOptionSizer->Add(m_numRadio, 0, wxALL, WXC_FROM_DIP(5));

    wxBoxSizer* txtSpinSizerNumRow = new wxBoxSizer(wxHORIZONTAL);

    numOptionSizer->Add(txtSpinSizerNumRow, 1, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_rowsTxt = new wxStaticText(m_byNumPanel, wxID_ANY, _("Number of rows:"), wxDefaultPosition,
                                 wxDLG_UNIT(m_byNumPanel, wxSize(-1, -1)), 0);

    txtSpinSizerNumRow->Add(m_rowsTxt, 0, wxALL, WXC_FROM_DIP(5));

    m_rowSpin = new wxSpinCtrl(m_byNumPanel, wxID_ANY, wxT("1"), wxDefaultPosition,
                               wxDLG_UNIT(m_byNumPanel, wxSize(-1, -1)), wxSP_ARROW_KEYS);
    m_rowSpin->SetRange(1, 101);
    m_rowSpin->SetValue(1);

    txtSpinSizerNumRow->Add(m_rowSpin, 0, wxALL, WXC_FROM_DIP(5));

    wxBoxSizer* txtSpinSizerNumCol = new wxBoxSizer(wxHORIZONTAL);

    numOptionSizer->Add(txtSpinSizerNumCol, 1, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_colTxt = new wxStaticText(m_byNumPanel, wxID_ANY, _("Number of columns:"), wxDefaultPosition,
                                wxDLG_UNIT(m_byNumPanel, wxSize(-1, -1)), 0);

    txtSpinSizerNumCol->Add(m_colTxt, 0, wxALL, WXC_FROM_DIP(5));

    m_colSpin = new wxSpinCtrl(m_byNumPanel, wxID_ANY, wxT("1"), wxDefaultPosition,
                               wxDLG_UNIT(m_byNumPanel, wxSize(-1, -1)), wxSP_ARROW_KEYS);
    m_colSpin->SetRange(1, 101);
    m_colSpin->SetValue(1);

    txtSpinSizerNumCol->Add(m_colSpin, 0, wxALL, WXC_FROM_DIP(5));

    m_staticLine = new wxStaticLine(m_mainPanel, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_mainPanel, wxSize(-1, -1)),
                                    wxLI_HORIZONTAL);

    optionSizer->Add(m_staticLine, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_byPixelPanel = new wxPanel(m_mainPanel, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_mainPanel, wxSize(-1, -1)),
                                 wxTAB_TRAVERSAL | wxBORDER_RAISED);
    m_byPixelPanel->SetBackgroundColour(wxColour(wxT("rgb(255,255,236)")));

    optionSizer->Add(m_byPixelPanel, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    wxBoxSizer* pixelOptionSizer = new wxBoxSizer(wxVERTICAL);
    m_byPixelPanel->SetSizer(pixelOptionSizer);

    m_pixelRadio = new wxRadioButton(m_byPixelPanel, wxID_ANY, _("Split picture by pixels"), wxDefaultPosition,
                                     wxDLG_UNIT(m_byPixelPanel, wxSize(-1, -1)), 0);
    m_pixelRadio->SetValue(0);

    pixelOptionSizer->Add(m_pixelRadio, 0, wxALL, WXC_FROM_DIP(5));

    wxBoxSizer* txtSpinSizerPixHeight = new wxBoxSizer(wxHORIZONTAL);

    pixelOptionSizer->Add(txtSpinSizerPixHeight, 1, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_heightTxt = new wxStaticText(m_byPixelPanel, wxID_ANY, _("Height:"), wxDefaultPosition,
                                   wxDLG_UNIT(m_byPixelPanel, wxSize(-1, -1)), 0);

    txtSpinSizerPixHeight->Add(m_heightTxt, 0, wxALL, WXC_FROM_DIP(5));

    m_heightSpin = new wxSpinCtrl(m_byPixelPanel, wxID_ANY, wxT("0"), wxDefaultPosition,
                                  wxDLG_UNIT(m_byPixelPanel, wxSize(-1, -1)), wxSP_ARROW_KEYS);
    m_heightSpin->Enable(false);
    m_heightSpin->SetRange(0, 100);
    m_heightSpin->SetValue(0);

    txtSpinSizerPixHeight->Add(m_heightSpin, 0, wxALL, WXC_FROM_DIP(5));

    wxBoxSizer* txtSpinSizerPixWidth = new wxBoxSizer(wxHORIZONTAL);

    pixelOptionSizer->Add(txtSpinSizerPixWidth, 1, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_widthTxt = new wxStaticText(m_byPixelPanel, wxID_ANY, _("Width:"), wxDefaultPosition,
                                  wxDLG_UNIT(m_byPixelPanel, wxSize(-1, -1)), 0);

    txtSpinSizerPixWidth->Add(m_widthTxt, 0, wxALL, WXC_FROM_DIP(5));

    m_widthSpin = new wxSpinCtrl(m_byPixelPanel, wxID_ANY, wxT("0"), wxDefaultPosition,
                                 wxDLG_UNIT(m_byPixelPanel, wxSize(-1, -1)), wxSP_ARROW_KEYS);
    m_widthSpin->Enable(false);
    m_widthSpin->SetRange(0, 100);
    m_widthSpin->SetValue(0);

    txtSpinSizerPixWidth->Add(m_widthSpin, 0, wxALL, WXC_FROM_DIP(5));

    m_splitBtn =
        new wxButton(m_mainPanel, wxID_ANY, _("Split!"), wxDefaultPosition, wxDLG_UNIT(m_mainPanel, wxSize(-1, -1)), 0);
    m_splitBtn->SetBackgroundColour(wxColour(wxT("rgb(64,0,64)")));
    m_splitBtn->SetForegroundColour(wxColour(wxT("rgb(255,255,255)")));
    m_splitBtn->Enable(false);

    optionSizer->Add(m_splitBtn, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_saveBtn = new wxButton(m_mainPanel, wxID_ANY, _("Save Splited Pictures"), wxDefaultPosition,
                             wxDLG_UNIT(m_mainPanel, wxSize(-1, -1)), 0);
    m_saveBtn->SetBackgroundColour(wxColour(wxT("rgb(64,0,64)")));
    m_saveBtn->SetForegroundColour(wxColour(wxT("rgb(255,255,255)")));
    m_saveBtn->Enable(false);

    optionSizer->Add(m_saveBtn, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_mainMenuBar = new wxMenuBar(0);
    this->SetMenuBar(m_mainMenuBar);

    m_fileMenu = new wxMenu();
    m_mainMenuBar->Append(m_fileMenu, _("File"));

    m_openItem = new wxMenuItem(m_fileMenu, wxID_OPEN, _("Open a picture\tCtrl-O"),
                                _("Open a picture file for splitting."), wxITEM_NORMAL);
    m_fileMenu->Append(m_openItem);

    m_quitItem = new wxMenuItem(m_fileMenu, wxID_EXIT, _("Exit\tAlt-X"), _("Quit"), wxITEM_NORMAL);
    m_fileMenu->Append(m_quitItem);

    m_helpMenu = new wxMenu();
    m_mainMenuBar->Append(m_helpMenu, _("Help"));

    m_aboutItem = new wxMenuItem(m_helpMenu, wxID_ABOUT, _("About..."), _("About FreePicturSplitter"), wxITEM_NORMAL);
    m_helpMenu->Append(m_aboutItem);

    m_mainStatusBar = new wxStatusBar(this, wxID_ANY, wxSTB_DEFAULT_STYLE);
    m_mainStatusBar->SetFieldsCount(1);
    this->SetStatusBar(m_mainStatusBar);

    SetName(wxT("MainFrameBase"));
    SetMinClientSize(wxSize(1000, 800));
    SetSize(wxDLG_UNIT(this, wxSize(1000, 800)));
    if(GetSizer()) {
        GetSizer()->Fit(this);
    }
    if(GetParent()) {
        CentreOnParent(wxBOTH);
    } else {
        CentreOnScreen(wxBOTH);
    }
    if(!wxPersistenceManager::Get().Find(this)) {
        wxPersistenceManager::Get().RegisterAndRestore(this);
    } else {
        wxPersistenceManager::Get().Restore(this);
    }
    // Connect events
    this->Bind(wxEVT_COMMAND_TOOL_CLICKED, &MainFrameBase::OnOpenFileToolClicked, this, wxID_OPEN);
    this->Bind(wxEVT_COMMAND_TOOL_CLICKED, &MainFrameBase::OnBatchSplitToolClicked, this, wxID_ANY);
    m_numRadio->Bind(wxEVT_COMMAND_RADIOBUTTON_SELECTED, &MainFrameBase::OnNumRadioButtonSelected, this);
    m_pixelRadio->Bind(wxEVT_COMMAND_RADIOBUTTON_SELECTED, &MainFrameBase::OnPixelRadioButtonSelected, this);
    m_splitBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrameBase::OnSplitBtnClicked, this);
    m_saveBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrameBase::OnSaveBtnClicked, this);
    this->Bind(wxEVT_MENU, &MainFrameBase::OnOpenItemMenu, this, m_openItem->GetId());
    this->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrameBase::OnExit, this, m_quitItem->GetId());
    this->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrameBase::OnAbout, this, m_aboutItem->GetId());
}

MainFrameBase::~MainFrameBase()
{
    this->Unbind(wxEVT_COMMAND_TOOL_CLICKED, &MainFrameBase::OnOpenFileToolClicked, this, wxID_OPEN);
    this->Unbind(wxEVT_COMMAND_TOOL_CLICKED, &MainFrameBase::OnBatchSplitToolClicked, this, wxID_ANY);
    m_numRadio->Unbind(wxEVT_COMMAND_RADIOBUTTON_SELECTED, &MainFrameBase::OnNumRadioButtonSelected, this);
    m_pixelRadio->Unbind(wxEVT_COMMAND_RADIOBUTTON_SELECTED, &MainFrameBase::OnPixelRadioButtonSelected, this);
    m_splitBtn->Unbind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrameBase::OnSplitBtnClicked, this);
    m_saveBtn->Unbind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrameBase::OnSaveBtnClicked, this);
    this->Unbind(wxEVT_MENU, &MainFrameBase::OnOpenItemMenu, this, m_openItem->GetId());
    this->Unbind(wxEVT_COMMAND_MENU_SELECTED, &MainFrameBase::OnExit, this, m_quitItem->GetId());
    this->Unbind(wxEVT_COMMAND_MENU_SELECTED, &MainFrameBase::OnAbout, this, m_aboutItem->GetId());
}

fpsBatchSplitDialogBase::fpsBatchSplitDialogBase(wxWindow* parent, wxWindowID id, const wxString& title,
                                                 const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if(!bBitmapLoaded) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxC9ED9InitBitmapResources();
        bBitmapLoaded = true;
    }

    SetName(wxT("fpsBatchSplitDialogBase"));
    SetSize(wxDLG_UNIT(this, wxSize(500, 300)));
    if(GetSizer()) {
        GetSizer()->Fit(this);
    }
    if(GetParent()) {
        CentreOnParent(wxBOTH);
    } else {
        CentreOnScreen(wxBOTH);
    }
    if(!wxPersistenceManager::Get().Find(this)) {
        wxPersistenceManager::Get().RegisterAndRestore(this);
    } else {
        wxPersistenceManager::Get().Restore(this);
    }
}

fpsBatchSplitDialogBase::~fpsBatchSplitDialogBase() {}
