// ��ǻ�Ϳ��� Microsoft Visual C++�� ����Ͽ� ������ IDispatch ���� Ŭ�����Դϴ�.

// ����: �� ������ ������ �������� ���ʽÿ�.  Microsoft Visual C++����
//  �� Ŭ������ �ٽ� ������ �� ������ ������ ����ϴ�.


#include "stdafx.h"
#include "explorer_control.h"
#include "embrowserDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CExplorer_control

IMPLEMENT_DYNCREATE(CExplorer_control, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CExplorer_control �Ӽ��Դϴ�.

/////////////////////////////////////////////////////////////////////////////
// CExplorer_control �۾��Դϴ�.


BOOL CExplorer_control::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	CembrowserDlg* pp = ((CembrowserDlg*)parentPtr);
	if (pp->currentState == CembrowserDlg::SCREENSTATE::SCREENSAVER)
	{
		if (pMsg->message == WM_LBUTTONDOWN)
		{
			OutputDebugString(_T("KeyDown"));			
			((CembrowserDlg*)parentPtr)->ShowIndexHTML();
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}
