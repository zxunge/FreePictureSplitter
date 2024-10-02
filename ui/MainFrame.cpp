/***************************************************************
 * Purpose:   Code for Main Frame
 * Author:    Grit Clef (3396563372@qq.com)
 * Created:   2024-08-08
 * Copyright: Grit Clef (https://zxunge.github.io)
 * License:   GPL v3
 **************************************************************/

#include "MainFrame.hpp"
#include "config.h"
#include "fpsBatchSplitDialog.hpp"
#include <vector>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/dcclient.h>
#include <wx/dcbuffer.h>
#include <wx/msgdlg.h>
#include <wx/wxhtml.h>
#include <wx/filedlg.h>
#include <wx/bitmap.h>

using std::vector;

MainFrame::MainFrame(wxWindow* parent)
    : MainFrameBase(parent)
{
    // ctor
    m_imagePanel = new wxImagePanel(m_mainPanel);
    m_imagePanel->SetBackgroundColour(wxColour(wxT("rgb(112,112,112)")));

    mainSizer->Add(m_imagePanel, 1, wxALL | wxEXPAND, WXC_FROM_DIP(5));
}

MainFrame::~MainFrame()
{
    // dtor
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxBoxSizer   *topsizer;
    wxHtmlWindow *html;
    wxDialog dlg(this, wxID_ANY, wxString(_("About FreePictureSplitter")));

    topsizer = new wxBoxSizer(wxVERTICAL);

    html = new wxHtmlWindow(&dlg, wxID_ANY, wxDefaultPosition, wxSize(380, 160), wxHW_SCROLLBAR_NEVER);
    html -> SetBorders(0);
    html -> SetPage(wxString(_("                                          \
        <html>                                                            \
        <head>                                                            \
            <meta charset=\"utf-8\" lang=\"en\">                          \
        </head>                                                           \
        <body>                                                            \
            <h1>FreePictureSplitter - A fast picture splitter!</h1>       \
            <p>Author: Grit Clef</p>                                      \
            <p>Contact: 3396563372@qq.com</p>                             \
            <p>Project Site:                                              \
                <a href=\"https://github.com/zxunge/FreePictureSplitter\">\
                FreePictureSplitter - GitHub</a></p>                      \
            <h4>Version: ") +
    fpsFULLVERSION_STRING   + _T(" ") +
    fpsSTATUS               +
    _T("</h3>")             +
    _T("</body>                                                           \
        </html>                                                           \
    ")));
    html -> SetInitialSize(wxSize(html -> GetInternalRepresentation() -> GetWidth(),
                                  html -> GetInternalRepresentation() -> GetHeight()));

    topsizer -> Add(html, 1, wxALL, 10);

    wxButton *OKBtn = new wxButton(&dlg, wxID_OK, _("OK"));
    OKBtn -> SetDefault();

    topsizer -> Add(OKBtn, 0, wxALL | wxALIGN_RIGHT, 15);
    topsizer -> Add(new wxStaticLine(&dlg, wxID_ANY), 0, wxEXPAND | wxLEFT | wxRIGHT, 10);

    dlg.SetSizer(topsizer);
    topsizer -> Fit(&dlg);

    dlg.ShowModal();
}

void MainFrame::OnExit(wxCommandEvent& event)
{
    Close();
}


void MainFrame::OnNumRadioButtonSelected(wxCommandEvent & event)
{
    m_pixelRadio->SetValue(false);
    m_colSpin->Enable();
    m_rowSpin->Enable();
    m_heightSpin->Enable(false);
    m_widthSpin->Enable(false);
}

void MainFrame::OnPixelRadioButtonSelected(wxCommandEvent& event)
{
    m_numRadio->SetValue(false);
    m_rowSpin->Enable(false);
    m_colSpin->Enable(false);
    m_heightSpin->Enable();
    m_widthSpin->Enable();
}

void MainFrame::OnBatchSplitToolClicked(wxCommandEvent& event)
{
    fpsBatchSplitDialog *batchSplitDlg = new fpsBatchSplitDialog(this);
    batchSplitDlg->Show();
}

void MainFrame::OnOpenItemMenu(wxCommandEvent& event)
{
    wxString path;
    wxFileDialog dlgInputFile(nullptr, _("Choose the input file."), _T("/"));
    if (dlgInputFile.ShowModal() != wxID_OK)
        return;
    path = dlgInputFile.GetPath();
    if (!m_image.LoadFile(path))
        return;
    
    m_imagePanel->loadImage(path);
    m_splitBtn->Enable();
}

void MainFrame::OnOpenFileToolClicked(wxCommandEvent& event)
{
    OnOpenItemMenu(event);
}

void MainFrame::OnSplitBtnClicked(wxCommandEvent& event)
{
    vector<vector<wxRect>> subRects;
    wxMemoryDC dc;
    size_t cRows {}, cCols {};
    
    dc.SelectObject(m_imagePanel->renderedBitmap);
    
    if (m_pixelRadio->GetValue())
        subRects = fpsImage::AuxGetSubRects(m_imagePanel->renderedBitmap.GetWidth(),
                                            m_imagePanel->renderedBitmap.GetHeight(),
                                            m_heightSpin->GetValue(), 
                                            m_widthSpin->GetValue(), fpsSplitMode::byPixel);
    else
        subRects = fpsImage::AuxGetSubRects(m_imagePanel->renderedBitmap.GetWidth(),
                                            m_imagePanel->renderedBitmap.GetHeight(),
                                            m_rowSpin->GetValue(), 
                                            m_colSpin->GetValue());
    
    if (subRects.empty())
        return;
        
    m_image.SetSubRects(subRects);        
    cRows = subRects.size();
    cCols = subRects[0].size();
        
    wxPen pen;
    pen.SetColour(wxColour(178, 34, 34));
    pen.SetWidth(5);
    dc.SetPen(pen);
        
    // Draw split lines, horizontally
    for (size_t i {}; i != cRows; ++i)
        dc.DrawLine(subRects[i][0].GetTopLeft(), subRects[i][cCols - 1].GetTopRight());
    // Draw split lines, vertically
    for (size_t i {}; i != cCols; ++i)
        dc.DrawLine(subRects[0][i].GetTopRight(), subRects[cRows - 1][i].GetBottomRight());
        
    dc.SelectObject(wxNullBitmap);
    m_imagePanel->Refresh();
    m_saveBtn->Enable();
}

void MainFrame::OnSaveBtnClicked(wxCommandEvent& event)
{
    wxDirDialog dlgOutput(nullptr, _("Choose the output directory"), _T("/"));

    if (dlgOutput.ShowModal() != wxID_OK)
        return;
        
    m_image.Split(dlgOutput.GetPath());
}
