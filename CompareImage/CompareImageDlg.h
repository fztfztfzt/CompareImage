
// CompareImageDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

// CCompareImageDlg 对话框
class CCompareImageDlg : public CDialogEx
{
// 构造
public:
	CCompareImageDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMPAREIMAGE_DIALOG };
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
	// 选择进行比较的图片
	CButton m_selectImage;
	// 显示图片
//	CStatic m_showImage;
	afx_msg void OnBnClickedSelectimage();
	void  DrawToHDC(cv::Mat mat, HDC hDC, RECT rect);
	cv::MatND getHistogram(cv::Mat image);
	CStatic m_showImage;
	cv::MatND srcHist;
	void norm(cv::MatND &hist);
	CString s_path;
	afx_msg void OnBnClickedOk();
	double compareHist(cv::MatND a, cv::MatND b);
	struct MyStruct
	{
		double result;
		CString name;
		CString path;
		MyStruct(double b, CString s1, CString s2):result(b),name(s1),path(s2)
		{

		}
		
	};
	std::vector< MyStruct > v_result;
	CStatic m_showImage2;
	CListBox m_list;
	
	afx_msg void OnSelchangeList1();
};
