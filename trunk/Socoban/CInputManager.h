#pragma once
#include <windows.h>
#include <windowsx.h>
#include <dinput.h>

class CInputManager
{
public:
	CInputManager(void);
	virtual ~CInputManager(void);

	void Release (void);
	BOOL Initialize (HINSTANCE hInstance, HWND hwnd);

	static CInputManager* GetInstance()
	{
		if (NULL == m_pInputManager)
		{
			m_pInputManager = new CInputManager();
		}
		return m_pInputManager;
	}

	LPDIRECTINPUTDEVICE GetKeyBoardDevice()
	{
		return m_lpKeyboard;
	}

	void SyncAcquire (BOOL bPaused, HWND hwnd);

protected:

private:
	LPDIRECTINPUT			m_lpDirectInput ;
	LPDIRECTINPUTDEVICE		m_lpKeyboard ;

	HWND m_hWnd;
	static CInputManager* m_pInputManager;
};
