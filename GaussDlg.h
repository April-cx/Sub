
// GaussDlg.h: 头文件
//

#pragma once
#include<string>
#include<map>
#include<cstdio>
#include<vector>
#include<cstdlib>
using namespace std;
// CGaussDlg 对话框
class CGaussDlg : public CDialogEx
{
// 构造
public:
	CGaussDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAUSS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_edit2;
	CString m_edit1;
	CString m_string;
	CArray<char*> equationSet;
	afx_msg void OnBnClickedButton1();
	int equation_count;//表示方程组的个数
	int parameter_count;//表示参数个数
	void GaussMethod(int row_count, int column_count, double* a);
	map<char, vector<string>> m_vector;//表示未知数矢量
};
