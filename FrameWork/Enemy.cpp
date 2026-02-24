#include "Include.h"

// ==========================================
// [부모] Enemy 공통 기능
// ==========================================
void Enemy::TakeDamage(int damage, int hitDir)
{
    if (isDead) return;

    hp -= damage;
    isHit = true;
    hitStartTime = GetTickCount();

    if (hp <= 0) {
        hp = 0;
        isDead = true;
        aniCount = 0; // 시체 프레임 시작
        gravity = -5.0f; // 죽을 때도 살짝 위로 튀며 사망
    }
    else {
        // 🌟 타격감 2번: 통통 튀어오르는 넉백!
        velocity.x = hitDir * 5.0f; // 맞은 방향으로 밀려남
        gravity = -7.0f;            // 위로 튀어오름
    }
}

void Enemy::Draw()
{
    // 피격 시 빨간색 깜빡임 효과 (무적/피격 리액션)
    D3DCOLOR color = 0xFFFFFFFF;
    if (isHit && !isDead) {
        if ((GetTickCount() - hitStartTime) < 200) color = D3DCOLOR_ARGB(255, 255, 50, 50); // 0.2초간 빨갛게
        else isHit = false;
    }

    float renderX = pos.x - CAM->GetX();
    float renderY = pos.y - CAM->GetY();

    if (isDead) {
        // 죽었을 땐 시체 이미지 출력
        deadImg[aniCount].SetColor((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF, 255);
        deadImg[aniCount].Render(renderX, renderY, 0, dir, 1, 1);
    }
    else {
        img[aniCount].SetColor((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF, 255);
        img[aniCount].Render(renderX, renderY, 0, dir, 1, 1);
    }

    // 디버그용 박스
    // coll.BoxSow(m_rc, 0, 0, D3DCOLOR_ARGB(255, 255, 0, 255));
}


// ==========================================
// [자식 1] 지상 몹 (Ground Enemy)
// ==========================================
void GroundEnemy::Init(float x, float y)
{
    pos.x = x; pos.y = y; hp = 3;
    char FileName[256];
    // 💡 이미지 이름은 임시입니다. (나중에 수정하세요!)
    for (int i = 0; i < 3; i++) { sprintf_s(FileName, "./resource/Img/mob/ground%02d.png", i + 1); img[i].Create(FileName, false, 0); }
    for (int i = 0; i < 2; i++) { sprintf_s(FileName, "./resource/Img/mob/ground_dead%02d.png", i + 1); deadImg[i].Create(FileName, false, 0); }
    SetRect(&m_rc, pos.x - 30, pos.y - 40, pos.x + 30, pos.y + 40);
}

void GroundEnemy::Update()
{
    if (GetTickCount() - aniTime > 150) {
        aniCount++;
        if (isDead) { if (aniCount > 1) aniCount = 1; } // 시체는 마지막 프레임 유지
        else { if (aniCount > 2) aniCount = 0; }
        aniTime = GetTickCount();
    }

    // 중력 및 넉백 적용
    pos.y += gravity;
    gravity += 0.5f; if (gravity > 10.0f) gravity = 10.0f;

    if (isHit || isDead) {
        pos.x += velocity.x;
        velocity.x *= 0.9f; // 마찰력으로 점점 느려짐
    }
    else {
        // 평상시 걷기
        pos.x += (dir == -1) ? speed : -speed;

        // 🌟 벽 및 낭떠러지 감지 센서!
        RECT nextRc = m_rc;
        nextRc.left += (dir == -1) ? 5 : -5; nextRc.right += (dir == -1) ? 5 : -5;

        RECT cliffRc = nextRc; // 낭떠러지 센서는 발 밑을 검사
        cliffRc.top += 80; cliffRc.bottom += 80;

        bool hitWall = false;
        bool hitFloor = false;
        RECT temp;
        for (auto& w : coll.m_Walls) {
            if (IntersectRect(&temp, &nextRc, &w)) hitWall = true;  // 벽에 박음
            if (IntersectRect(&temp, &m_rc, &w)) {                  // 바닥 착지
                pos.y = w.top - 40.0f; gravity = 0;
            }
            if (IntersectRect(&temp, &cliffRc, &w)) hitFloor = true; // 앞에 바닥이 있음
        }

        // 벽에 막히거나, 앞에 낭떠러지(바닥 없음)면 뒤돌기!
        if (hitWall || (!hitFloor && gravity == 0)) {
            dir *= -1;
        }
    }

    SetRect(&m_rc, pos.x - 30, pos.y - 40, pos.x + 30, pos.y + 40);
}

// ==========================================
// [자식 2] 비행 몹 (Fly Enemy)
// ==========================================
void FlyEnemy::Init(float x, float y)
{
    pos.x = x; pos.y = y; startPos = pos; hp = 3;
    spawnTime = GetTickCount();
    char FileName[256];
    for (int i = 0; i < 4; i++) { sprintf_s(FileName, "./resource/Img/mob/fly%02d.png", i + 1); img[i].Create(FileName, false, 0); }
    for (int i = 0; i < 2; i++) { sprintf_s(FileName, "./resource/Img/mob/fly_dead%02d.png", i + 1); deadImg[i].Create(FileName, false, 0); }
}

void FlyEnemy::Update()
{
    if (GetTickCount() - aniTime > 100) {
        aniCount++;
        if (isDead) { if (aniCount > 1) aniCount = 1; }
        else { if (aniCount > 3) aniCount = 0; }
        aniTime = GetTickCount();
    }

    if (isDead || isHit) { // 피격 및 추락
        pos.y += gravity; gravity += 0.5f;
        pos.x += velocity.x; velocity.x *= 0.9f;

        // 바닥에 닿으면 멈춤
        RECT temp;
        for (auto& w : coll.m_Walls) {
            if (IntersectRect(&temp, &m_rc, &w)) { pos.y = w.top - 30; gravity = 0; velocity.x = 0; }
        }
    }
    else {
        // 🌟 수학의 마법: 부드러운 ∞ (무한대 8자) 비행 궤도
        DWORD t = GetTickCount() - spawnTime;
        float speed = 0.0015f;

        float nextX = startPos.x + 250.0f * sin(t * speed);          // 좌우 폭 250
        float nextY = startPos.y + 80.0f * sin(t * speed * 2.0f);    // 상하 폭 80 (두 배 빠르게 진동하여 8자 형태가 됨)

        // 방향 바라보기
        if (nextX > pos.x) dir = -1; else dir = 1;

        pos.x = nextX;
        pos.y = nextY;
    }

    SetRect(&m_rc, pos.x - 30, pos.y - 30, pos.x + 30, pos.y + 30);
}