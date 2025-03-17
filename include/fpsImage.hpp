/***************************************************************
 * Purpose:   Interface for fpsImage class
 * Author:    Grit Clef (3396563372@qq.com)
 * Created:   2024-08-08
 * Copyright: Grit Clef (https://zxunge.github.io)
 * License:   GPL v3
 **************************************************************/

#ifndef FPSIMAGE_HPP
#define FPSIMAGE_HPP

#include <vector>
#include <wx/image.h>
#include <wx/gdicmn.h>
#include <wx/string.h>
#include <wx/filename.h>
#include <wx/arrstr.h>

enum class fpsSplitMode { byPixel, byCount };

class fpsImage
{
private:
    wxImage m_image;
    std::vector<std::vector<wxRect>> m_rect;
    bool m_fileLoaded { false };
    wxString m_fileName;
    
private:
    wxArrayString GetOutputList(int rows, int cols);
    
public:
    fpsImage() {}
    fpsImage(wxString path) 
    {
        m_fileLoaded = m_image.LoadFile(path);
        m_fileName = m_fileLoaded ? path : wxString(wxEmptyString);
    }
    ~fpsImage() {}
    static std::vector<std::vector<wxRect>>
        AuxGetSubRects(int width, int height,
                       int rowsOrHeight, 
                       int colsOrWidth, 
                       fpsSplitMode mode = fpsSplitMode::byCount);
    void SetSubRects(const std::vector<std::vector<wxRect>> &rect)
    {
        m_rect = rect;
    }
    bool IsLoaded() { return m_fileLoaded; }
    bool LoadFile(const wxString &path) 
    {
        if (!m_image.LoadFile(path))
            return false;
        else
        {
            m_fileName = path;
            m_fileLoaded = true;
            return true;
        }
    }
    wxImage GetImage() { return m_fileLoaded ? m_image
                                             : wxNullImage; }
    bool Split(wxString outputDir);
};

#endif // FPSIMAGE_HPP
