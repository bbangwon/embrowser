#include "stdafx.h"
#include "EMUtil.h"


CEMUtil::CEMUtil()
{
}


CEMUtil::~CEMUtil()
{
}


CString CEMUtil::GetExcutePath(bool onlyPath)
{
	TCHAR chrFileName[MAX_PATH];
	GetModuleFileName(nullptr, chrFileName, MAX_PATH);

	CString retPath = chrFileName;
	
	if(onlyPath)
		retPath = retPath.Left(retPath.ReverseFind('\\'));
	return retPath;
}


// ���� �ü���� NT �迭�� ���, SE_SHUTDOWN_NAME ������ ���� �����
// ������ �־�� �ϱ� ������ �Ʒ��� �Լ��� �̿��Ͽ� ������ ����ϴ�.
char CEMUtil::GetNtPrivilege()
{
	HANDLE h_token;
	TOKEN_PRIVILEGES privilege_info;

	// ���� ���μ����� ���Ѱ� ���õ� ������ �����ϱ� ���� ��ū������ ���ϴ�.
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES |
		TOKEN_QUERY, &h_token)) {
		// ���Ѱ� ���õ� ���� ���ٿ� ������..
		return 0;
	}

	// ���� ���μ����� SE_SHUTDOWN_NAME ������ ����Ҽ� �ֵ��� �����Ѵ�.
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &privilege_info.Privileges[0].Luid);
	privilege_info.PrivilegeCount = 1;
	privilege_info.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// ������ ������ ������ �����Ѵ�.
	AdjustTokenPrivileges(h_token, FALSE, &privilege_info, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	if (GetLastError() != ERROR_SUCCESS) {
		// ���� ������ ������ ���...
		return 0;
	}

	return 1;
}

// ��ǻ�͸� ���� ����� �Լ�
void CEMUtil::PowerOff()
{
	if (GetNtPrivilege()) {
		// ���� �ü���� Windows NT, Windows 2000, Windows XP �� ���...
		ExitWindowsEx(EWX_FORCE | EWX_POWEROFF, 0);
	}
}
