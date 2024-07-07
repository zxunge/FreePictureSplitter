/***************************************************************
 * Purpose:   Code for actual splitting operations
 * Author:    Grit Clef (3396563372@qq.com)
 * Created:   2024-06-24
 * Copyright: Grit Clef (https://zxunge.github.io)
 * License:   GPL v3
 **************************************************************/

#ifndef FPSSPLIT_H_INCLUDED
#define FPSSPLIT_H_INCLUDED

#include <wx/string.h>
#include <wx/dir.h>

// 分割主函数，rows为分割行数，cols为分割列数，InputDir为欲分割图片所在文件夹
int fpsSplit(const wxString &inputFile, const wxString &outputDir, int rows, int cols);

// 错误代码
#define FPS_ERR_SMALLPIC -1
#define FPS_ERR_LOADPIC  -2
#define FPS_ERR_MKDIR    -3
#define FPS_ERR_SAVEPIC  -4

#endif // FPSSPLIT_H_INCLUDED
