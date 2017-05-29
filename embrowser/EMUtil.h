#pragma once
class CEMUtil
{
private:
	static char GetNtPrivilege();

public:
	CEMUtil();
	~CEMUtil();

	static CString GetExcutePath(bool onlyPath = true);

	//현재 경로 가져오기
	static void PowerOff();
};

