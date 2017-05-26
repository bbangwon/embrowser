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
