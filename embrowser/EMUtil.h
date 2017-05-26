#pragma once
class CEMUtil
{
public:
	CEMUtil();
	~CEMUtil();

	static CString GetExcutePath(bool onlyPath = true);	//현재 경로 가져오기
};

