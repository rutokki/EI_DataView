#include "stdafx.h"
#include "CustomBCGTabWnd.h"

CustomBCGTabWnd::CustomBCGTabWnd()
{
	m_bVisualManagerStyle = TRUE;
	EnableToolTips(FALSE);
	HideDisabledScrollButtons(TRUE);
	EnableTabSwap(FALSE);

}

CustomBCGTabWnd::~CustomBCGTabWnd()
{
}

void CustomBCGTabWnd::SetColumn(std::vector<CString> columnNames)
{

}
