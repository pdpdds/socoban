#include "CInputManager.h"
#include <dinput.h>

CInputManager* CInputManager::m_pInputManager = NULL;

CInputManager::CInputManager(void)
{
	m_lpDirectInput = NULL;
	m_lpKeyboard = NULL;
}

CInputManager::~CInputManager(void)
{
}

BOOL CInputManager::Initialize (HINSTANCE hInstance, HWND hwnd)
{
	HRESULT hResult ;

	hResult = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID *)&m_lpDirectInput, NULL) ;	

	if (hResult != DI_OK)
	{
		MessageBox (hwnd, "DirectInputCreate 에러", "키보드입력", MB_OK) ;
		return FALSE ;
	}

	hResult = m_lpDirectInput->CreateDevice (GUID_SysKeyboard, &m_lpKeyboard, NULL) ;

	if (hResult != DI_OK)
	{
		MessageBox (hwnd, "CreateDevice 에러", "키보드입력", MB_OK) ;
		return FALSE ;
	}

	hResult = m_lpKeyboard->SetDataFormat (&c_dfDIKeyboard) ;

	if (hResult != DI_OK)
	{
		MessageBox (hwnd, "SetDataFormat 에러", "키보드입력", MB_OK) ;
		return FALSE ;
	}

	hResult = m_lpKeyboard->SetCooperativeLevel
		(hwnd, DISCL_NONEXCLUSIVE |DISCL_BACKGROUND/* DISCL_FOREGROUND*/) ;

	if (hResult != DI_OK)
	{
		MessageBox (hwnd, "SetCooperativeLevel 에러", "키보드입력", MB_OK) ;
		return FALSE ;
	}
	if(FAILED(m_lpKeyboard->Acquire()))
		exit(0);

	return TRUE ;
}


void CInputManager::Release (void)
{
	if (m_lpKeyboard)
	{
		m_lpKeyboard->Unacquire () ;
		m_lpKeyboard->Release () ;
		m_lpKeyboard = NULL ;
	}
	if (m_lpDirectInput)
	{
		m_lpDirectInput->Release () ;
		m_lpDirectInput = NULL ;
	}

}

void CInputManager::SyncAcquire (BOOL bPaused, HWND hwnd)
{
	if (bPaused)
	{
		if (m_lpKeyboard)
			m_lpKeyboard->Unacquire () ;
	}
	else
	{
		if (m_lpKeyboard)
			m_lpKeyboard->Acquire () ;
	}
}
