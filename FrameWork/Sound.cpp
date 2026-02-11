#include "Include.h"

Sound sound;



Sound::Sound(void)
{
	//g_pSoundManager = NULL; 사용 안했다
}

Sound::~Sound(void)
{
}

void Sound::Init()
{
	g_pSoundManager->Create( &m_Bk1, (LPWSTR)L"./resource/Sound/BGM_0001.wav", 0, GUID_NULL ) ;
	g_pSoundManager->Create( &m_Shot, (LPWSTR)L"./resource/Sound/shot.wav", 0, GUID_NULL ) ;
}

