#ifndef FREEPICTURESPLITTER_H_INCLUDED
#define FREEPICTURESPLITTER_H_INCLUDED

/************************************
 *  Copyright (c) 2023, Grit Clef
 */


#endif // FREEPICTURESPLITTER_H_INCLUDED

#include "wx/string.h"

// 分割主函数，rows为分割行数，cols为分割列数，InputDir为欲分割图片所在文件夹
// 返回0为正常，-1为异常
int fpsSplit(FreePictureSplitterFrame*fpsf, int rows, int cols, const wxString InputDir);
