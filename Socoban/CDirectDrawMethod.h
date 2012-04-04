#pragma once
#include "GameMain.h"

#define DDRAW_INIT_STRUCT(ddstruct) { memset(&ddstruct,0,sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct); }
#define KEYDOWN(State,KeyID)	(State[KeyID] & 0x80) 

class DirectDrawMethod
{
public:
	DirectDrawMethod(void);
	virtual ~DirectDrawMethod(void);
	
	BOOL Initialize(HWND hWnd);

	HRESULT	Copy_Bitmap (LPDIRECTDRAWSURFACE pDDS, 
		                 HBITMAP hBitmap, 
						 INT iX=0, 
						 INT iY=0, 
						 INT iDeltaX = 0, 
						 INT iDeltaY = 0);

	int DirectDrawMethod::Blit_Surface(	LPDIRECTDRAWSURFACE dest,    // surface to draw the surface on
				 int x, int y,                 // position to draw at
				 int width, int height,					  
				 LPDIRECTDRAWSURFACE source, // source surface to draw 
				 int src_x, int src_y,
				 int src_width, int src_height,
				 int transparent);
	void FillSurface (LPDIRECTDRAWSURFACE lpSurface, 
		              DWORD dwColor);

	LPDIRECTDRAWSURFACE Create_Surface (INT iWidth, 
		                                INT iHeight, 
										INT iMemFlags = DDSCAPS_SYSTEMMEMORY, 
										DWORD dwColorKey = 0);

	LPDIRECTDRAWSURFACE	Load_Bitmap (LPCSTR szBitmap_name, 
									 COLORREF dwColorKey, 
									 INT iDx = 0, 
									 INT iDy = 0);

	HRESULT Set_ColorKey(LPDIRECTDRAWSURFACE pdds, COLORREF rgb);

	HRESULT  UninitDirectDraw( void );

	HRESULT  InitDirectDraw (HWND	hMain, 
		                     INT iXRes, 
							 INT iYRes, 
							 INT iColordepth );


	LPDIRECTDRAWSURFACE DirectDrawMethodCreate_Surface(int width, int height, int mem_flags, DWORD color_key);

	static DirectDrawMethod* GetInstance()
	{
		if (NULL == m_pDirectDrawManager)
		{
			m_pDirectDrawManager = new DirectDrawMethod();
		}
		return m_pDirectDrawManager;
	}

	BOOL Render (INT* ppPoint1, INT* ppPoint2, INT iHeroX, INT iHeroY, INT iSize = 20);

protected:

private:
	LPDIRECTDRAW2	lpDD2;
	//LPDIRECTDRAWSURFACE lpPrimarySurface, lpBackSurface,target_ball2,target_ball1,BGSurface;
	//LPDIRECTDRAWSURFACE BackGroundSurface,misaile,ball1,pSurface;

	DWORD	Pitch;
	DDSURFACEDESC        m_ddsd;  
		
	HWND m_hWnd;
	static DirectDrawMethod* m_pDirectDrawManager;

	CDisplay*            g_pDisplay;
	CSurface*            g_Surface1;  
	CSurface*            g_Surface2;  
	CSurface*            g_Surface3; 
	CSurface*            g_Surface4; 
	CSurface*            g_Surface5; 
	CSurface*		     g_TextSurface;
	CSurface*		     g_TextSurface2;

	HBRUSH hbrush1,hbrush2;
	HPEN hpen1,hpen2;

	HFONT hfont;
};
