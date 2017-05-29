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


// 현재 운영체제가 NT 계열인 경우, SE_SHUTDOWN_NAME 레벨의 권한 등급을
// 가지고 있어야 하기 때문에 아래의 함수를 이용하여 권한을 얻습니다.
char CEMUtil::GetNtPrivilege()
{
	HANDLE h_token;
	TOKEN_PRIVILEGES privilege_info;

	// 현재 프로세스의 권한과 관련된 정보를 변경하기 위해 토큰정보를 엽니다.
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES |
		TOKEN_QUERY, &h_token)) {
		// 권한과 관련된 정보 접근에 실패함..
		return 0;
	}

	// 현재 프로세스가 SE_SHUTDOWN_NAME 권한을 사용할수 있도록 설정한다.
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &privilege_info.Privileges[0].Luid);
	privilege_info.PrivilegeCount = 1;
	privilege_info.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// 지정한 값으로 권한을 조정한다.
	AdjustTokenPrivileges(h_token, FALSE, &privilege_info, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	if (GetLastError() != ERROR_SUCCESS) {
		// 권한 조정에 실패한 경우...
		return 0;
	}

	return 1;
}

// 컴퓨터를 끌때 사용할 함수
void CEMUtil::PowerOff()
{
	if (GetNtPrivilege()) {
		// 현재 운영체제가 Windows NT, Windows 2000, Windows XP 인 경우...
		ExitWindowsEx(EWX_FORCE | EWX_POWEROFF, 0);
	}
}
