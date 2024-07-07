/***************************************************************
 * Purpose:   Code for actual splitting operations
 * Author:    Grit Clef (3396563372@qq.com)
 * Created:   2024-06-24
 * Copyright: Grit Clef (https://zxunge.github.io)
 * License:   GPL v3
 **************************************************************/

#include "fpsSplit.h"
#include <vector>
#include <wx/filename.h>
#include <wx/dir.h>
#include <wx/image.h>
#include <wx/string.h>
#include <wx/gdicmn.h>
#include <wx/msgdlg.h>

using std::vector;

static wxString fpsGetFileExtension(const wxString &filename)
{
	int i {0};
	for (i = filename.size() - 1; i >= 0 && filename[i] != '.'; --i)
        continue;
    return filename.Mid(i);
}

static int fpsGetSubRects(vector<vector<wxRect>> &subRects, int rows, int cols, int width, int height)
{
    int s1rWidth {width / cols},
        s1rHeight {height / rows},                       // s1r: Step 1 Rectangles'
        s2rWidth {width - (cols - 1) * s1rWidth},
        s2rHeight {height - (rows - 1) * s1rHeight};     // s2r: Step 2 Rectangles'

    if ((!s1rHeight) || (!s1rWidth))
        return -1;

    // Step 1：把(rows - 1)(cols - 1)的部分分割掉
    for (int i {0}; i < rows - 1; ++i)
        for (int j {0}; j < cols - 1; ++j)
            subRects[i][j] = wxRect(s1rWidth * j, s1rHeight * i, s1rWidth, s1rHeight);
    // Step 2：把最后一列 * 最后一行分割掉
    // 在这里，我们先割最后一行（不含右下角那一块）
    for (int j {0}; j < cols - 1; ++j)
        subRects[rows - 1][j] = wxRect(s1rWidth * j, s1rHeight * (rows - 1), s1rWidth, s2rHeight);
    // 再割最后一列（不含右下角那一块）
    for (int i {0}; i < rows - 1; ++i)
        subRects[i][cols - 1] = wxRect(s1rWidth * (cols - 1), s1rHeight * i, s2rWidth, s1rHeight);
    // 最后分割右下角那一块
    subRects[rows - 1][cols - 1] = wxRect(s1rWidth * (cols - 1), s1rHeight * (rows - 1), s2rWidth, s2rHeight);
    // OK! 完成任务！返回......
    return 0;
}

static wxArrayString fpsGetOutputList(int rows, int cols, const wxString &inputFileName, unsigned int splitCount)
{
    wxArrayString outputList;
    for (int i {1}; i != rows * cols + 1; ++i)
        outputList.Insert(inputFileName + _T("_") + wxString::Format(_T("%0*d"), splitCount, i), i - 1);
    return outputList;
}

int fpsSplit(const wxString &inputFile, const wxString &outputDir, int rows, int cols)
{
    wxInitAllImageHandlers();

    wxImage                inputImage;
    vector<vector<wxRect>> subRects(rows, vector<wxRect>(cols));
    wxArrayString          outputList;
    wxDir                  dirOut(outputDir);
    wxString               fileSuffix      {fpsGetFileExtension(inputFile)},
                           inputFileName   {wxFileName::FileName(inputFile).GetName()},
                           outputDirForPic {dirOut.GetName() + _T("\\") + inputFileName};

    auto splitCount {wxString::Format(_T("%d"), rows * cols).size()};

    if (!inputImage.LoadFile(inputFile))
        return FPS_ERR_LOADPIC;
    if (-1 == fpsGetSubRects(subRects,
                             rows, cols,
                             inputImage.GetWidth(),
                             inputImage.GetHeight()))
        return FPS_ERR_SMALLPIC;
    outputList = fpsGetOutputList(rows, cols, inputFileName, splitCount);

    // 为分割下来的图片创建一个子文件夹
    if (!wxFileName::Mkdir(outputDirForPic))
        return FPS_ERR_MKDIR;

    for (int j {0}; j< rows; ++j)
        for (int k {0}; k< cols; ++k)
            if (!inputImage.GetSubImage(subRects[j][k]).SaveFile(outputDirForPic + _T("\\") + outputList[j * cols + k] + fileSuffix))
                return FPS_ERR_SAVEPIC;
    return 0;
}
