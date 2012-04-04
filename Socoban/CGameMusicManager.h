#pragma once
#include <dmusicc.h>
#include <dmusici.h>
#include "DirectMusicUtil.h"
#include "DirectXUtil.h"

#define MUSIC_VOLUME_RANGE      ( 0-(DMUS_VOLUME_MIN/4) )

class CGameMusicManager
{
public:
	CGameMusicManager(void);
	virtual ~CGameMusicManager(void);

	BOOL Initialize(HWND hWnd);

	HRESULT ProcessDirectMusicMessages( HWND hDlg );
	VOID    OnOpenSoundFile( HWND hDlg );
	HRESULT LoadSegmentFile( HWND hDlg, TCHAR* strFileName );
	HRESULT OnPlayAudio( HWND hDlg );
	VOID    EnablePlayUI( HWND hDlg, BOOL bEnable );

	static CGameMusicManager* GetInstance()
	{
		if (NULL == m_pGameMusicManager)
		{
			m_pGameMusicManager = new CGameMusicManager();
		}
		return m_pGameMusicManager;
	}

protected:

private:
	HWND m_hWnd;
	//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------
	CMusicManager*     m_pMusicManager;
	CMusicSegment*     m_pMusicSegment;
	HANDLE             m_hDMusicMessageEvent;
	IDirectMusicPerformance* m_pPerf;

	static CGameMusicManager* m_pGameMusicManager;
};
