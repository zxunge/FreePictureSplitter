/************************************
 *  Copyright (c) 2023, Grit Clef
 */

#include "FreePictureSplitterMain.h"
#include <wx/dirdlg.h>
#include <wx/filename.h>
#include <wx/dir.h>
#include <wx/image.h>
#include <wx/string.h>
#include <wx/gdicmn.h>
#include <wx/msgdlg.h>
#include <cstdlib>

const wxString fileSpec = _("*.jpg");

using namespace std;

template <typename T>
T** malloc_Array2D(int row, int col)
{
	int size = sizeof(T);
	int point_size = sizeof(T*);
	//鍏堢敵璇峰唴瀛?#xff0c;鍏朵腑point_size * row琛ㄧず瀛樻斁row涓鎸囬拡
	T **arr = (T **) malloc(point_size * row + size * row * col);
	if (arr != NULL)
	{
		memset(arr, 0, point_size * row + size * row * col);
		T *head = (T*)((int)arr + point_size * row);
		while (row--)
			arr[row] = (T*)((int)head + row * col * size);
	}
	return (T**)arr;
}
//閲婃斁浜岀淮鏁扮粍
void free_Array2D(void **arr)
{
	if (arr != NULL)
		free(arr);
}

int fpsGetFileCount(const wxDir dir)
{
    // 鏈嚱鏁板彧鑰冭檻褰撳墠鐩綍涓嬫枃浠讹紝涓嶉€掑綊瀛愭枃浠跺す
    int fileCount = 0;
    wxString filePath;
    bool cont = dir.GetFirst(&filePath, fileSpec, wxDIR_FILES);
    while (cont)
    {
        ++fileCount;
        cont = dir.GetNext(&filePath);
    }

    return fileCount;
}

wxRect **fpsGetSubRects(int rows, int cols, int width, int height)
{
    wxRect **subRects = malloc_Array2D<wxRect>(rows, cols);
    int s1rWidth = width / cols,
        s1rHeight = height / rows,                       // s1r: Step 1 Rectangles'
        s2rWidth = width - (cols - 1) * s1rWidth,
        s2rHeight = height - (rows - 1) * s1rHeight;     // s2r: Step 2 Rectangles'

    if ((!s1rHeight) || (!s1rWidth))
    {
        wxMessageBox(_("鎮ㄧ殑鍥剧墖澶皬锛屾垨鍒嗗壊娆℃暟澶锛岃閲嶈瘯锛?), _("FreePictureSplitter閿欒"), wxICON_ERROR);
        free_Array2D((void **)subRects);
        return nullptr;
    }

    // Step 1锛氭妸(rows - 1)(cols - 1)鐨勯儴鍒嗗垎鍓叉帀
    for (int i = 0; i < rows - 1; ++i)
        for (int j = 0; j < cols - 1; ++j)
            subRects[i][j] = wxRect(s1rWidth * j, s1rHeight * i, s1rWidth, s1rHeight);
    // Step 2锛氭妸鏈€鍚庝竴鍒?* 鏈€鍚庝竴琛屽垎鍓叉帀
    // 鍦ㄨ繖閲岋紝鎴戜滑鍏堝壊鏈€鍚庝竴琛岋紙涓嶅惈鍙充笅瑙掗偅涓€鍧楋級
    for (int j = 0; j < cols - 1; ++j)
        subRects[rows - 1][j] = wxRect(s1rWidth * j, s1rHeight * (rows - 1), s1rWidth, s2rHeight);
    // 鍐嶅壊鏈€鍚庝竴鍒楋紙涓嶅惈鍙充笅瑙掗偅涓€鍧楋級
    for (int i = 0; i < rows - 1; ++i)
        subRects[i][cols - 1] = wxRect(s1rWidth * (cols - 1), s1rHeight * i, s2rWidth, s1rHeight);
    // 鏈€鍚庡垎鍓插彸涓嬭閭ｄ竴鍧?    subRects[rows - 1][cols - 1] = wxRect(s1rWidth * (cols - 1), s1rHeight * (rows - 1), s2rWidth, s2rHeight);
    // OK! 瀹屾垚浠诲姟锛佽繑鍥?.....
    return subRects;
}

wxArrayString *fpsGetOutputList(int rows, int cols, const wxString inputFileName, int splitCount)
{
    wxArrayString *outputList = new wxArrayString;
    // 瀵逛簬JPG鏂囦欢
    for (int i = 1; i <= rows * cols; ++i)
        outputList -> Insert(inputFileName + _("\\") + inputFileName + _("_") + wxString::Format(_("%0*d"), splitCount, i) + _(".jpg"), i - 1);
    return outputList;
}

int fpsSplit(FreePictureSplitterFrame*fpsf, int rows, int cols, const wxString dirName)
{
    wxInitAllImageHandlers();

    wxDir dirIn(dirName),
          dirOut;
    wxString inFilePath, inFileName;
    wxImage inputImage;
    wxRect **subRects = nullptr;
    wxArrayString *outputList = nullptr;

    wxDirDialog chooseOutputDir(nullptr, _("閫夋嫨杈撳嚭鍥剧墖鐨勬枃浠跺す"), _("/"));

    if (chooseOutputDir.ShowModal() != wxID_OK)
        return -1;
    dirOut.Open(chooseOutputDir.GetPath());
    if (!dirOut.IsOpened())
        return -1;
    int fileCount = fpsGetFileCount(dirName);
    int splitCount = wxString::Format(_("%d"), rows * cols).size();
    bool cont = dirIn.GetFirst(&inFileName, fileSpec, wxDIR_FILES);
    int sgRange = fpsf -> SplitGauge -> GetRange();

    // 鎴戜滑寮€濮嬪垎鍓插暒锛?    for (int i = 1; i<= fileCount && cont; ++i)
    {
        inFilePath = dirName + _("\\") + inFileName;     // 鑾峰彇鏂囦欢鍚嶅拰璺緞

        if (!inputImage.LoadFile(inFilePath))
            return -1;
        subRects = fpsGetSubRects(rows, cols,
                                  inputImage.GetWidth(),
                                  inputImage.GetHeight());
        if (!subRects)
            return -1;
        outputList = fpsGetOutputList(rows, cols, inFileName, splitCount);

        // 缁欐瘡涓垎鍓蹭笅鏉ョ殑鍥剧墖涓€涓瓙鏂囦欢澶?        if (!wxFileName::Mkdir(dirOut.GetName() + _("\\") + inFileName))
        {
            free_Array2D((void **)subRects);
            delete outputList;
            return -1;
        }

        for (int j = 0; j< rows; ++j)
            for (int k = 0; k< cols; ++k)
                if (!inputImage.GetSubImage(subRects[j][k]).SaveFile(dirOut.GetName() + _("\\") + (*outputList)[j * cols + k]))
                    return -1;

        fpsf -> SplitGauge -> SetValue(int(double(i) / fileCount * sgRange));    // 鏇存柊杩涘害
        cont = dirIn.GetNext(&inFileName);

        delete outputList;
        free_Array2D((void **)subRects);
    }
    return 0;
}
