#include "Include.h"

BossEnemy::BossEnemy(int id)
{
    type = 3; // 보스 전용 타입
    bossID = id;
    hp = 15;
    state = B_STATE_SLEEP; // 처음엔 무조건 잠들어 있음
}

BossEnemy::~BossEnemy() {}

void BossEnemy::Init(float x, float y)
{
    // 1. 기본 위치 및 스펙
    pos.x = x;
    pos.y = y;
    hp = 15;
    dir = 1; // 왼쪽을 바라봄

    // =======================================================
    // 우주로 텔레포트 방지! (모든 변수 강제 0 초기화)
    // =======================================================
    velocity.x = 0.0f;
    velocity.y = 0.0f;
    gravity = 0.0f;
    isDead = false;
    isHit = false;
    aniCount = 0;

    char FileName[256];

    // 1. 수면 이미지 로드 (보스 ID에 따라 sleep01, 02, 03 배정)
    sprintf_s(FileName, "./resource/Img/boss/boss_sleep%02d.png", bossID);
    sleepImg.Create(FileName, false, 0);

    // 2. 걷기 이미지 로드 (walk01 ~ walk06)
    for (int i = 0; i < 6; i++) {
        sprintf_s(FileName, "./resource/Img/boss/boss_walk%02d.png", i + 1);
        walkImg[i].Create(FileName, false, 0);
    }

    // 3. 포효 이미지 로드 (roar01 ~ roar04)
    for (int i = 0; i < 4; i++) {
        sprintf_s(FileName, "./resource/Img/boss/boss_roar%02d.png", i + 1);
        roarImg[i].Create(FileName, false, 0);
    }

    // 3. 3배 크기 히트박스 갱신
    SetRect(&m_rc, pos.x - 100, pos.y - 130, pos.x + 100, pos.y + 130);
}
void BossEnemy::TakeDamage(int damage, int hitDir)
{
    if (isDead) return;
    if (state == B_STATE_SLEEP) return; // 자고 있을 땐 데미지를 입지 않음 (무적)

    hp -= damage;
    isHit = true;
    hitStartTime = GetTickCount();

    // 🌟 [핵심] 보스는 슈퍼아머! 일반 몹처럼 velocity.x나 gravity를 건드리지 않아서 넉백되지 않습니다.

    if (hp <= 0) {
        hp = 0;
        isDead = true;
        // 사망 시 처리 (추후 2페이즈 호출 등)
    }
}

void BossEnemy::ChangeState(int newState)
{
    state = newState;
    stateStartTime = GetTickCount();
    aniCount = 0;
    aniTime = GetTickCount(); // 애니메이션 타이머 초기화
}

bool BossEnemy::CanDealDamage()
{
    if (state == B_STATE_SLEEP) {
        return false; // 자고 있으면 타격 판정 없음!
    }
    return true;
}

void BossEnemy::Update()
{
    // 1. 거대한 몸집에 맞는 무거운 중력 적용
    pos.y += gravity;
    gravity += 0.8f;
    if (gravity > 15.0f) gravity = 15.0f;

    // 갱신
    SetRect(&m_rc, pos.x - 90, pos.y - 120, pos.x + 90, pos.y + 120);
    // 2. 바닥 충돌 (미끄러짐 및 지형 착지)
    pos.x += velocity.x;
    velocity.x *= 0.9f;

    RECT temp;
    for (auto& w : coll.m_Walls) {
        if (IntersectRect(&temp, &m_rc, &w)) {
            // 발바닥 착지 (히트박스가 크기 때문에 120px 오프셋 필요)
            if (gravity >= 0 && (m_rc.bottom - 40) <= w.top) {
                pos.y = w.top - 120.0f;
                gravity = 0;

                SetRect(&m_rc, pos.x - 90, pos.y - 120, pos.x + 90, pos.y + 120);
            }
        }
    }

    // 2. 애니메이션 프레임 제어
    DWORD elapsed = GetTickCount() - stateStartTime;

    if (state == B_STATE_AWAKE_ROAR) {
        // 🌟 2초(2000ms) 동안 4프레임 분배 (0.5초마다 변경)
        if (elapsed < 500) aniCount = 0;       // 준비 동작 1
        else if (elapsed < 1000) aniCount = 1; // 준비 동작 2
        else if (elapsed < 1500) aniCount = 2; // 포효 1
        else aniCount = 3;                     // 포효 2
    }
    else if (state == B_STATE_WALK || state == B_STATE_IDLE) {
        // 🌟 걷기 애니메이션 재생 (0.15초마다 프레임 넘김)
        if (GetTickCount() - aniTime > 150) {
            aniCount++;
            if (aniCount > 5) aniCount = 0; // 6프레임 반복
            aniTime = GetTickCount();
        }
    }

    // 히트박스 갱신
    SetRect(&m_rc, pos.x - 90, pos.y - 120, pos.x + 90, pos.y + 120);

    // 3. 뼈대만 있는 상태 머신 (Step 3에서 채울 예정)
    switch (state)
    {
    case B_STATE_SLEEP:
        break;
    case  B_STATE_AWAKE_ROAR:
        //2초간 포효 애니메이션 (aniCount 0~5)
        if (GetTickCount() - stateStartTime > 2000)
        {
			ChangeState(B_STATE_WALK);
			knight.isCutscene = false; // 포효 끝나면 컷씬 해제 (기사 행동 가능)
        }
        break;
	case B_STATE_IDLE:
        // 패턴
        break;
    }
}

void BossEnemy::Draw()
{
    // 오렌지색 피격 깜빡임 연출
    D3DCOLOR color = 0xFFFFFFFF;

    // 1. 수면 상태: 배경에 묻혀있는 느낌을 주기 위해 아주 어두운 회색으로 렌더링합니다!
    if (state == B_STATE_SLEEP) 
    {
        color = D3DCOLOR_ARGB(255, 60, 60, 60);
    }
    else if (isHit && !isDead)
    {
        if ((GetTickCount() - hitStartTime) < 200) color = D3DCOLOR_ARGB(255, 255, 120, 0); // 주황색
        else isHit = false;
    }

    float renderX = pos.x - CAM->GetX();
    float renderY = pos.y - CAM->GetY();

    // 3배 거대화 렌더링!
    float scale = 1.0f;

    // 상태에 따라 출력할 이미지를 다르게 선택!
    Sprite* currentImg = &sleepImg; // 기본값

    if (state == B_STATE_SLEEP) currentImg = &sleepImg;
    else if (state == B_STATE_AWAKE_ROAR) currentImg = &roarImg[aniCount];
    else if (state == B_STATE_WALK || state == B_STATE_IDLE) currentImg = &walkImg[aniCount];

    currentImg->SetColor((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF, 255);
    currentImg->Render(renderX, renderY, 0, dir, scale, scale);

    if (Gmanager.m_GameStart && coll.isDebugDraw) {
        coll.BoxSow(m_rc, 0, 0, D3DCOLOR_ARGB(255, 255, 0, 0));
    }
}