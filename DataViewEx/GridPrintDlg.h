#pragma once
#include "stdafx.h"


// GridPrintDlg 대화 상자

class GridPrintDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(GridPrintDlg)

public:
	GridPrintDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~GridPrintDlg();
	std::vector<CString> m_headers;
	std::vector<ColumnInfo> m_columnInfos;
	std::vector<std::vector<CString>> m_rows;
	int m_nCurrentPage;
	int m_rowsPerPage;
	int m_gridType;
	bool m_isMultiLine;  // 다중 행 여부
	//CBCGPGridCtrl* inputGrid;
	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum {
		IDD = IDD_GRID_PRINT_DLG
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	afx_msg void OnPaint();
	void OnBnClickedBtnPrint();

	afx_msg void OnBnClickedBtnPrevPage();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnNextPage();

	DECLARE_MESSAGE_MAP()
public:
	//CBCGPButton okBtn;
	CBCGPButton prevBtn;
	CBCGPButton nextgBtn;
	CBCGPButton printBtn;
	CBCGPButton cancelBtn;
};
