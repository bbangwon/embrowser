// EMBrowserSettingDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "embrowser.h"
#include "EMBrowserSettingDlg.h"
#include "afxdialogex.h"
#include "EMUtil.h"

#define INI_FILE_NAME _T("embrowser.ini")
#define SECTION_FULLSCREEN _T("FullScreen")
#define SECTION_SCREENSAVER _T("ScreenSaver")
#define SECTION_AUTOOFF _T("AutoOff")
#define SECTION_EXTENDSCREEN _T("ExtendScreen")

#define KEY_COMMON_SEL _T("Sel")
#define KEY_FULLSCREEN_WIDTH _T("Width")
#define KEY_FULLSCREEN_HEIGHT _T("Height")

#define KEY_SCREENSAVER_INTERVAL _T("Interval")
#define KEY_SCREENSAVER_UNIT _T("Unit")

#define KEY_AUTOOFF_HOUR _T("Hour")
#define KEY_AUTOOFF_MINUTE _T("Minute")

#define KEY_EXTENDSCREEN_SX _T("sx")
#define KEY_EXTENDSCREEN_SY _T("sy")
#define KEY_EXTENDSCREEN_EX _T("ex")
#define KEY_EXTENDSCREEN_EY _T("ey")



// CEMBrowserSettingDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEMBrowserSettingDlg, CDialogEx)

CEMBrowserSettingDlg::CEMBrowserSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SETTING, pParent)
	, m_fullscreenWidth(_T(""))
	, m_fullscreenHeight(_T(""))
	, m_saverTime(0)
	, m_addpixelSX(_T(""))
	, m_addpixelSY(_T(""))
	, m_addpixelEX(_T(""))
	, m_addpixelEY(_T(""))
{
	m_strINIFileName = CEMUtil::GetExcutePath() + _T("\\") + INI_FILE_NAME;
}

CEMBrowserSettingDlg::~CEMBrowserSettingDlg()
{
}

void CEMBrowserSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_WEB_WIDTH, m_fullscreenWidth);
	DDX_Text(pDX, IDC_EDIT_WEB_HEIGHT, m_fullscreenHeight);
	DDX_Control(pDX, IDC_COMBO_SAVER_TIME, m_cbSaverTime);
	DDX_Control(pDX, IDC_COMBO_SAVER_SECORMIN, m_cbSaverSecOrMin);
	DDX_Control(pDX, IDC_COMBO_AUTOQUIT_HOUR, m_cbAutoquitHour);
	DDX_Control(pDX, IDC_COMBO_AUTOQUIT_MINUTE, m_cbAutoquitMinute);
	DDX_Text(pDX, IDC_EDIT_ADDPIXEL_SX, m_addpixelSX);
	DDX_Text(pDX, IDC_EDIT_ADDPIXEL_SY, m_addpixelSY);
	DDX_Text(pDX, IDC_EDIT_ADDPIXEL_EX, m_addpixelEX);
	DDX_Text(pDX, IDC_EDIT_ADDPIXEL_EY, m_addpixelEY);
}


BEGIN_MESSAGE_MAP(CEMBrowserSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CEMBrowserSettingDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_SCREEN_FULL, &CEMBrowserSettingDlg::UpdateCtrls)
	ON_BN_CLICKED(IDC_RADIO_SCREEN_LT, &CEMBrowserSettingDlg::UpdateCtrls)
	ON_BN_CLICKED(IDC_RADIO_SCREEN_LB, &CEMBrowserSettingDlg::UpdateCtrls)
	ON_BN_CLICKED(IDC_RADIO_SCREEN_RT, &CEMBrowserSettingDlg::UpdateCtrls)
	ON_BN_CLICKED(IDC_RADIO_SCREEN_RB, &CEMBrowserSettingDlg::UpdateCtrls)
	ON_BN_CLICKED(IDC_RADIO_SAVER_SCREEN, &CEMBrowserSettingDlg::UpdateCtrls)
	ON_BN_CLICKED(IDC_RADIO_SAVER_INIT, &CEMBrowserSettingDlg::UpdateCtrls)
	ON_BN_CLICKED(IDC_RADIO_SAVER_NO, &CEMBrowserSettingDlg::UpdateCtrls)
	ON_BN_CLICKED(IDC_RADIO_AUTOQUIT_PROGRAM, &CEMBrowserSettingDlg::UpdateCtrls)
	ON_BN_CLICKED(IDC_RADIO_AUTOQUIT_PC, &CEMBrowserSettingDlg::UpdateCtrls)
	ON_BN_CLICKED(IDC_RADIO_AUTOQUIT_NO, &CEMBrowserSettingDlg::UpdateCtrls)
END_MESSAGE_MAP()


// CEMBrowserSettingDlg 메시지 처리기입니다.


void CEMBrowserSettingDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ReadControlValues();
	WriteINI();

	CDialogEx::OnOK();
}


BOOL CEMBrowserSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CString cbsaverTimes[] = { _T("3"), _T("5"), _T("10"), _T("30"), _T("60"), _T("90"), _T("120"), _T("150") };
	CString cbsaverUnits[] = { _T("분"), _T("초") };
	
	
	
	//콤보박스 세팅
	for(CString s : cbsaverTimes)
		m_cbSaverTime.AddString(s);

	for (CString s : cbsaverUnits)
		m_cbSaverSecOrMin.AddString(s);

	for (int h = 0; h < 24; h++)
	{
		CString hs;
		hs.Format(_T("%d"), h);
		m_cbAutoquitHour.AddString(hs);
	}

	for (int m = 0; m < 60; m += 5)
	{
		CString ms;
		ms.Format(_T("%d"), m);
		m_cbAutoquitMinute.AddString(ms);
	}
		

	


	ReadINI();
	WriteControlValues();
	UpdateCtrls();




	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



bool CEMBrowserSettingDlg::RadioGetCheck(int nIDDlgItem)
{
	return ((CButton*)GetDlgItem(nIDDlgItem))->GetCheck();
}

void CEMBrowserSettingDlg::RadioSetCheck(int nIDDlgItem, bool check)
{
	return ((CButton*)GetDlgItem(nIDDlgItem))->SetCheck(check);
}

void CEMBrowserSettingDlg::ReadControlValues()
{
	UpdateData();

	//fullscreen sel
	if (RadioGetCheck(IDC_RADIO_SCREEN_FULL))
		m_fullscreenSel = 0;
	else if (RadioGetCheck(IDC_RADIO_SCREEN_LT))
		m_fullscreenSel = 1;
	else if (RadioGetCheck(IDC_RADIO_SCREEN_LB))
		m_fullscreenSel = 2;
	else if (RadioGetCheck(IDC_RADIO_SCREEN_RT))
		m_fullscreenSel = 3;
	else if (RadioGetCheck(IDC_RADIO_SCREEN_RB))
		m_fullscreenSel = 4;	


	//화면보호기
	if (RadioGetCheck(IDC_RADIO_SAVER_SCREEN))
		m_saverSel = 0;
	else if (RadioGetCheck(IDC_RADIO_SAVER_INIT))
		m_saverSel = 1;
	else if (RadioGetCheck(IDC_RADIO_SAVER_NO))
		m_saverSel = 2;

	//자동종료
	if (RadioGetCheck(IDC_RADIO_AUTOQUIT_PROGRAM))
		m_autoquitSel = 0;
	if (RadioGetCheck(IDC_RADIO_AUTOQUIT_PC))
		m_autoquitSel = 1;
	if (RadioGetCheck(IDC_RADIO_AUTOQUIT_NO))
		m_autoquitSel = 2;
}


void CEMBrowserSettingDlg::WriteControlValues()
{
	
	switch (m_fullscreenSel)
	{
	case 0:
		RadioSetCheck(IDC_RADIO_SCREEN_FULL);
		break;
	case 1:
		RadioSetCheck(IDC_RADIO_SCREEN_LT);
		break;
	case 2:
		RadioSetCheck(IDC_RADIO_SCREEN_LB);
		break;
	case 3:
		RadioSetCheck(IDC_RADIO_SCREEN_RT);
		break;
	case 4:
		RadioSetCheck(IDC_RADIO_SCREEN_RB);
		break;
	}

	switch (m_saverSel)
	{
	case 0:
		RadioSetCheck(IDC_RADIO_SAVER_SCREEN);
		break;
	case 1:
		RadioSetCheck(IDC_RADIO_SAVER_INIT);
		break;
	case 2:
		RadioSetCheck(IDC_RADIO_SAVER_NO);
		break;
	}

	switch (m_autoquitSel)
	{
	case 0:
		RadioSetCheck(IDC_RADIO_AUTOQUIT_PROGRAM);
		break;
	case 1:
		RadioSetCheck(IDC_RADIO_AUTOQUIT_PC);
		break;
	case 2:
		RadioSetCheck(IDC_RADIO_AUTOQUIT_NO);
		break;
	}

	UpdateData(false);
}


void CEMBrowserSettingDlg::WriteINI()
{
	CString IntToString;
	IntToString.Format(_T("%d"), m_fullscreenSel);	
	WritePrivateProfileString(SECTION_FULLSCREEN, KEY_COMMON_SEL, IntToString, m_strINIFileName);

	WritePrivateProfileString(SECTION_FULLSCREEN, KEY_FULLSCREEN_WIDTH, m_fullscreenWidth, m_strINIFileName);
	WritePrivateProfileString(SECTION_FULLSCREEN, KEY_FULLSCREEN_HEIGHT, m_fullscreenHeight, m_strINIFileName);

	IntToString.Format(_T("%d"), m_saverSel);
	WritePrivateProfileString(SECTION_SCREENSAVER, KEY_COMMON_SEL, IntToString, m_strINIFileName);

	CString comboValue;
	m_cbSaverTime.GetLBText(m_cbSaverTime.GetCurSel(), comboValue);
	WritePrivateProfileString(SECTION_SCREENSAVER, KEY_SCREENSAVER_INTERVAL, comboValue, m_strINIFileName);

	IntToString.Format(_T("%d"), m_cbSaverSecOrMin.GetCurSel());
	WritePrivateProfileString(SECTION_SCREENSAVER, KEY_SCREENSAVER_UNIT, IntToString, m_strINIFileName);

	IntToString.Format(_T("%d"), m_autoquitSel);
	WritePrivateProfileString(SECTION_AUTOOFF, KEY_COMMON_SEL, IntToString, m_strINIFileName);
	
	m_cbAutoquitHour.GetLBText(m_cbAutoquitHour.GetCurSel(), comboValue);
	WritePrivateProfileString(SECTION_AUTOOFF, KEY_AUTOOFF_HOUR, comboValue, m_strINIFileName);

	m_cbAutoquitMinute.GetLBText(m_cbAutoquitMinute.GetCurSel(), comboValue);
	WritePrivateProfileString(SECTION_AUTOOFF, KEY_AUTOOFF_MINUTE, comboValue, m_strINIFileName);


	WritePrivateProfileString(SECTION_EXTENDSCREEN, KEY_EXTENDSCREEN_SX, m_addpixelSX, m_strINIFileName);
	WritePrivateProfileString(SECTION_EXTENDSCREEN, KEY_EXTENDSCREEN_SY, m_addpixelSY, m_strINIFileName);
	WritePrivateProfileString(SECTION_EXTENDSCREEN, KEY_EXTENDSCREEN_EX, m_addpixelEX, m_strINIFileName);
	WritePrivateProfileString(SECTION_EXTENDSCREEN, KEY_EXTENDSCREEN_EY, m_addpixelEY, m_strINIFileName);
}

void CEMBrowserSettingDlg::ReadINI()
{	
	TCHAR szBuf[MAX_PATH];

	m_fullscreenSel = GetPrivateProfileInt(SECTION_FULLSCREEN, KEY_COMMON_SEL, 0, m_strINIFileName);	
	m_fullscreenWidth.Format(_T("%d"), GetPrivateProfileInt(SECTION_FULLSCREEN, KEY_FULLSCREEN_WIDTH, 0, m_strINIFileName));
	m_fullscreenHeight.Format(_T("%d"), GetPrivateProfileInt(SECTION_FULLSCREEN, KEY_FULLSCREEN_HEIGHT, 0, m_strINIFileName));
	
	m_saverSel = GetPrivateProfileInt(SECTION_SCREENSAVER, KEY_COMMON_SEL, 0, m_strINIFileName);
	
	GetPrivateProfileString(SECTION_SCREENSAVER, KEY_SCREENSAVER_INTERVAL, _T(""), szBuf, MAX_PATH, m_strINIFileName);
	if(m_cbSaverTime)
		m_cbSaverTime.SetCurSel(m_cbSaverTime.FindStringExact(0, szBuf));
	m_saverTime = _ttoi(szBuf);

	m_saverSecOrMin = GetPrivateProfileInt(SECTION_SCREENSAVER, KEY_SCREENSAVER_UNIT, 0, m_strINIFileName);
	if(m_cbSaverSecOrMin)
		m_cbSaverSecOrMin.SetCurSel(m_saverSecOrMin);

	m_autoquitSel = GetPrivateProfileInt(SECTION_AUTOOFF, KEY_COMMON_SEL, 0, m_strINIFileName);

	GetPrivateProfileString(SECTION_AUTOOFF, KEY_AUTOOFF_HOUR, _T(""), szBuf, MAX_PATH, m_strINIFileName);
	if (m_cbAutoquitHour)
		m_cbAutoquitHour.SetCurSel(m_cbAutoquitHour.FindStringExact(0, szBuf));
	m_autoquitHour = _ttoi(szBuf);

	GetPrivateProfileString(SECTION_AUTOOFF, KEY_AUTOOFF_MINUTE, _T(""), szBuf, MAX_PATH, m_strINIFileName);
	if (m_cbAutoquitMinute)
		m_cbAutoquitMinute.SetCurSel(m_cbAutoquitMinute.FindStringExact(0, szBuf));
	m_autoquitMinute = _ttoi(szBuf);


	m_addpixelSX.Format(_T("%d"), GetPrivateProfileInt(SECTION_EXTENDSCREEN, KEY_EXTENDSCREEN_SX, 0, m_strINIFileName));
	m_addpixelSY.Format(_T("%d"), GetPrivateProfileInt(SECTION_EXTENDSCREEN, KEY_EXTENDSCREEN_SY, 0, m_strINIFileName));
	m_addpixelEX.Format(_T("%d"), GetPrivateProfileInt(SECTION_EXTENDSCREEN, KEY_EXTENDSCREEN_EX, 0, m_strINIFileName));
	m_addpixelEY.Format(_T("%d"), GetPrivateProfileInt(SECTION_EXTENDSCREEN, KEY_EXTENDSCREEN_EY, 0, m_strINIFileName));
}

void CEMBrowserSettingDlg::UpdateCtrls()
{
	if (RadioGetCheck(IDC_RADIO_SCREEN_FULL))
	{
		GetDlgItem(IDC_EDIT_WEB_WIDTH)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_WEB_HEIGHT)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_EDIT_WEB_WIDTH)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_WEB_HEIGHT)->EnableWindow(true);
	}

	if (RadioGetCheck(IDC_RADIO_SAVER_NO))
	{
		GetDlgItem(IDC_COMBO_SAVER_TIME)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_SAVER_SECORMIN)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_COMBO_SAVER_TIME)->EnableWindow(true);
		GetDlgItem(IDC_COMBO_SAVER_SECORMIN)->EnableWindow(true);
	}

	if (RadioGetCheck(IDC_RADIO_AUTOQUIT_NO))
	{
		GetDlgItem(IDC_COMBO_AUTOQUIT_HOUR)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_AUTOQUIT_MINUTE)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_COMBO_AUTOQUIT_HOUR)->EnableWindow(true);
		GetDlgItem(IDC_COMBO_AUTOQUIT_MINUTE)->EnableWindow(true);
	}

}

