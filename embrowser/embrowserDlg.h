
// embrowserDlg.h : ��� ����
//

#pragma once
#include "explorer_control.h"
#include "EMBrowserSettingDlg.h"
#include "EMMP3.h"

// CembrowserDlg ��ȭ ����
class CembrowserDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CembrowserDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EMBROWSER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
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
