#include "CGameMusicManager.h"
#include <stdio.h>

CGameMusicManager* CGameMusicManager::m_pGameMusicManager = NULL;

CGameMusicManager::CGameMusicManager(void)
{
	m_pMusicManager = NULL;
	m_pMusicSegment = NULL;
	m_hDMusicMessageEvent = NULL;
	m_pPerf = NULL;
	m_hWnd = NULL;
}

CGameMusicManager::~CGameMusicManager(void)
{
	SAFE_DELETE( m_pMusicSegment );		
	SAFE_DELETE( m_pMusicManager );		
	CloseHandle( m_hDMusicMessageEvent );
}

BOOL CGameMusicManager::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;

	m_hDMusicMessageEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
	m_pMusicManager = new CMusicManager();

	if( FAILED( m_pMusicManager->Initialize(m_hWnd) ) )
		exit(0);

	// Register segment notification
	m_pPerf = m_pMusicManager->GetPerformance();
	GUID guid = GUID_NOTIFICATION_SEGMENT;
	m_pPerf->AddNotificationType( guid );
	m_pPerf->SetNotificationHandle( m_hDMusicMessageEvent, 0 );  

	OnPlayAudio(m_hWnd);

	return TRUE;
}

VOID CGameMusicManager::OnOpenSoundFile( HWND hDlg ) 
{
	static TCHAR strFileName[MAX_PATH] = TEXT("");
	static TCHAR strPath[MAX_PATH] = TEXT("");

	// Get the default media path (something like C:\MSSDK\SAMPLES\DMUSIC\MEDIA)
	if( '\0' == strPath[0] )
	{
		const TCHAR* szDir = DXUtil_GetDXSDKMediaPath();
		strcpy( strPath, szDir );
	}

	// Setup the OPENFILENAME structure
	OPENFILENAME ofn = { sizeof(OPENFILENAME), hDlg, NULL,
		TEXT("DirectMusic Content Files\0*.sgt;*.mid;*.rmi\0Wave Files\0*.wav\0All Files\0*.*\0\0"), NULL,
		0, 1, strFileName, MAX_PATH, NULL, 0, strPath,
		TEXT("Open Content File"),
		OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, 0, 0,
		TEXT(".sgt"), 0, NULL, NULL };

	if( m_pMusicSegment )
		m_pMusicSegment->Stop( 0 );


	// Display the OpenFileName dialog. Then, try to load the specified file
	if( TRUE != GetOpenFileName( &ofn ) )
	{

		return;
	}

	if( S_FALSE == LoadSegmentFile( hDlg, strFileName ) )
	{
		// Not a critical failure, so just update the status

	}

	// Remember the path for next time
	strcpy( strPath, strFileName );
	char* strLastSlash = strrchr( strPath, '\\' );
	strLastSlash[0] = '\0';
}
HRESULT CGameMusicManager::LoadSegmentFile( HWND hDlg, TCHAR* strFileName )
{

	char buf[256];
	sprintf(buf,"%s",strFileName);


	// Free any previous segment, and make a new one
	SAFE_DELETE( m_pMusicSegment );

	// Have the loader collect any garbage now that the old 
	// segment has been released
	m_pMusicManager->CollectGarbage();

	// Set the media path based on the file name (something like C:\MEDIA)
	// to be used as the search directory for finding DirectMusic content
	// related to this file.
	//   TCHAR strMediaPath[MAX_PATH];
	// _tcscpy( strMediaPath, strFileName );
	//   TCHAR* strLastSlash = _tcsrchr(strMediaPath, TEXT('\\'));
	//   *strLastSlash = 0;
	//   if( FAILED( hr = m_pMusicManager->SetSearchDirectory( strMediaPath ) ) )
	//       return 0;

	// For DirectMusic must know if the file is a standard MIDI file or not
	// in order to load the correct instruments.
	BOOL bMidiFile = FALSE;
	if( strstr( strFileName, ".mid" ) != NULL ||
		strstr( strFileName, ".rmi" ) != NULL ) 
	{
		bMidiFile = TRUE;
	}

	BOOL bWavFile = FALSE;
	if( strstr( strFileName, ".wav" ) != NULL )
	{
		bWavFile = TRUE;
	}       
	// Load the file into a DirectMusic segment 
	if( FAILED( m_pMusicManager->CreateSegmentFromFile( &m_pMusicSegment, buf, 
		TRUE, bMidiFile ) ) )
	{
		// Not a critical failure, so just update the status
		return S_FALSE; 
	}



	return S_OK;
}
HRESULT CGameMusicManager::OnPlayAudio( HWND hDlg )
{
	HRESULT hr;

	if( m_pMusicSegment )
		m_pMusicSegment->Stop( 0 );

	if( S_FALSE == LoadSegmentFile( hDlg, "./Media/kmare.mid" ) )
	{
		// Not a critical failure, so just update the status

	}

	/*  if( bLooped )
	{
	// Set the segment to repeat many times
	if( FAILED( hr = m_pMusicSegment->SetRepeats( DMUS_SEG_REPEAT_INFINITE ) ) )
	return DXTRACE_ERR( TEXT("SetRepeats"), hr );
	}
	else
	{
	// Set the segment to not repeat
	if( FAILED( hr = m_pMusicSegment->SetRepeats( 0 ) ) )
	return DXTRACE_ERR( TEXT("SetRepeats"), hr );
	}*/

	// Play the segment and wait. The DMUS_SEGF_BEAT indicates to play on the 
	// next beat if there is a segment currently playing. 
	if( FAILED( hr = m_pMusicSegment->Play( DMUS_SEGF_BEAT ) ) )
		return 0;

	return S_OK;
}
