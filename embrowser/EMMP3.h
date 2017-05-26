#pragma once

#include "mmsystem.h"


class CEMMP3
{
public:
	CEMMP3();
	~CEMMP3();

	void Open(CString fileName);
	void Play();
	void Pause();
	void Stop();

	bool IsPlay();
	bool IsPause();
	bool IsStop();

	int GetPlayTime();
	int GetCurrentPos();

	MCI_OPEN_PARMS m_mciOpen;
	MCI_PLAY_PARMS m_mciPlay;
	MCI_STATUS_PARMS m_mciStatus;
	DWORD m_dwID;

private:
	DWORD _GetMCIState(int state);
};

