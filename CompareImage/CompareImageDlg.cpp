
// CompareImageDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CompareImage.h"
#include "CompareImageDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CCompareImageDlg �Ի���



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


// CCompareImageDlg ��Ϣ�������

BOOL CCompareImageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	AllocConsole();                     // �򿪿���̨��Դ

	freopen("CONOUT$", "w+t", stdout);// ����д
	freopen("CONIN$", "r+t", stdin);  // �����
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCompareImageDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCompareImageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void  CCompareImageDlg::DrawToHDC(cv::Mat mat, HDC hDC, RECT rect)
{
	CImage img; //ATL::CImage
	int w = mat.cols;  //��
	int h = mat.rows;  //��
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
		AfxMessageBox(TEXT("��֧�ֻҶ�ͼ/3ͨ����ɫͼ"));
		return;
	}

	SetStretchBltMode(hDC, COLORONCOLOR);
	img.StretchBlt(hDC, rect, SRCCOPY);
	img.Destroy();
}

void CCompareImageDlg::OnBnClickedSelectimage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog fileDlg(true);
	fileDlg.m_ofn.lpstrTitle = "ѡ��ͼƬ";
	fileDlg.m_ofn.lpstrFilter = "ͼƬ(*.bmp;*.png;*.jpg)\0*.bmp;*.png;*.jpg\0All files(*.*)\0*.*\0\0";
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
	cv::calcHist(&image,//Դͼ��
		1,//ͼ����Ŀ
		channels,//ͨ����
		cv::Mat(),//��ֵ����
		hist,//���ص�ֱ��ͼ
		1,//����1D��ֱ��ͼ
		histSize,//�������
		ranges//����ֵ�ķ�Χ
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
		//2 ������һ��
		bRet = find.FindNextFile();
		//3 ��ȡ�ļ�����
		CString strName = find.GetFileName();
		strPath = find.GetFilePath();
		if (!find.IsDots())
		{
			sum++;
			if (find.IsDirectory())
			{
				//printf("Ŀ¼��%s\n", strPath);

			}
			else
			{
				//printf("�ļ�:%s\n", strPath);
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
	//4 ��������
	find.Close();
	
	std::sort(v_result.begin(), v_result.end(),cmp);
	for (int i = 0; i < v_result.size(); i++)
	{
		CString s;
		printf("%s %lf \n", v_result[i].name, v_result[i].result);
		s.Format("%s ���ƶ�:%lf", v_result[i].name, v_result[i].result);
		m_list.AddString(s);
	}
}


void CCompareImageDlg::OnSelchangeList1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int i = m_list.GetAnchorIndex();
	MyStruct temp = v_result[i];
	cv::MatND img = cv::imread(temp.path.GetString());
	CRect rect;
	m_showImage2.GetClientRect(&rect);
	CDC *dc = m_showImage2.GetDC();
	HDC hDC = dc->GetSafeHdc();
	DrawToHDC(img, hDC, rect);


}
