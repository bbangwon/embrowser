#include "stdafx.h"
#include "EMMP3.h"

#pragma comment(lib, "winmm.lib")

CEMMP3::CEMMP3()
{
}


CEMMP3::~CEMMP3()
{
}


void CEMMP3::Open(CString fileName)
{
	m_mciOpen.lpstrElementName = fileName;
	m_mciOpen.lpstrDeviceType = L"mpegvideo";
	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&m_mciOpen);
	mciSendCommand(m_dwID, MCI_PLAY, MCI_NOTIFY, (DWORD)&m_mciPlay);

	m_dwID = m_mciOpen.wDeviceID;
}

void CEMMP3::Play()
{
	mciSendCommand(m_dwID, MCI_PLAY, MCI_NOTIFY, (DWORD)&m_mciPlay);
}

void CEMMP3::Pause()
{
	mciSendCommand(m_dwID, MCI_PAUSE, MCI_NOTIFY, (DWORD)(LPVOID)&m_mciPlay);
}

void CEMMP3::Stop()
{
	mciSendCommand(m_dwID, MCI_CLOSE, 0, NULL);
}

bool CEMMP3::IsPlay()
{
	if (_GetMCIState(MCI_STATUS_MODE) == MCI_MODE_PLAY)
		return true;
	return false;
}

bool CEMMP3::IsPause()
{
	if (_GetMCIState(MCI_STATUS_MODE) == MCI_MODE_PAUSE)
		return true;
	return false;
}

bool CEMMP3::IsStop()
{
	if (_GetMCIState(MCI_STATUS_MODE) == MCI_MODE_STOP)
		return true;
	return false;
}

int CEMMP3::GetPlayTime()
{
	DWORD playtime = _GetMCIState(MCI_STATUS_LENGTH);
	return (playtime / 1000);
}

int CEMMP3::GetCurrentPos()
{
	DWORD currenttime = _GetMCIState(MCI_STATUS_POSITION);
	return (currenttime / 1000);
}

DWORD CEMMP3::_GetMCIState(int state)
{
	DWORD dwMode;
	MCI_STATUS_PARMS mciStatusParms;
	mciStatusParms.dwItem = state;
	mciSendCommand(m_dwID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&mciStatusParms);
	dwMode = (LONG)mciStatusParms.dwReturn;
	return dwMode;
}