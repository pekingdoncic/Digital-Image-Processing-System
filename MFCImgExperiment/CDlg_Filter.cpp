#define _CRT_SECURE_NO_WARNINGS
// CDlg_Filter.cpp: 实现文件
//

#include "pch.h"
#include "MFCImgExperiment.h"
#include "afxdialogex.h"
#include "CDlg_Filter.h"
#include "MainFrm.h"
#include "PointTrans.h"
#include "MFCImgExperimentDoc.h"
#include <string>
using namespace std;
// CDlg_Filter 对话框

IMPLEMENT_DYNAMIC(CDlg_Filter, CDialogEx)

CDlg_Filter::CDlg_Filter(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Filter, pParent)
	, num1(0)
	, num2(0)
	, num3(0)
	, num4(0)
	, num5(0)
	, num6(0)
	, num7(0)
	, num8(0)
	, num9(0)
	, m_Coefficient(0)
	, m_Thre(0)
{

}

CDlg_Filter::~CDlg_Filter()
{
}

void CDlg_Filter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, num1);
	DDX_Text(pDX, IDC_EDIT2, num2);
	DDX_Text(pDX, IDC_EDIT3, num3);
	DDX_Text(pDX, IDC_EDIT4, num4);
	DDX_Text(pDX, IDC_EDIT5, num5);
	DDX_Text(pDX, IDC_EDIT6, num6);
	DDX_Text(pDX, IDC_EDIT7, num7);
	DDX_Text(pDX, IDC_EDIT8, num8);
	DDX_Text(pDX, IDC_EDIT9, num9);
	DDX_Text(pDX, IDC_Coefficient, m_Coefficient);
	DDX_Text(pDX, IDC_Thre, m_Thre);
}


BEGIN_MESSAGE_MAP(CDlg_Filter, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO1, &CDlg_Filter::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CDlg_Filter::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CDlg_Filter::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CDlg_Filter::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, &CDlg_Filter::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO6, &CDlg_Filter::OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_RADIO7, &CDlg_Filter::OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO8, &CDlg_Filter::OnBnClickedRadio8)
	ON_BN_CLICKED(IDC_RADIO9, &CDlg_Filter::OnBnClickedRadio9)
	ON_BN_CLICKED(IDC_RADIO10, &CDlg_Filter::OnBnClickedRadio10)
	ON_BN_CLICKED(IDC_RADIO11, &CDlg_Filter::OnBnClickedRadio11)
	ON_BN_CLICKED(IDC_RADIO12, &CDlg_Filter::OnBnClickedRadio12)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlg_Filter::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlg_Filter::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlg_Filter::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlg_Filter::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDlg_Filter 消息处理程序


void CDlg_Filter::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	num1 = 0;
	num2 = 0;
	num3 = 0;
	num4 = 0;
	num5 = 1;
	num6 = 0;
	num7 = 0;
	num8 = 0;
	num9 = 0;
	m_Coefficient=-1;
	m_Thre=255;
	 // 设置文本框不可用
	 (CEdit*)GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	 (CEdit*)GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
	 (CEdit*)GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
	 (CEdit*)GetDlgItem(IDC_EDIT4)->EnableWindow(FALSE);
	 (CEdit*)GetDlgItem(IDC_EDIT5)->EnableWindow(FALSE);
	 (CEdit*)GetDlgItem(IDC_EDIT6)->EnableWindow(FALSE);
	 (CEdit*)GetDlgItem(IDC_EDIT7)->EnableWindow(FALSE);
	 (CEdit*)GetDlgItem(IDC_EDIT8)->EnableWindow(FALSE);
	 (CEdit*)GetDlgItem(IDC_EDIT9)->EnableWindow(FALSE);
	 (CEdit*)GetDlgItem(IDC_Coefficient)->EnableWindow(FALSE);
	 (CEdit*)GetDlgItem(IDC_Thre)->EnableWindow(FALSE);

	 UpdateData(FALSE);
}


void CDlg_Filter::OnBnClickedRadio2()
{
	//均值模糊：
	// TODO: 在此添加控件通知处理程序代码
	num1 = 1;
	num2 = 1;
	num3 = 1;
	num4 = 1;
	num5 = 1;
	num6 = 1;
	num7 = 1;
	num8 = 1;
	num9 = 1;
	m_Coefficient = 0.1111;
	m_Thre = 0;

	// 设置文本框不可用
	(CEdit*)GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT4)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT5)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT6)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT7)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT8)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT9)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_Coefficient)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_Thre)->EnableWindow(FALSE);

	UpdateData(FALSE);
}


void CDlg_Filter::OnBnClickedRadio3()
{
	//高斯模糊
	// TODO: 在此添加控件通知处理程序代码
	//num1 = 0.04535;
	//num2 = 0.05664;
	//num3 = 0.04535;
	//num4 = 0.05664;
	//num5 = 0.07074;
	//num6 = 0.05664;
	//num7 = 0.04535;
	//num8 = 0.05664;
	//num9 = 0.04535;
	//m_Coefficient = 2.08899;
	//m_Thre = 0;
	num1 = 1;
	num2 = 2;
	num3 = 1;
	num4 = 2;
	num5 = 4;
	num6 = 2;
	num7 = 1;
	num8 = 2;
	num9 = 1;
	m_Coefficient = 0.0625;
	m_Thre = 0;

	// 设置文本框不可用
	(CEdit*)GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT4)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT5)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT6)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT7)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT8)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT9)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_Coefficient)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_Thre)->EnableWindow(FALSE);

	UpdateData(FALSE);
}


void CDlg_Filter::OnBnClickedRadio4()
{		//锐化
	// TODO: 在此添加控件通知处理程序代码
	num1 = 0;
	num2 = 0;
	num3 = 0;
	num4 = 0;
	num5 = 1;
	num6 = 0;
	num7 = 0;
	num8 = 0;
	num9 = 0;
	m_Coefficient = 1;
	m_Thre = 50;
	
	// 设置文本框不可用
	(CEdit*)GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT4)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT5)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT6)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT7)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT8)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT9)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_Coefficient)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_Thre)->EnableWindow(TRUE);

	UpdateData(FALSE);
}


void CDlg_Filter::OnBnClickedRadio5()
{
	//改变亮度：(变暗）
	// TODO: 在此添加控件通知处理程序代码
	num1 = 0;
	num2 = 0;
	num3 = 0;
	num4 = 0;
	num5 = 1;
	num6 = 0;
	num7 = 0;
	num8 = 0;
	num9 = 0;
	m_Coefficient = 1;
	m_Thre = -50;

	// 设置文本框不可用
	(CEdit*)GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT4)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT5)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT6)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT7)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT8)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT9)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_Coefficient)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_Thre)->EnableWindow(TRUE);

	UpdateData(FALSE);
}


void CDlg_Filter::OnBnClickedRadio6()
{
	// TODO: 在此添加控件通知处理程序代码
	//锐化
	num1 = -1;
	num2 = -1;
	num3 = -1;
	num4 = -1;
	num5 = 8;
	num6 = -1;
	num7 = -1;
	num8 = -1;
	num9 = -1;
	m_Coefficient = 1;
	m_Thre = 0;

	// 设置文本框不可用
	(CEdit*)GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT4)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT5)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT6)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT7)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT8)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT9)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_Coefficient)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_Thre)->EnableWindow(FALSE);

	UpdateData(FALSE);
}


void CDlg_Filter::OnBnClickedRadio7()
{
	// TODO: 在此添加控件通知处理程序代码
	//浮雕
	num1 = -1;
	num2 = -1;
	num3 = 0;
	num4 = -1;
	num5 = 0;
	num6 = 1;
	num7 = 0;
	num8 = 1;
	num9 = 1;
	m_Coefficient = 1;
	m_Thre = 128;

	// 设置文本框不可用
	(CEdit*)GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT4)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT5)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT6)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT7)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT8)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT9)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_Coefficient)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_Thre)->EnableWindow(FALSE);

	UpdateData(FALSE);
}


void CDlg_Filter::OnBnClickedRadio8()
{
	// TODO: 在此添加控件通知处理程序代码
	//自定义算子：
	(CEdit*)GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
	(CEdit*)GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
	(CEdit*)GetDlgItem(IDC_EDIT3)->EnableWindow(TRUE);
	(CEdit*)GetDlgItem(IDC_EDIT4)->EnableWindow(TRUE);
	(CEdit*)GetDlgItem(IDC_EDIT5)->EnableWindow(TRUE);
	(CEdit*)GetDlgItem(IDC_EDIT6)->EnableWindow(TRUE);
	(CEdit*)GetDlgItem(IDC_EDIT7)->EnableWindow(TRUE);
	(CEdit*)GetDlgItem(IDC_EDIT8)->EnableWindow(TRUE);
	(CEdit*)GetDlgItem(IDC_EDIT9)->EnableWindow(TRUE);
	(CEdit*)GetDlgItem(IDC_Coefficient)->EnableWindow(TRUE);
	(CEdit*)GetDlgItem(IDC_Thre)->EnableWindow(TRUE);
}


void CDlg_Filter::OnBnClickedRadio9()
{
	// TODO: 在此添加控件通知处理程序代码
	flag = 1;
	(CEdit*)GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT4)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT5)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT6)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT7)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT8)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT9)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_Coefficient)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_Thre)->EnableWindow(FALSE);
	UpdateData(FALSE);
}


void CDlg_Filter::OnBnClickedRadio10()
{
	// TODO: 在此添加控件通知处理程序代码
	flag = 2;
	(CEdit*)GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT4)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT5)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT6)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT7)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT8)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT9)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_Coefficient)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_Thre)->EnableWindow(FALSE);
	UpdateData(FALSE);
}


void CDlg_Filter::OnBnClickedRadio11()
{
	//垂直方向：
	// TODO: 在此添加控件通知处理程序代码
	if (flag == 1)
	{
		((CButton*)GetDlgItem(IDC_RADIO9))->SetCheck(TRUE);
		num1 = -1;
		num2 = -2;
		num3 = -1;
		num4 = 0;
		num5 = 0;
		num6 = 0;
		num7 = 1;
		num8 = 2;
		num9 = 1;
		m_Coefficient = 1;
		m_Thre = 0;
		UpdateData(FALSE);//更新数据
	}

	if (flag == 2)
	{
		((CButton*)GetDlgItem(IDC_RADIO10))->SetCheck(TRUE);
		num1 = -1;
		num2 = -1;
		num3 = -1;
		num4 = 0;
		num5 = 0;
		num6 = 0;
		num7 = 1;
		num8 = 1;
		num9 = 1;
		m_Coefficient = 1;
		m_Thre = 0;
		UpdateData(FALSE);//更新数据
	}
}


void CDlg_Filter::OnBnClickedRadio12()
{
	// TODO: 在此添加控件通知处理程序代码
	if (flag == 1)
	{
		((CButton*)GetDlgItem(IDC_RADIO9))->SetCheck(TRUE);
		num1 = -1;
		num2 = 0;
		num3 = 1;
		num4 = -2;
		num5 = 0;
		num6 = 2;
		num7 = -1;
		num8 = 0;
		num9 = 1;
		m_Coefficient = 1;
		m_Thre = 0;
		UpdateData(FALSE);//更新数据
	}

	if (flag == 2)
	{
		((CButton*)GetDlgItem(IDC_RADIO10))->SetCheck(TRUE);
		num1 = -1;
		num2 = 0;
		num3 = 1;
		num4 = -1;
		num5 = 0;
		num6 = 1;
		num7 = -1;
		num8 = 0;
		num9 = 1;
		m_Coefficient = 1;
		m_Thre = 0;
		UpdateData(FALSE);//更新数据
	}
}


void CDlg_Filter::OnBnClickedButton3()
{
	//预览图像：
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();   //调用主框架类，获得句柄
	CMFCImgExperimentDoc* chuzhi = (CMFCImgExperimentDoc*)pMainFrame->GetActiveDocument(); //从主框架类里面获得当前doc 
	UpdateData(TRUE);
	float temp[11];
	temp[0] = num1;
	temp[1] = num2;
	temp[2] = num3;
	temp[3] = num4;
	temp[4] = num5;
	temp[5] = num6;
	temp[6] = num7;
	temp[7] = num8;
	temp[8] = num9;
	MyTemplate(lpSrcStartBits, lSrcWidth, lSrcHeight, 3, 3, 1, 1, temp, m_Coefficient, m_Thre);
	flag1 = 1;
	chuzhi->UpdateAllViews(NULL);
}


void CDlg_Filter::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();   //调用主框架类，获得句柄
	CMFCImgExperimentDoc* pDoc = (CMFCImgExperimentDoc*)pMainFrame->GetActiveDocument(); //从主框架类里面获得当前doc	
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;


	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////

//int lBitCountNew = 24;
	int lBitCountNew = lpSrcBitCount;
	long lLineBytesNew = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lBitCountNew);
	int PaletteSize = 0;
	if (lpSrcBitCount == 8) PaletteSize = 1024;
	memcpy(lpSrcDib, pDoc->lpImgCpy, sizeof(BITMAPINFOHEADER) + PaletteSize + lSrcHeight * lLineBytesNew);


	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CDlg_Filter::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//保存算子
	UpdateData(true);//更新数据
	BOOL isOpen = FALSE;
	//F:\学习\数字图像基础\实验\MFCImgExperiment
	CString defaultDir = _T("F:\\学习\\数字图像\\实验\\MFCImgExperiment\\课设\\test");//默认打开的文件路径
	CString fileName = _T("算子");//默认打开的文件名
	TCHAR filter[] = _T("文件 (*.txt;)|*.txt;||");
	CFileDialog saveFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
	saveFileDlg.GetOFN().lpstrInitialDir = defaultDir;
	INT_PTR result = saveFileDlg.DoModal();
	CString filePath = defaultDir + "\\" + fileName;
	if (result == IDOK)
	{
		filePath = saveFileDlg.GetPathName();
		float buffer[11];
		buffer[0] = num1;
		buffer[1] = num2;
		buffer[2] = num3;
		buffer[3] = num4;
		buffer[4] = num5;
		buffer[5] = num6;
		buffer[6] = num7;
		buffer[7] = num8;
		buffer[8] = num9;
		buffer[9] = m_Coefficient;
		buffer[10] = m_Thre;
		string a;
		string sbuffer;
		char* t = new char[1];
		for (int i = 0; i < 11; i++)
		{
			sprintf_s(t, sizeof(t), "%.0f", buffer[i]);
			sbuffer += t;
			sbuffer += " ";
		}
		char* buff = (char*)sbuffer.data();
		CFile txt(filePath, CFile::modeWrite | CFile::modeCreate);
		txt.Write(buff, strlen(buff));
		txt.Close();
	}
}


void CDlg_Filter::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//装载算子
	UpdateData(true);//更新数据
	BOOL isOpen = TRUE;//否为保存
	//F:\学习\数字图像基础\实验\MFCImgExperiment
	CString defaultDir = _T("F:\\学习\\数字图像基础\\实验\\MFCImgExperiment\\test");//默认打开的文件路径
	CString fileName = _T("算子");//默认打开的文件名
	CString filter = _T("文件 (*.txt;)|*.txt;||");
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);
	openFileDlg.GetOFN().lpstrInitialDir = defaultDir;
	INT_PTR result = openFileDlg.DoModal();
	CString filePath = defaultDir;
	if (result == IDOK)
	{
		filePath = openFileDlg.GetPathName();

		CFile txt(filePath, CFile::modeRead);
		CFileStatus status;
		CFile::GetStatus(filePath, status);
		int size = status.m_size;

		char* buffer;
		buffer = new char[size];
		txt.Read(buffer, size);
		char* temp = new char[1000];
		//strcpy_s(temp, 2, " ");
		strcpy(temp, " ");
		int k = 0;
		float suanzi[11];
		for (int i = 0; i < size; i++)
		{
			if (buffer[i] != ' ')
			{
				//strcat_s(temp, sizeof(temp), &buffer[i]);
				strcat(temp, &buffer[i]);
			}
			else if (k < 12)
			{
				suanzi[k] = std::atof(temp);
				k += 1;
				//strcpy_s(temp, 1, "");
				strcpy(temp, "");
			}
		}
		num1 = suanzi[0];
		num2 = suanzi[1];
		num3 = suanzi[2];
		num4 = suanzi[3];
		num5 = suanzi[4];
		num6 = suanzi[5];
		num7 = suanzi[6];
		num8 = suanzi[7];
		num9 = suanzi[8];
		m_Coefficient = suanzi[9];
		m_Thre = suanzi[10];
		txt.Close();
		UpdateData(false);
		UpdateData(true);
	}
}
