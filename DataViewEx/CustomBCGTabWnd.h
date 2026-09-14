#pragma once
class CustomBCGTabWnd : public CBCGPTabWnd
{
public:
	CustomBCGTabWnd();
	~CustomBCGTabWnd();
public:
	void SetColumn(std::vector<CString> columnNames);
};

