
// embrowser.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CembrowserApp:
// �� Ŭ������ ������ ���ؼ��� embrowser.cpp�� �����Ͻʽÿ�.
//

class CembrowserApp : public CWinApp
{
public:
	CembrowserApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CembrowserApp theApp;
