#ifndef MAINFRAME_HPP
#define MAINFRAME_HPP
#include "UiBase.hpp"
#include "fpsImage.hpp"
#include "imagepanel.h"

class MainFrame : public MainFrameBase
{
public:
    MainFrame(wxWindow* parent);
    virtual ~MainFrame();
protected:
    virtual void OnSaveBtnClicked(wxCommandEvent& event);
    virtual void OnSplitBtnClicked(wxCommandEvent& event);
    virtual void OnOpenFileToolClicked(wxCommandEvent& event);
    virtual void OnBatchSplitToolClicked(wxCommandEvent& event);
    virtual void OnOpenItemMenu(wxCommandEvent& event);
    virtual void OnPixelRadioButtonSelected(wxCommandEvent& event);
    virtual void OnNumRadioButtonSelected(wxCommandEvent& event);
    virtual void OnAbout(wxCommandEvent& event);
    virtual void OnExit(wxCommandEvent& event);
    
private:
    fpsImage m_image;
    wxImagePanel *m_imagePanel;
};
#endif // MAINFRAME_HPP
