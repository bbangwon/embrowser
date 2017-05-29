
// embrowserDlg.h : 헤더 파일
//

#pragma once
#include "explorer_control.h"
#include "EMBrowserSettingDlg.h"
#include "EMMP3.h"

// CembrowserDlg 대화 상자
class CembrowserDlg : public CDialogEx
{
// 생성입니다.
public:
	CembrowserDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EMBROWSER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	enum class SCREENSTATE {
		NORMAL,
		SCREENSAVER
	} currentState = SCREENSTATE::NORMAL;	

	CExplorer_control m_control;
	CEMBrowserSettingDlg m_settingDlg;
	void Start();
	void QuitProcess();
	void ShowIndexHTML();
	void ShowSaverHTML();
	void SetScreenSize();
	void SetScreenSaver();

	void ProcScreenSaver();

	void BGMPlay();
	void BGMStop();

	CRect m_backgroundScreenRect;
	CEMMP3 m_mp3;
	int saverTime;

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
