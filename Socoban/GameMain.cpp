#include "GameMain.h"
#include "CGameCore.h"

HINSTANCE g_hInst;
HWND g_hWnd;

#define SPRITE_DIAMETER 30


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nCmdShow )
{

	g_hInst = hInstance;
	static char Name[] = "Sokoban" ;	
	MSG         msg ;
	WNDCLASS  wndclass ;
	wndclass.style         =CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW; 
	wndclass.lpfnWndProc   = CGameCore::WndProc ;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = 0 ;
	wndclass.hInstance     = (HINSTANCE)GetWindowLong(NULL,GWL_HINSTANCE);
	wndclass.hIcon         = LoadIcon (NULL,"YUKINO") ;
	wndclass.hCursor       = LoadCursor (NULL,IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); 
	wndclass.lpszMenuName  = NULL;
	wndclass.lpszClassName = Name ;

	RegisterClass (&wndclass) ;

	g_hWnd = CreateWindow (Name,     
		"Sokoban",WS_POPUP | WS_VISIBLE,   
		200,
		60,  400,
		500,       

		NULL,                 
		NULL,                 
		(HINSTANCE)GetWindowLong(NULL,GWL_HINSTANCE),            
		NULL) ;		          

	ShowWindow (g_hWnd,SW_SHOW) ;
	UpdateWindow (g_hWnd) ;
			
	CGameCore::GetInstance()->Initialize(hInstance, g_hWnd);	
	
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))

		{
			if (!GetMessage(&msg, NULL, 0, 0))
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		else if (CGameCore::GetInstance()->GetPause())
		{
			WaitMessage();
		}
		else
		{
			CGameCore::GetInstance()->ProcessGame();

		}

	}
	
	return 0;
}








