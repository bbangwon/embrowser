
// embrowserDlg.cpp : ���� ����
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

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CembrowserDlg ��ȭ ����



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


// CembrowserDlg �޽��� ó����

BOOL CembrowserDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	Start();

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CembrowserDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
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
	m_settingDlg.ReadINI();	//INI���� �б�
	SetScreenSize();
	ShowIndexHTML();
	QuitProcess();	
}



void CembrowserDlg::QuitProcess()
{
	switch (m_settingDlg.m_autoquitSel)
	{
	case 0:	//���α׷� ����
	case 1:	//PC ����
		SetTimer(2, 1000, nullptr);
	case 2:	//�ƹ��͵� ���� ����.
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

	switch (m_settingDlg.m_fullscreenSel)	//��üȭ��
	{
	case 0:	//��üȭ��
		width = m_backgroundScreenRect.Width() + 3;
		height = m_backgroundScreenRect.Height() + 3;
		break;
	case 1:	//�»��
		break;
	case 2:	//���ϴ�
		yPos = m_backgroundScreenRect.Height() - height;
		break;
	case 3:	//����
		xPos = m_backgroundScreenRect.Width() - width;
		break;
	case 4:	//���ϴ�
		yPos = m_backgroundScreenRect.Height() - height;
		break;
	}	

	//������ ����	
	m_control.SetWindowPos(nullptr, addSizeSX, addSizeSY, width + addSizeEX, height + addSizeEY, 0);
	SetWindowPos(&CWnd::wndTopMost, xPos, yPos, width, height, 0);
	
}

void CembrowserDlg::SetScreenSaver()
{
	if (m_settingDlg.m_saverSel < 2)	//0 ��ũ�����̹�, 1 �ʱ�ȭ��, 2 ������
	{
		saverTime = m_settingDlg.m_saverTime;
		if (m_settingDlg.m_saverSecOrMin == 0)	//���ϰ��
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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
		case 2:	//���α׷� ���� ����
			chrono::system_clock::time_point now = chrono::system_clock::now();
			time_t now_c = chrono::system_clock::to_time_t(now);
			struct tm parts;
			localtime_s(&parts, &now_c);
			if (parts.tm_hour == m_settingDlg.m_autoquitHour && parts.tm_min == m_settingDlg.m_autoquitMinute)
			{
				KillTimer(2);
				if (m_settingDlg.m_autoquitSel == 0)	//���α׷� ����
				{
					PostQuitMessage(0);
				}
				else if (m_settingDlg.m_autoquitSel == 1)	//PC ����
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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4) return TRUE;
	else if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
	else if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F9)
	{
		if(IsCTRLPressed() && IsSHIFTPressed())
			PostQuitMessage(0);
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
