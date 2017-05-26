// 컴퓨터에서 Microsoft Visual C++를 사용하여 생성한 IDispatch 래퍼 클래스입니다.

// 참고: 이 파일의 내용을 수정하지 마십시오.  Microsoft Visual C++에서
//  이 클래스를 다시 생성할 때 수정한 내용을 덮어씁니다.


#include "stdafx.h"
#include "explorer_control.h"
#include "embrowserDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CExplorer_control

IMPLEMENT_DYNCREATE(CExplorer_control, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CExplorer_control 속성입니다.

/////////////////////////////////////////////////////////////////////////////
// CExplorer_control 작업입니다.


BOOL CExplorer_control::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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
