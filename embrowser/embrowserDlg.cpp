
// embrowserDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "embrowser.h"
#include "embrowserDlg.h"
#include "afxdialogex.h"
#include "EMUtil.h"

#include <chrono>
#include <ctime>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BGM_MP3 "bgm.mp3"
#define INDEX_HTML "index.html"
#define SAVER_HTML "intro.html"

#define IsSHIFTPressed() ( 0x8000 ==(GetKeyState(VK_SHIFT) & 0x8000   ))
#define IsCTRLPressed()  ( 0x8000 ==(GetKeyState(VK_CONTROL) & 0x8000 ))

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CembrowserDlg 대화 상자



CembrowserDlg::CembrowserDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EMBROWSER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_control.parentPtr = this;
}

void CembrowserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER_CONTROL, m_control);
}

BEGIN_MESSAGE_MAP(CembrowserDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CembrowserDlg 메시지 처리기

BOOL CembrowserDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	Start();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CembrowserDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CembrowserDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CembrowserDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CembrowserDlg::BGMPlay()
{
	CString mp3FileName = CEMUtil::GetExcutePath() + "\\" + BGM_MP3;

	BGMStop();
	m_mp3.Open(mp3FileName);

	m_mp3.Play();
	SetTimer(0, 1000, NULL);
}

void CembrowserDlg::BGMStop()
{
	KillTimer(0);
	m_mp3.Stop();
}

void CembrowserDlg::Start()
{
	SystemParametersInfo(SPI_GETWORKAREA, 0, &m_backgroundScreenRect, 0);
	m_settingDlg.ReadINI();	//INI파일 읽기
	SetScreenSize();
	ShowIndexHTML();
	QuitProcess();	
}



void CembrowserDlg::QuitProcess()
{
	switch (m_settingDlg.m_autoquitSel)
	{
	case 0:	//프로그램 종료
	case 1:	//PC 종료
		SetTimer(2, 1000, nullptr);
	case 2:	//아무것도 하지 않음.
		break;
	}
}

void CembrowserDlg::ShowIndexHTML()
{	
	CString indexHTMLPath = CEMUtil::GetExcutePath() + "\\" + INDEX_HTML;
	m_control.Navigate(indexHTMLPath, nullptr, nullptr, nullptr, nullptr);
	currentState = SCREENSTATE::NORMAL;
	SetScreenSaver();
	BGMPlay();
}

void CembrowserDlg::ShowSaverHTML()
{
	CString indexHTMLPath = CEMUtil::GetExcutePath() + "\\" + SAVER_HTML;
	m_control.Navigate(indexHTMLPath, nullptr, nullptr, nullptr, nullptr);
	currentState = SCREENSTATE::SCREENSAVER;	
	BGMStop();
}


void CembrowserDlg::SetScreenSize()
{

	int xPos = -1;
	int yPos = -1;
	int width = _ttoi(m_settingDlg.m_fullscreenWidth) + 3;
	int height = _ttoi(m_settingDlg.m_fullscreenHeight) + 3;

	int addSizeSX, addSizeSY, addSizeEX, addSizeEY;
	
	addSizeSX = _ttoi(m_settingDlg.m_addpixelSX);
	addSizeSY = _ttoi(m_settingDlg.m_addpixelSY);
	addSizeEX = _ttoi(m_settingDlg.m_addpixelEX) + abs(addSizeSX);
	addSizeEY = _ttoi(m_settingDlg.m_addpixelEY) + abs(addSizeSY);	

	switch (m_settingDlg.m_fullscreenSel)	//전체화면
	{
	case 0:	//전체화면
		width = m_backgroundScreenRect.Width() + 3;
		height = m_backgroundScreenRect.Height() + 3;
		break;
	case 1:	//좌상단
		break;
	case 2:	//좌하단
		yPos = m_backgroundScreenRect.Height() - height;
		break;
	case 3:	//우상단
		xPos = m_backgroundScreenRect.Width() - width;
		break;
	case 4:	//우하단
		yPos = m_backgroundScreenRect.Height() - height;
		break;
	}	

	//사이즈 조절	
	m_control.SetWindowPos(nullptr, addSizeSX, addSizeSY, width + addSizeEX, height + addSizeEY, 0);
	SetWindowPos(&CWnd::wndTopMost, xPos, yPos, width, height, 0);
	
}

void CembrowserDlg::SetScreenSaver()
{
	if (m_settingDlg.m_saverSel < 2)	//0 스크린세이버, 1 초기화면, 2 사용안함
	{
		saverTime = m_settingDlg.m_saverTime;
		if (m_settingDlg.m_saverSecOrMin == 0)	//분일경우
			saverTime *= 60;
		SetTimer(1, 1000, nullptr);
	}
}

void CembrowserDlg::ProcScreenSaver()
{
	switch (m_settingDlg.m_saverSel)
	{
	case 0:
		ShowSaverHTML();
		break;
	case 1:
		ShowIndexHTML();
		break;
	}
}

void CembrowserDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	switch(nIDEvent)
	{
		case 0:
			{
				int mp3PlayTime = m_mp3.GetPlayTime();
				int mp3CurrentPos = m_mp3.GetCurrentPos();

				if (mp3CurrentPos > 0)
				{
					int nPer = mp3CurrentPos * 100 / mp3PlayTime;
					if (nPer == 100)
					{
						KillTimer(0);
						BGMPlay();
					}
				}
			}
			break;
		case 1:
			if (--saverTime == 0)
			{
				KillTimer(1);
				ProcScreenSaver();
			}
			break;
		case 2:	//프로그램 종료 관련
			chrono::system_clock::time_point now = chrono::system_clock::now();
			time_t now_c = chrono::system_clock::to_time_t(now);
			struct tm parts;
			localtime_s(&parts, &now_c);
			if (parts.tm_hour == m_settingDlg.m_autoquitHour && parts.tm_min == m_settingDlg.m_autoquitMinute)
			{
				KillTimer(2);
				if (m_settingDlg.m_autoquitSel == 0)	//프로그램 종료
				{
					PostQuitMessage(0);
				}
				else if (m_settingDlg.m_autoquitSel == 1)	//PC 종료
				{
					CEMUtil::PowerOff();
				}
			}
			break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CembrowserDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4) return TRUE;
	else if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
	else if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F9)
	{
		if(IsCTRLPressed() && IsSHIFTPressed())
			PostQuitMessage(0);
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
