#pragma once

// 여기에 사용할 사운드 이름을 추가하세요.
enum SoundType {
	// --- 배경음 (BGM) ---
	SND_BGM_STAGE1,
	SND_BGM_BOSS,

	// --- 효과음 (SFX) ---
	SND_EFF_JUMP,
	SND_EFF_DASH,
	SND_EFF_ATTACK,
	SND_EFF_HIT,
	SND_EFF_MONSTER_DIE,
	SND_EFF_BOSS_ROAR,
	SND_EFF_BOSS_ATTACK,
	SND_EFF_BOSS_ROLL,
	// (항상 마지막에 둠)
	SND_END
};


class Sound
{

private:

	int m_SoundIDs[SND_END];
public:

	Sound();
	~Sound();
	// 싱글톤 인스턴스 반환
	static Sound* GetInstance() {
		static Sound instance;
		return &instance;
	}
	
//CSoundManager* g_pSoundManager;
//CSound*		m_Bk1;
//CSound*		m_Shot;

//int m_Select;
	void Init();

	// 재생 함수
	void PlayBGM(SoundType type);   // 배경음 재생 (Loop)
	void PlayEffect(SoundType type); // 효과음 재생 (Once)

	// 제어 함수
	void StopBGM();
	void VolUp();
	void VolDown();
	
};

//extern Sound sound;

#define SOUND Sound::GetInstance()