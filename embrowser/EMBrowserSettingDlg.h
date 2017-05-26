#pragma once
#include "afxwin.h"


// CEMBrowserSettingDlg 대화 상자입니다.

class CEMBrowserSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEMBrowserSettingDlg)

public:
	CEMBrowserSettingDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEMBrowserSettingDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	void WriteINI();
	void ReadINI();
	
	bool RadioGetCheck(int nIDDlgItem);
	void RadioSetCheck(int nIDDlgItem, bool check=true);

	void ReadControlValues();
	void WriteControlValues();

	void UpdateCtrls();

	CString m_strINIFileName;
	int m_fullscreenSel;

	CString m_fullscreenWidth;
	CString m_fullscreenHeight;

	int m_saverSel;
	CComboBox m_cbSaverTime;
	int m_saverTime;
	CComboBox m_cbSaverSecOrMin;
	int m_saverSecOrMin;

	int m_autoquitSel;
	
	CComboBox m_cbAutoquitHour;
	int m_autoquitHour;
	CComboBox m_cbAutoquitMinute;
	int m_autoquitMinute;
	CString m_addpixelSX;
	CString m_addpixelSY;
	CString m_addpixelEX;
	CString m_addpixelEY;
};
