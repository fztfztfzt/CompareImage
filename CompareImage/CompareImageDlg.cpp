
// CompareImageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CompareImage.h"
#include "CompareImageDlg.h"
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


// CCompareImageDlg 对话框



CCompareImageDlg::CCompareImageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMPAREIMAGE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCompareImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SELECTIMAGE, m_selectImage);
	//  DDX_Control(pDX, IDC_SHOWIMAGE, m_showImage);
	DDX_Control(pDX, IDC_SHOWIMAGE, m_showImage);
	DDX_Control(pDX, IDC_SHOWIMAGE2, m_showImage2);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BEGIN_MESSAGE_MAP(CCompareImageDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SELECTIMAGE, &CCompareImageDlg::OnBnClickedSelectimage)
	ON_BN_CLICKED(IDOK, &CCompareImageDlg::OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_LIST1, &CCompareImageDlg::OnSelchangeList1)
END_MESSAGE_MAP()


// CCompareImageDlg 消息处理程序

BOOL CCompareImageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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
	AllocConsole();                     // 打开控制台资源

	freopen("CONOUT$", "w+t", stdout);// 申请写
	freopen("CONIN$", "r+t", stdin);  // 申请读
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCompareImageDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCompareImageDlg::OnPaint()
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
HCURSOR CCompareImageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void  CCompareImageDlg::DrawToHDC(cv::Mat mat, HDC hDC, RECT rect)
{
	CImage img; //ATL::CImage
	int w = mat.cols;  //宽
	int h = mat.rows;  //高
	int chinnels = mat.channels();
	img.Create(w, h, chinnels << 3);

	int i, j, k;
	BYTE *pSource = NULL;
	BYTE *pImgData = (BYTE *)img.GetBits();
	int step = img.GetPitch();

	if (chinnels == 1)
	{
		for (i = 0; i < h; ++i)
		{
			pSource = mat.ptr<BYTE>(i);
			for (j = 0; j < w; ++j)
			{
				*(pImgData + i*step + j) = pSource[j];
			}
		}
	}
	else if (chinnels == 3)
	{
		for (i = 0; i < h; ++i)
		{
			pSource = mat.ptr<BYTE>(i);
			for (j = 0; j < w; ++j)
			{
				for (k = 0; k < 3; ++k)
				{
					*(pImgData + i*step + j * 3 + k) = pSource[j * 3 + k];
				}
			}
		}
	}
	else
	{
		AfxMessageBox(TEXT("仅支持灰度图/3通道彩色图"));
		return;
	}

	SetStretchBltMode(hDC, COLORONCOLOR);
	img.StretchBlt(hDC, rect, SRCCOPY);
	img.Destroy();
}

void CCompareImageDlg::OnBnClickedSelectimage()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fileDlg(true);
	fileDlg.m_ofn.lpstrTitle = "选择图片";
	fileDlg.m_ofn.lpstrFilter = "图片(*.bmp;*.png;*.jpg)\0*.bmp;*.png;*.jpg\0All files(*.*)\0*.*\0\0";
	if (IDOK == fileDlg.DoModal())
	{
		CString s_name = fileDlg.GetFileName();
		s_path = fileDlg.GetPathName();
		cv::Mat image = cv::imread(s_path.GetString());
		srcHist = getHistogram(image);
		norm(srcHist);
		CRect rect;
		m_showImage.GetClientRect(&rect);
		CDC *dc = m_showImage.GetDC();
		HDC hDC = dc->GetSafeHdc();
		DrawToHDC(image,hDC,rect);

		/*cv::namedWindow("view");
		HWND hWnd = (HWND)cvGetWindowHandle("view");
		HWND hParent = ::GetParent(hWnd);
		::SetParent(hWnd, m_showImage.m_hWnd);
		::ShowWindow(hParent, SW_HIDE);
		cv::imshow("view", image);*/
	}
}
cv::MatND CCompareImageDlg::getHistogram(cv::Mat image)
{
	cv::MatND hist;
	float hranges[2] = { 0.0,255.0 };
	int channels[1] = { 0 };
	int histSize[1] = { 256 };
	const float *ranges[1] = { hranges };
	cv::calcHist(&image,//源图像
		1,//图像数目
		channels,//通道数
		cv::Mat(),//二值掩码
		hist,//返回的直方图
		1,//这是1D的直方图
		histSize,//项的数量
		ranges//像素值的范围
		);
	return hist;
}
void CCompareImageDlg::norm(cv::MatND &his)
{
	float mm = 0;
	for (int i = 0; i < his.rows; i++)
	{
		mm += his.at<float>(i);
	}
	for (int i = 0; i < his.rows; i++)
	{
		his.at<float>(i) /= mm;
	}
}
double CCompareImageDlg::compareHist(cv::MatND a, cv::MatND b)
{
	double sum = 0;
	for (int i = 0; i < 255; i++)
	{
		sum += sqrt(a.at<float>(i)*b.at<float>(i));
	}
	return sum;
}
bool cmp(CCompareImageDlg::MyStruct a, CCompareImageDlg::MyStruct  b)
{
	return a.result > b.result;
}
void CCompareImageDlg::OnBnClickedOk()
{
	v_result.clear();
	m_list.ResetContent();
	CFileFind find;
	int n = s_path.ReverseFind('\\');
	s_path.Delete(n, s_path.GetLength() - n);
	s_path += "\\";
	BOOL bRet = find.FindFile(s_path+"*.*");
	CString strPath;
	int sum = 0;
	while (bRet)
	{
		//2 查找下一个
		bRet = find.FindNextFile();
		//3 获取文件名称
		CString strName = find.GetFileName();
		strPath = find.GetFilePath();
		if (!find.IsDots())
		{
			sum++;
			if (find.IsDirectory())
			{
				//printf("目录：%s\n", strPath);

			}
			else
			{
				//printf("文件:%s\n", strPath);
				CString type = strPath.Right(3);
				type.MakeUpper();
				if (type == "BMP" || type == "PNG" || type == "JPG")
				{
					cv::Mat img = cv::imread(strPath.GetString());
					cv::MatND hist = getHistogram(img);
					norm(hist);
					double result = compareHist(srcHist, hist);
					v_result.push_back(MyStruct(result,strName,strPath));
				}
				
			}
		}

	}
	//4 结束查找
	find.Close();
	
	std::sort(v_result.begin(), v_result.end(),cmp);
	for (int i = 0; i < v_result.size(); i++)
	{
		CString s;
		printf("%s %lf \n", v_result[i].name, v_result[i].result);
		s.Format("%s 相似度:%lf", v_result[i].name, v_result[i].result);
		m_list.AddString(s);
	}
}


void CCompareImageDlg::OnSelchangeList1()
{
	// TODO: 在此添加控件通知处理程序代码
	int i = m_list.GetAnchorIndex();
	MyStruct temp = v_result[i];
	cv::MatND img = cv::imread(temp.path.GetString());
	CRect rect;
	m_showImage2.GetClientRect(&rect);
	CDC *dc = m_showImage2.GetDC();
	HDC hDC = dc->GetSafeHdc();
	DrawToHDC(img, hDC, rect);


}
