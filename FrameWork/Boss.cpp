#include "Include.h"

BossEnemy::BossEnemy(int id)
{
    type = 3; // 보스 전용 타입
    bossID = id;
    hp = 20;
    state = B_STATE_SLEEP; // 처음엔 무조건 잠들어 있음
}

BossEnemy::~BossEnemy() {}

void BossEnemy::Init(float x, float y)
{
    // 1. 기본 위치 및 스펙
    pos.x = x;
    pos.y = y;
    hp = 20;
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
    
    // Swing (대못 공격 11장) 로드
    for (int i = 0; i < 11; i++) {
        sprintf_s(FileName, "./resource/Img/boss/swing%02d.png", i + 1);
        swingImg[i].Create(FileName, false, 0);
    }
    // Roll (구르기 7장) 로드
    for (int i = 0; i < 7; i++) {
        sprintf_s(FileName, "./resource/Img/boss/roll%02d.png", i + 1);
        rollImg[i].Create(FileName, false, 0);
    }
    // 히트박스 갱신
    BossSetRect();
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
bool BossEnemy::IsTargetable()
{
    if (state == B_STATE_SLEEP) {
        return false; // 자고 있으면 타격 판정 아예 없음!
    }
    return true;
}
void BossEnemy::BossSetRect()
{
    SetRect(&m_rc, pos.x - 100, pos.y - 120, pos.x + 150, pos.y + 130);
}

void BossEnemy::Update()
{
    // 1. 거대한 몸집에 맞는 무거운 중력 적용
    pos.y += gravity;
    gravity += 0.8f;
    if (gravity > 15.0f) gravity = 15.0f;

    // 갱신
    BossSetRect();
    // 2. 바닥 충돌 (미끄러짐 및 지형 착지)
    pos.x += velocity.x;
    velocity.x *= 0.9f;

    RECT temp;
    for (auto& w : coll.m_Walls) {
        if (IntersectRect(&temp, &m_rc, &w)) {
            // 발바닥 착지 (히트박스가 크기 때문에 120px 오프셋 필요)
            if (gravity >= 0 && (m_rc.bottom - 30.0f) <= w.top) {
                pos.y = w.top - 130.0f;
                gravity = 0;

                BossSetRect();
            }
        }
    }

    // 2. 애니메이션 프레임 제어
    DWORD elapsed = GetTickCount() - stateStartTime;
    float distToKnight = abs(knight.pos.x - pos.x);
    // 기사가 왼쪽에 있으면 1, 오른쪽에 있으면 -1 (보스가 바라보는 방향)
    int lookDir = (knight.pos.x < pos.x) ? 1 : -1;
    // 공격 박스 초기화 (공격 중일 때만 세팅됨)
    isAttacking = false;
    SetRect(&attackBox, 0, 0, 0, 0);

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
    BossSetRect();

    // 3. 뼈대만 있는 상태 머신 (Step 3에서 채울 예정)
    switch (state)
    {
    case B_STATE_SLEEP:
        break;

    case B_STATE_AWAKE_ROAR:
        if (elapsed < 500) aniCount = 0;
        else if (elapsed < 1000) aniCount = 1;
        else if (elapsed < 1500) aniCount = 2;
        else aniCount = 3;

        if (elapsed > 2000) {
            ChangeState(B_STATE_IDLE);
            knight.isCutscene = false;
        }
        break;

    case B_STATE_IDLE:
        // 방향 전환
        dir = lookDir;

        if (elapsed > 500) // 0.5초 대기 후 행동 결정
        {
            if (distToKnight <= 300.0f) {
                // 🌟 [수정] 근접 시 확률: 60% 가로/세로베기, 20% 구르기 돌진, 20% 뒤로 구르기
                int randPattern = rand() % 100;

                if (randPattern < 60) ChangeState(B_STATE_MELEE);
                else if (randPattern < 80) ChangeState(B_STATE_ROLL_DASH); // 근접에서도 냅다 구르기!
                else ChangeState(B_STATE_ROLL_BACK);
            }
            else {
                // 멀다! -> 걷기(접근) 또는 구르기 돌진
                if (rand() % 100 < 40) ChangeState(B_STATE_ROLL_DASH);
                else ChangeState(B_STATE_WALK);
            }
        }
        break;

    case B_STATE_WALK:
        dir = lookDir;
        // 느리게 걸어감
        velocity.x = (dir == 1) ? -1.5f : 1.5f;

        // 걷기 애니메이션 (0.15초)
        if (GetTickCount() - aniTime > 150) {
            aniCount = (aniCount + 1) % 6;
            aniTime = GetTickCount();
        }

        // 걷다가 가까워지면 즉시 공격!
        if (distToKnight <= 250.0f) ChangeState(B_STATE_MELEE);
        // 너무 오래 걸으면(2초) 다시 생각함
        else if (elapsed > 2000) ChangeState(B_STATE_IDLE);
        break;

    case B_STATE_MELEE:
        // 🌟 근접 2연타 (Swing01 ~ 11)
        // 0~5: 가로 베기 / 6~10: 세로 베기
        if (GetTickCount() - aniTime > 100) { // 0.1초마다 프레임 넘김
            aniCount++;
            aniTime = GetTickCount();

            // 전진 이동 기믹 (특정 타격 프레임에서 앞으로 쿵! 이동)
            if (aniCount == 3 || aniCount == 8) {
                velocity.x = (dir == 1) ? -15.0f : 15.0f;
            }
        }

        //// 타격 히트박스(attackBox) 활성화 구간 (이미지 크기에 맞게 거대하게!)
        //if ((aniCount >= 2 && aniCount <= 4) || (aniCount >= 7 && aniCount <= 9)) {
        //    isAttacking = true;
        //    if (dir == 1) { // 왼쪽 보고 칠 때
        //        SetRect(&attackBox, pos.x - 250, pos.y - 150, pos.x + 50, pos.y + 120);
        //    }
        //    else { // 오른쪽 보고 칠 때
        //        SetRect(&attackBox, pos.x - 50, pos.y - 150, pos.x + 250, pos.y + 120);
        //    }
        //}

        // 1. 가로 베기 구간 (옆으로 쫙 길게! 위아래는 얇게)
        if (aniCount >= 2 && aniCount <= 4) {
            isAttacking = true;
            if (dir == 1) { // 왼쪽 보고 칠 때 (X를 왼쪽으로 길게 -300)
                SetRect(&attackBox, pos.x - 300, pos.y - 50, pos.x + 50, pos.y + 50);
            }
            else {        // 오른쪽 보고 칠 때 (X를 오른쪽으로 길게 +300)
                SetRect(&attackBox, pos.x - 50, pos.y - 50, pos.x + 300, pos.y + 50);
            }
        }
        // 2. 세로 베기 구간 (위로 아주 높게! X 폭은 적당히)
        else if (aniCount >= 7 && aniCount <= 9) {
            isAttacking = true;
            if (dir == 1) { // 왼쪽 (Y를 위로 엄청 높게 -250)
                SetRect(&attackBox, pos.x - 200, pos.y - 230, pos.x + 50, pos.y + 120);
            }
            else {        // 오른쪽
                SetRect(&attackBox, pos.x - 50, pos.y - 230, pos.x + 200, pos.y + 120);
            }
        }

        if (aniCount > 10) ChangeState(B_STATE_IDLE); // 공격 끝나면 대기
        break;

    case B_STATE_ROLL_DASH:
        // 🌟 지상 구르기 돌진 (Roll01 ~ Roll07)
        if (elapsed < 400) {
            // 준비 동작 (살짝 뒤로 물러남)
            aniCount = 0;
            velocity.x = (dir == 1) ? 2.0f : -2.0f; // 반대 방향으로 스멀스멀
        }
        else {
            // 본격적인 돌진 (무한 궤도 1~4 프레임 반복)
            velocity.x = (dir == 1) ? -15.0f : 15.0f; // 엄청난 속도로 돌진!

            if (GetTickCount() - aniTime > 80) {
                aniCount++;
                if (aniCount > 4 || aniCount < 1) aniCount = 1; // 1,2,3,4 무한 반복
                aniTime = GetTickCount();
            }

            // 벽에 부딪혔는가?
            bool hitWall = false;
            if (dir == 1 && pos.x <= 150.0f) hitWall = true; // 왼쪽 벽
            if (dir == -1 && pos.x >= SCREEN_WITH * 2 - 150.0f) hitWall = true; // 오른쪽 벽 (보스방 기준)

            // 기사에게 맞췄는가? (MapManager에서 보스가 기사 때릴 때 state를 강제로 IDLE로 바꾸게 처리할 수도 있지만, 우선 거리로 판별)
            //if (distToKnight < 100.0f) hitWall = true; // 기사와 쾅 부딪힘

            if (hitWall) {
                // 튕겨 나가면서 멈춤 애니메이션(5, 6) 재생 상태로 전환
                velocity.x = (dir == 1) ? 10.0f : -10.0f; // 반대로 튕김
                ChangeState(B_STATE_IDLE); // 일단 대기 상태로 빼서 멈춤 애니메이션은 나중에 다듬기
            }
        }
        break;

    case B_STATE_ROLL_BACK: // 후퇴 (임시)
        ChangeState(B_STATE_IDLE);
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

    // 상태에 맞춰 알맞은 이미지 배열을 가리킵니다.
    if (state == B_STATE_SLEEP) currentImg = &sleepImg;
    else if (state == B_STATE_AWAKE_ROAR) currentImg = &roarImg[aniCount];
    else if (state == B_STATE_WALK) currentImg = &walkImg[aniCount];
    else if (state == B_STATE_IDLE) currentImg = &walkImg[0]; // 대기할 땐 걷기 1번 프레임
    else if (state == B_STATE_MELEE) currentImg = &swingImg[aniCount];
    else if (state == B_STATE_ROLL_DASH) currentImg = &rollImg[aniCount];
    
    currentImg->color = color;
    //currentImg->SetColor((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF, 255);
    currentImg->Render(renderX, renderY, 0, dir, scale, scale);

	//콜라이더 박스 그리기 (디버그용)
    if (Gmanager.m_GameStart && coll.isDebugDraw) {
        coll.BoxSow(m_rc, 0, 0, D3DCOLOR_ARGB(255, 255, 0, 0)); // 보스 몸통 (빨강)
        if (isAttacking) {
            coll.BoxSow(attackBox, 0, 0, D3DCOLOR_ARGB(255, 255, 255, 0)); // 보스 공격 범위 (노랑)
        }
    }
}