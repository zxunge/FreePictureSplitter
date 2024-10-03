/***************************************************************
 * Purpose:   Interface for fpsBatchSplitDialog
 * Author:    Grit Clef (3396563372@qq.com)
 * Created:   2024-10-03
 * Copyright: Grit Clef (https://zxunge.github.io)
 * License:   GPL v3
 **************************************************************/

#ifndef FPSBATCHSPLITDIALOG_HPP
#define FPSBATCHSPLITDIALOG_HPP
#include "UiBase.hpp"

class fpsBatchSplitDialog : public fpsBatchSplitDialogBase
{
public:
    fpsBatchSplitDialog(wxWindow* parent);
    virtual ~fpsBatchSplitDialog();
};
#endif // FPSBATCHSPLITDIALOG_HPP
