#pragma once
#include <windows.h>
#include <windowsx.h>

#define SOCOBAN_MAX_WIDTH 20
#define SOCOBAN_MAX_HEIGHT 20

enum  enumSpaceProperty
{		
	ENUM_SPACE_EMPTY,
	ENUM_SPACE_BLOCK,	
	ENUM_SPACE_BOX_POINT,
	ENUM_SPACE_BOX,
	ENUM_SPACE_ENEMY,
};

class CGameCore
{	
public:	
	CGameCore(void);
	virtual ~CGameCore(void);

	BOOL Initialize(HINSTANCE hInstance, HWND hWnd);

	static CGameCore* GetInstance()
	{
		if (NULL == m_pGameCore)
		{
			m_pGameCore = new CGameCore();
		}
		return m_pGameCore;
	}

	static LRESULT CALLBACK WndProc (HWND opening, UINT iMsg, WPARAM wParam, LPARAM lParam);

	BOOL InitStage();
	void UpdateGameStatus();
	void ProcessInput (HWND hwnd);
	BOOL ProcessGame();
	void Render();
	BOOL ProcessDirective(INT iX, INT iY);

	BOOL GetPause()
	{
		return m_bPaused;
	}
	

protected:

private:
	static CGameCore* m_pGameCore;
	static BOOL m_bPaused;
	static HWND m_hWnd;

	static BOOL m_bLevelReset;
	static INT iStageNum;

	int array_value[SOCOBAN_MAX_WIDTH][SOCOBAN_MAX_HEIGHT];
	int array_value1[SOCOBAN_MAX_WIDTH][SOCOBAN_MAX_HEIGHT];
	int array_value2[SOCOBAN_MAX_WIDTH][SOCOBAN_MAX_HEIGHT];
	int hero_x;
	int hero_y;
	
	int iTotalBoxNum;
	int iCurrentExactBoxCount;
};
