/***************************************************************
 * Purpose:   Code for fpsImage class
 * Author:    Grit Clef (3396563372@qq.com)
 * Created:   2024-08-08
 * Copyright: Grit Clef (https://zxunge.github.io)
 * License:   GPL v3
 **************************************************************/

#include "fpsImage.hpp"

using std::vector;

wxArrayString fpsImage::GetOutputList(int rows, int cols)
{
    wxArrayString outputList;
    auto splitCount { wxString::Format(_T("%d"), rows * cols).size() };
    auto baseName { wxFileName(m_fileName).GetName() };
    
    for (int i {1}; i != rows * cols + 1; ++i)
        outputList.Insert(baseName + _T("_") + wxString::Format(_T("%0*d"), splitCount, i), i - 1);
    return outputList;
}

/* static */ vector<vector<wxRect>>
    fpsImage::AuxGetSubRects(int width, int height,
                             int rowsOrHeight, 
                             int colsOrWidth, 
                             fpsSplitMode mode)
{
    int rows, cols,                               // For rows and cols when splitting by pixel.
        s1rWidth, s1rHeight,                      // s1r: Step 1 Rectangles'
        s2rWidth, s2rHeight;                      // s2r: Step 2 Rectangles'

    //------------Split by counts-----------
    if (mode == fpsSplitMode::byCount)
    {
        s1rWidth  = width / colsOrWidth;
        s1rHeight = height / rowsOrHeight;                    
        s2rWidth  = width - (colsOrWidth - 1) * s1rWidth;
        s2rHeight = height - (rowsOrHeight - 1) * s1rHeight;     
        rows      = rowsOrHeight;
        cols      = colsOrWidth;
    }
    // ------------Split by pixel-----------
    else
    {
        s1rHeight = rowsOrHeight;
        s1rWidth  = colsOrWidth;
        rows      = height / rowsOrHeight + 1;
        cols      = width / colsOrWidth + 1;
    }

    vector<vector<wxRect>> subRects(rows, vector<wxRect>(cols));

    if ((!s1rHeight) || (!s1rWidth))
        return vector<vector<wxRect>>(0);

    // Step 1：Get (rows - 1)(cols - 1) parts splited.
    for (int i {}; i < rows - 1; ++i)
        for (int j {}; j < cols - 1; ++j)
            subRects[i][j] = wxRect(s1rWidth * j, s1rHeight * i, s1rWidth, s1rHeight);
    // Step 2：Get the last column * the last row splited.
    // In this case, we get the last row first (without the right-bottom part).
    for (int j {}; j < cols - 1; ++j)
        subRects[rows - 1][j] = wxRect(s1rWidth * j, s1rHeight * (rows - 1), s1rWidth, s2rHeight);
    // Then get the last column splited (without the right-bottom part).
    for (int i {}; i < rows - 1; ++i)
        subRects[i][cols - 1] = wxRect(s1rWidth * (cols - 1), s1rHeight * i, s2rWidth, s1rHeight);
    // Get the right-bottom part splited at last.
    subRects[rows - 1][cols - 1] = wxRect(s1rWidth * (cols - 1), s1rHeight * (rows - 1), s2rWidth, s2rHeight);
    // OK! Task is finished. Return......
    return subRects;
}

bool fpsImage::Split(wxString outputDir)
{
    if (!IsLoaded())
        return false;
    
    auto fileSuffix { wxFileName(m_fileName).GetExt() };
    auto cRows { m_rect.size() }, cCols { m_rect[0].size() };
    wxArrayString outputList { GetOutputList(cRows, cCols) };
    
    if (!wxFileName::Exists(outputDir))
        if (!wxFileName::Mkdir(outputDir))
            return false;

    for (int j {}; j < cRows; ++j)
        for (int k {}; k < cCols; ++k)
            if (!m_image.GetSubImage(m_rect[j][k]).SaveFile(outputDir + _T("\\") + outputList[j * cCols + k] + _T(".") + fileSuffix))
                return false;
    
    return true;
}