#include "Include.h"

Sound sound;



Sound::Sound()
{
	// ID 배열 초기화
	for (int i = 0; i < SND_END; i++)
		m_SoundIDs[i] = -1;
}

Sound::~Sound()
{
}

void Sound::Init()
{
	//g_pSoundManager->Create( &m_Bk1, (LPWSTR)L"./resource/Sound/BGM_0001.wav", 0, GUID_NULL ) ;
	//g_pSoundManager->Create( &m_Shot, (LPWSTR)L"./resource/Sound/shot.wav", 0, GUID_NULL ) ;

	// =========================================================
	// AddSoundFile("경로", 반복여부)
	// =========================================================

	// 1. 배경음 (반복 = true)
	m_SoundIDs[SND_BGM_STAGE1] = AddSoundFile("./resource/Sound/Stage_Theme.wav", true);
	m_SoundIDs[SND_BGM_BOSS] = AddSoundFile("./resource/Sound/watcher/Boss Battle 1.wav", true);

	// 2. 효과음 (반복 = false)
	m_SoundIDs[SND_EFF_JUMP] = AddSoundFile("./resource/Sound/Jump.wav", false);
	m_SoundIDs[SND_EFF_DASH] = AddSoundFile("./resource/Sound/Dash.wav", false);
	m_SoundIDs[SND_EFF_ATTACK] = AddSoundFile("./resource/Sound/Attack.wav", false);
	m_SoundIDs[SND_EFF_HIT] = AddSoundFile("./resource/Sound/Hit.wav", false);
	m_SoundIDs[SND_EFT_CRAWLER] = AddSoundFile("./resource/Sound/crawler.wav", true);
	m_SoundIDs[SND_EFT_FLYER] = AddSoundFile("./resource/Sound/mosquito_fly_loop.wav", true);
	m_SoundIDs[SND_EFF_MONSTER_DIE] = AddSoundFile("./resource/Sound/MonsterDie.wav", false);
	m_SoundIDs[SND_EFF_BOSS_ROAR] = AddSoundFile("./resource/Sound/BossRoar.wav", false);


}

void Sound::PlayBGM(SoundType type)
{
	// 유효한 범위인지, 로드가 제대로 됐는지 체크
	if (type < 0 || type >= SND_END || m_SoundIDs[type] == -1) return;

	// FmodSound의 전역 함수 호출
	BGPlay(m_SoundIDs[type]);
}

void Sound::PlayEffect(SoundType type)
{
	if (type < 0 || type >= SND_END || m_SoundIDs[type] == -1) return;

	// FmodSound의 전역 함수 호출
	EffectPlay(m_SoundIDs[type]);
}

void Sound::StopBGM()
{
	BGStop();
}

void Sound::VolUp()
{
	VolumUp();
}

void Sound::VolDown()
{
	VolumDown();
}
// 🌟 [추가] 구현부
void Sound::StopEffect(SoundType type)
{
	// 예외 처리
	if (type < 0 || type >= SND_END || m_SoundIDs[type] == -1) return;

	// FmodSound의 전역 함수 호출 (구현되어 있다고 가정)
	// 만약 EffectStop이라는 함수가 없다면, FMOD 내부의 Stop 기능을 연결해야 합니다.
	EffectStop(m_SoundIDs[type]);
}

// 사용법
//배경음 재생(MapManager::ChangeMap 등)
//
//C++
//// 1스테이지 BGM 틀기 (기존 BGM은 알아서 꺼짐)
//SOUND->PlayBGM(SND_BGM_STAGE1);
//점프할 때(Player::Update)
//
//C++
//if (key.KeyDown(VK_SPACE))
//{
//	// 점프 로직...
//	SOUND->PlayEffect(SND_EFF_JUMP); // 점프 소리!
//}
//공격할 때
//
//C++
//SOUND->PlayEffect(SND_EFF_ATTACK);
//보스방 입장 시
//
//C++
//SOUND->PlayBGM(SND_BGM_BOSS); // 보스 음악으로 전환
//SOUND->PlayEffect(SND_EFF_BOSS_ROAR); // 보스 포효 효과음