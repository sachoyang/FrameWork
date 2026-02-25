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
    dir = -1; // 왼쪽을 바라봄

    // =======================================================
    // 🌟 [핵심] 우주로 텔레포트 방지! (모든 변수 강제 0 초기화)
    // =======================================================
    velocity.x = 0.0f;
    velocity.y = 0.0f;
    gravity = 0.0f;
    isDead = false;
    isHit = false;
    aniCount = 0;

    // 2. 임시 이미지 할당 (파일 이름 ground01.png 확인 완료!)
    char FileName[256];
    for (int i = 0; i < 3; i++) {
        sprintf_s(FileName, "./resource/Img/monster/ground%02d.png", i + 1);
        img[i].Create(FileName, false, 0);
    }

    // 3. 3배 크기 히트박스 갱신
    SetRect(&m_rc, pos.x - 90, pos.y - 120, pos.x + 90, pos.y + 120);
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

    // 히트박스 갱신
    SetRect(&m_rc, pos.x - 90, pos.y - 120, pos.x + 90, pos.y + 120);

    // 3. 뼈대만 있는 상태 머신 (Step 3에서 채울 예정)
    switch (state)
    {
    case B_STATE_SLEEP:
        break;
    }
}

void BossEnemy::Draw()
{
    // 🌟 오렌지색 피격 깜빡임 연출
    D3DCOLOR color = 0xFFFFFFFF;
    if (isHit && !isDead) {
        if ((GetTickCount() - hitStartTime) < 200) color = D3DCOLOR_ARGB(255, 255, 120, 0); // 주황색
        else isHit = false;
    }

    float renderX = pos.x - CAM->GetX();
    float renderY = pos.y - CAM->GetY();

    // 🌟 3배 거대화 렌더링!
    float scale = 3.0f;

    img[0].SetColor((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF, 255);
    img[0].Render(renderX, renderY, 0, dir, scale, scale);

    // 보스는 전용으로 붉은색 히트박스 출력 (크기 확인용)
    if (Gmanager.m_GameStart) {
        char bossTxt[32] = "BOSS";
        // 머리(renderY - 150) 위치 쯤에 빨간색으로 글자 출력
        dv_font.DrawString(bossTxt, renderX - 20, renderY - 150, D3DCOLOR_ARGB(255, 255, 0, 0));
        coll.BoxSow(m_rc, 0, 0, D3DCOLOR_ARGB(255, 255, 0, 0));
    }
}