#pragma once
class CEMUtil
{
private:
	static char GetNtPrivilege();

public:
	CEMUtil();
	~CEMUtil();

	static CString GetExcutePath(bool onlyPath = true);

	//���� ��� ��������
	static void PowerOff();
};

