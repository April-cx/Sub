
// GaussDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Gauss.h"
#include "GaussDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGaussDlg 对话框



CGaussDlg::CGaussDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GAUSS_DIALOG, pParent)
	, m_edit2(_T(""))
	, m_edit1(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGaussDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_edit2);
	DDX_Text(pDX, IDC_EDIT1, m_edit1);
}

BEGIN_MESSAGE_MAP(CGaussDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CGaussDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CGaussDlg 消息处理程序

BOOL CGaussDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGaussDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGaussDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGaussDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGaussDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	equationSet.RemoveAll();
	m_vector.clear();
	m_string = m_edit2;
	char* pstr;
	char delims[] = "\n\r";      
	char* strToken = nullptr;              
	char* next_token = nullptr; 
	pstr = (LPSTR)(LPCTSTR)m_string;
	strToken = strtok_s(pstr, delims, &next_token);//以下的几个操作都是将方程组逐个分成方程
	while (strToken != NULL)
	{
		if (strToken != NULL)
		{
			equationSet.Add(strToken);
			strToken = strtok_s(nullptr, delims, &next_token);
		}
	}
	equation_count = equationSet.GetSize();
	
	int* b = new int[equation_count];
	for (int i = 0; i < equation_count; i++)
	{
		string equation = equationSet.GetAt(i);
		int EqualPos = equation.find('=');
		if (EqualPos > equation.size())
		{
			AfxMessageBox("方程的格式不对");
			return;
		}
		string temp="";
		for (int j = 0; j < equation.size(); j++)
		{
			char sample = equation.at(j);//方程组元素逐个遍历
			if ((sample >= '0' && sample <= '9') || sample == '-')
				temp += sample;
			else if ((sample >= 'a' && sample <= 'z') || (sample >= 'A' && sample <= 'Z'))//此时遇到未知数
			{
				map<char, vector<string>>::iterator iter;
				iter = m_vector.find(sample);
				if (temp == ""||temp=="-")
					temp += '1';
				if (iter == m_vector.end())//表示有新的未知数加入
				{
					vector<string> m_arr;
					for (int k = 0; k < equation_count; k++)
						m_arr.push_back("0");
					m_arr.at(i) = temp;
					m_vector.insert(make_pair(sample, m_arr));
				}
				else
				{
					iter->second.at(i)=temp;
					
				}
				temp="";
			}
			else if (sample != '='&&sample!='+')
			{
				AfxMessageBox("方程出现别的符号");
					return;
			}
		}
		double pos = equation.find("=");
		string Temp = equation.substr(pos + 1);
		if (Temp.at(0) == '-')
		{
			Temp = Temp.substr(1);
			b[i] = atof(Temp.c_str()) * -1;
		}
		else
			b[i] = atof(Temp.c_str());
	}
	parameter_count = m_vector.size();
	double** array = new double* [equation_count];
	for (int i = 0; i < equation_count; i++)
		array[i] = new double[parameter_count + 1];
	/*for(int i=0;i<parameter_count;i++)
		for (int j = 0; j < equation_count; j++)
		{
			string temp = m_vector.at(i).at(j);
			if (temp.at(0) == '-')
			{
				temp.erase(0);
				array[i][j] = atof(temp.c_str()) * -1;
			}
			else
				array[i][j] = atof(temp.c_str());
		}*/
	int k = 0;
	map<char, vector<string>>::iterator iter = m_vector.begin();
	for (iter; iter != m_vector.end(); iter++)
	{
		for (int j = 0; j < equation_count; j++)
		{
			string temp = iter->second.at(j);
			if (temp.at(0) == '-')
			{
				temp = temp.substr(1);
				array[j][k] = atof(temp.c_str()) * -1;
			}
			else
			{

				array[j][k] = atof(temp.c_str());
			}
		}
		k++;
	}
	for (int i = 0; i < equation_count; i++)
		array[i][parameter_count] = b[i];
	double* a = new double[(parameter_count + 1) * equation_count];
	for (int i = 0; i < equation_count; i++)
		for (int j = 0; j < parameter_count + 1; j++)
			a[i * (parameter_count + 1) + j] = array[i][j];
	GaussMethod(equation_count, parameter_count + 1, a);
}


void CGaussDlg::GaussMethod(int row_count, int column_count, double* a)
{
	// TODO: 在此处添加实现代码.
	int InEqualZeroLine_count = row_count;//该参数表示非零行的行数
	double** array = new double* [row_count];
	//完成一维数组向二维数组的转换
	for (int i = 0; i < row_count; i++)
		array[i] = new double[column_count];
	for (int i = 0; i < row_count; i++)
		for (int j = 0; j < column_count; j++)
			array[i][j] = a[i * column_count + j];
	int now, n;
	if (row_count > column_count - 1)
		n = column_count - 1;
	else
		n = row_count - 1;
	for (int i = 0, k = 0; i < n && k < column_count - 1; i++, k++)
	{
		now = i;
		for (int j = i + 1; j < row_count; j++)
			if (fabs(array[now][k]) < fabs(array[j][k]))
				now = j;
		for (int j = i; j < column_count; j++)
			swap(array[now][j], array[i][j]);
		if (array[i][k] == 0)
			k++;
		for (int j = k+1; j < column_count; j++)
			array[i][j] /= array[i][k];
		array[i][k] = 1;
		for (int j = i + 1; j < row_count; j++)
		{
			for (int l = k + 1; l < column_count; l++)
				array[j][l] -= array[j][k] * array[i][l];
			array[j][k] = 0;
		}
	}
	//判断方程组解的情况
	for (int i = 0; i < row_count; i++)
	{
		int j;
		for (j = 0; j < column_count - 1; j++)
		{
			if (array[i][j] != 0)
				break;
		}
		if (j == column_count - 1)
			if (array[i][j] != 0)
			{
				AfxMessageBox("方程组无解");
				CString show = "无解";
				SetDlgItemText(IDC_EDIT1, show);
				return;
			}
			else
				InEqualZeroLine_count--;
	}
		if (InEqualZeroLine_count > column_count - 1)
		{
			int i;
			for (i = 0; i < row_count; i++)
				if (array[i][column_count - 1] != 0)
					break;
			if (i == row_count)
			{
				AfxMessageBox("方程组只有零解");
				CString show = "方程组只有零解";
				SetDlgItemText(IDC_EDIT1, show);
				return;
			}
			else
			{
				AfxMessageBox("方程组无解");
				CString show = "无解";
				SetDlgItemText(IDC_EDIT1, show);
				return;
			}
		}
			if (InEqualZeroLine_count < column_count - 1)
			{
				AfxMessageBox("方程有无穷多解");
				CString show = "无穷解";
				SetDlgItemText(IDC_EDIT1, show);
				return;
			}
			if (InEqualZeroLine_count == column_count - 1)
			{
				
				map<char, vector<string>>::iterator iter = m_vector.begin();
				for (int i = InEqualZeroLine_count - 1; i > 0; i--)
					for (int j = i - 1; j >= 0; j--)
						array[j][column_count - 1] -= array[j][i] * array[i][column_count - 1];
				int k = 0;
				CString show = NULL, parameter, data;
				for (iter; iter != m_vector.end(); iter++)
				{
					parameter.Format("%c", iter->first);
					data.Format("%f", array[k][column_count - 1]);
					show += parameter + '=' + data + "\r\n";
					k++;
				}
				SetDlgItemText(IDC_EDIT1, show);
				return;
			}
}
