#include "CGameCore.h"
#include "CGameMusicManager.h"
#include "CDirectDrawMethod.h"
#include "CInputManager.h"

CGameCore* CGameCore::m_pGameCore = NULL;
BOOL CGameCore::m_bPaused = FALSE;
HWND CGameCore::m_hWnd = NULL;
BOOL CGameCore::m_bLevelReset = FALSE;
INT CGameCore::iStageNum = 1;

CGameCore::CGameCore(void)
{
	hero_x=7;
	hero_y=11;
	
	iTotalBoxNum = 0;
	iCurrentExactBoxCount = 0;
}

CGameCore::~CGameCore(void)
{
}

BOOL CGameCore::Initialize(HINSTANCE hInstance, HWND hWnd)
{
	DirectDrawMethod::GetInstance()->Initialize(hWnd);	

	CGameMusicManager::GetInstance()->Initialize(hWnd);

	CInputManager::GetInstance()->Initialize(hInstance, hWnd);

	return TRUE;
}

BOOL CGameCore::InitStage()
{
	FILE *fp;	
	iCurrentExactBoxCount = 0;
	iTotalBoxNum = 0;
	CHAR szFileName[256];

	int iIndex1 = 0;
	int iIndex2 =0;
	char cChar;

	if(FALSE == m_bLevelReset)		
	{
		hero_x=7;hero_y=11;
		//hero_x=9;hero_y=8;
		//hero_x=9;hero_y=9;

		sprintf(szFileName, "map%d.txt", iStageNum);			
		fp=fopen(szFileName,"rb");		

		if(fp==NULL)
		{
			MessageBox(m_hWnd,"맵이 존재하지 않습니다. 게임을 종료합니다.","맵이 없어요",MB_OK);
			return FALSE;
		}
		else 
		{
			cChar=getc(fp);

			while(cChar != EOF)
			{ 
//////////////////////////////////////////////////////////////////////////////////////////
//2002년에 간단히 만든 것을 2008년 11월에 소스를 다시 보니 웃음이 절로 솟는 군
//밑에 세줄의 의미를 잘 모르겠다.
//////////////////////////////////////////////////////////////////////////////////////////
				if (iIndex2 > SOCOBAN_MAX_HEIGHT - 1)
				{
					iIndex2 = -2;
					iIndex1++;
				}  

				INT iSpaceProperty = (INT)cChar;
				iSpaceProperty = iSpaceProperty - 48;

				switch (iSpaceProperty)
				{			
				case ENUM_SPACE_EMPTY:
					{
						array_value[iIndex1][iIndex2] = ENUM_SPACE_EMPTY;
					}
					break;
				case ENUM_SPACE_BLOCK:
					{
						array_value[iIndex1][iIndex2] = ENUM_SPACE_BLOCK;
					}
					break;
				case ENUM_SPACE_BOX_POINT:
					{
						array_value[iIndex1][iIndex2] = ENUM_SPACE_BOX_POINT;
					}
					break;
				case ENUM_SPACE_BOX: // 밀 수 있는 박스
					{
						array_value[iIndex1][iIndex2] = ENUM_SPACE_BOX;
					}
					break;
				case ENUM_SPACE_ENEMY:
					{
						array_value[iIndex1][iIndex2] = ENUM_SPACE_ENEMY;
					}
					break;
				}

				iIndex2++;

				cChar = getc(fp);	
			}


		}

		fclose(fp);

		for (iIndex1=0; iIndex1 < SOCOBAN_MAX_WIDTH; iIndex1++)
		{
			for (iIndex2 = 0; iIndex2 < SOCOBAN_MAX_HEIGHT; iIndex2++)
			{
				if(array_value[iIndex1][iIndex2] == ENUM_SPACE_BOX)			
				{
					array_value1[iIndex1][iIndex2] = array_value[iIndex1][iIndex2];		
					iTotalBoxNum++;
				}
				else 
				{
					array_value1[iIndex1][iIndex2] = ENUM_SPACE_EMPTY;
				}

				if(array_value[iIndex1][iIndex2] == ENUM_SPACE_BOX_POINT)	
				{
					array_value2[iIndex1][iIndex2] = array_value[iIndex1][iIndex2];
				}
				else
				{
					array_value2[iIndex1][iIndex2] = ENUM_SPACE_EMPTY;
				}

			}
		}

		m_bLevelReset = TRUE;
	}

	return TRUE;
}


void CGameCore::UpdateGameStatus(){
	int miro_mode=0,iCurrentExactBoxCount=0;
	int i,j,s=0;
	int direct=2;
	for(i=0;i<20;i++)
		for(j=0;j<20;j++)
			if(array_value[i][j]== ENUM_SPACE_BOX && array_value2[i][j] == ENUM_SPACE_BOX_POINT)
				iCurrentExactBoxCount++;

	if (iCurrentExactBoxCount == iTotalBoxNum)
	{ 
		MessageBox(m_hWnd,"축하합니다. 다음 단계로 넘어갑니다.","성공!",MB_OK);	
		m_bLevelReset = 0;	
		iStageNum++;	
		iTotalBoxNum = 0;
	}

	iCurrentExactBoxCount=0;
}

void CGameCore::ProcessInput (HWND hwnd)
{
	if (CInputManager::GetInstance()->GetKeyBoardDevice())
	{
		BYTE KeyState[256] ;
		HRESULT hResult ;

		for (;;)
		{
			hResult = CInputManager::GetInstance()->GetKeyBoardDevice()->GetDeviceState (sizeof (KeyState), &KeyState) ;

			if (hResult == DIERR_INPUTLOST)
			{
				hResult = CInputManager::GetInstance()->GetKeyBoardDevice()->Acquire () ;

				if (hResult != DI_OK)
				{
					break ;
				}
			}
			else break ;
		}

		if (hResult == DI_OK)
		{

			if (KEYDOWN(KeyState, DIK_UP)) 
			{  
				ProcessDirective(0, -1);
			}
			else if (KEYDOWN(KeyState, DIK_LEFT)) 
			{
				ProcessDirective(-1, 0);
			}
			else if (KEYDOWN(KeyState, DIK_DOWN)) 
			{
				ProcessDirective(0, 1);
			}
			else if (KEYDOWN(KeyState, DIK_RIGHT)) 
			{
				ProcessDirective(1, 0);
			}			
		}
	}     
}

BOOL CGameCore::ProcessGame()
{
	if(m_bLevelReset == FALSE)
		InitStage();

	ProcessInput(m_hWnd);

	Render();
	UpdateGameStatus();

	return TRUE;
}

void CGameCore::Render()
{	
	DirectDrawMethod::GetInstance()->Render(&(array_value[0][0]), &(array_value2[0][0]), hero_x, hero_y);	
}

BOOL CGameCore::ProcessDirective(INT iX, INT iY)
{
	hero_x += iX;
	hero_y += iY;
	if(array_value[hero_y][hero_x] == ENUM_SPACE_BLOCK)
	{
		hero_x -= iX;
		hero_y -= iY;
		return FALSE;
	}

	else if(array_value[hero_y][hero_x] == ENUM_SPACE_BOX)
	{
		if (array_value[hero_y + iY][hero_x + iX] == ENUM_SPACE_EMPTY || 
			array_value[hero_y + iY][hero_x + iX] == ENUM_SPACE_BOX_POINT)				
		{
			array_value[hero_y][hero_x] = ENUM_SPACE_EMPTY;				
			array_value[hero_y + iY][hero_x + iX] = ENUM_SPACE_BOX;
		}				
		else 
		{
			hero_x -= iX;
			hero_y -= iY;
			return FALSE;
		}
	}

	return TRUE;
}

LRESULT CALLBACK CGameCore::WndProc (HWND opening, UINT iMsg, WPARAM wParam, LPARAM lParam)
{ 
	switch (iMsg)
	{		case WM_MOUSEMOVE:
	return 0;
	case WM_MOVE:
		//if( g_pDisplay )
		//	g_pDisplay->UpdateBounds();
		return 0L;

	case WM_ACTIVATE:
		if (wParam == WA_INACTIVE)
			m_bPaused = TRUE ;
		else
			m_bPaused = FALSE ;
		CInputManager::GetInstance()->SyncAcquire (m_bPaused, m_hWnd) ;
		break;
	case WM_KEYDOWN:
		switch (wParam) 
		{
		case VK_ESCAPE:
			CInputManager::GetInstance()->Release();
			DirectDrawMethod::GetInstance()->UninitDirectDraw();					
			DestroyWindow(m_hWnd);
			PostQuitMessage (0) ;
			break;
		case 'P':				
			m_bLevelReset = FALSE;			
			break;

		}
		return TRUE;

	case WM_DESTROY :
		DirectDrawMethod::GetInstance()->UninitDirectDraw();					
		PostQuitMessage (0) ;
		break;

	}
	return DefWindowProc (opening, iMsg, wParam, lParam) ;

}


