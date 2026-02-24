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
    coll.BoxSow(m_rc, 0, 0, D3DCOLOR_ARGB(255, 255, 0, 255));
}


// ==========================================
// [자식 1] 지상 몹 (Ground Enemy)
// ==========================================
void GroundEnemy::Init(float x, float y)
{
    pos.x = x; pos.y = y; hp = 3;
    char FileName[256];
    for (int i = 0; i < 3; i++) { sprintf_s(FileName, "./resource/Img/monster/ground%02d.png", i + 1); img[i].Create(FileName, false, 0); }
    for (int i = 0; i < 2; i++) { sprintf_s(FileName, "./resource/Img/monster/ground_dead%02d.png", i + 1); deadImg[i].Create(FileName, false, 0); }
    SetRect(&m_rc, pos.x - 30, pos.y - 40, pos.x + 30, pos.y + 40);
}

void GroundEnemy::Update()
{
    if (GetTickCount() - aniTime > 150) {
        aniCount++;
        if (isDead) { if (aniCount > 1) aniCount = 1; }
        else { if (aniCount > 2) aniCount = 0; }
        aniTime = GetTickCount();
    }

    // 시체가 땅에 안착했다면 물리 연산을 완전히 멈춤 (떨림 방지!)
    bool isRestingCorpse = (isDead && gravity == 0 && abs(velocity.x) < 0.5f);

    if (!isRestingCorpse) {
        // 중력 적용
        pos.y += gravity;
        gravity += 0.5f; if (gravity > 10.0f) gravity = 10.0f;

        // x축 이동
        if (isHit || isDead) {
            pos.x += velocity.x;
            velocity.x *= 0.9f;
        }
        else {
            pos.x += (dir == -1) ? speed : -speed;
        }
    }

    RECT nextRc = m_rc;
    nextRc.top += 10; nextRc.bottom -= 10;
    nextRc.left += (dir == -1) ? 5 : -5; nextRc.right += (dir == -1) ? 5 : -5;

    RECT cliffRc = m_rc;
    cliffRc.left += (dir == -1) ? 20 : -20; cliffRc.right += (dir == -1) ? 20 : -20;
    cliffRc.top = m_rc.bottom + 5; cliffRc.bottom = m_rc.bottom + 30;

    bool hitWall = false;
    bool hitFloor = false;
    RECT temp;

    for (auto& w : coll.m_Walls) {
        if (!isDead && IntersectRect(&temp, &nextRc, &w)) hitWall = true;
        if (!isDead && IntersectRect(&temp, &cliffRc, &w)) hitFloor = true;

        if (IntersectRect(&temp, &m_rc, &w)) {
            if (gravity >= 0 && (m_rc.bottom - 20) <= w.top) {
                pos.y = w.top - 40.0f;
                gravity = 0;
                if (isDead) velocity.x = 0; // 시체 미끄러짐 멈춤 (동결 시작)
            }
        }
    }

    if (!isHit && !isDead) {
        if (hitWall || (!hitFloor && gravity == 0)) dir *= -1;
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
    for (int i = 0; i < 4; i++) { sprintf_s(FileName, "./resource/Img/monster/fly%02d.png", i + 1); img[i].Create(FileName, false, 0); }
    for (int i = 0; i < 2; i++) { sprintf_s(FileName, "./resource/Img/monster/fly_dead%02d.png", i + 1); deadImg[i].Create(FileName, false, 0); }
}

void FlyEnemy::Update()
{
    if (GetTickCount() - aniTime > 100) {
        aniCount++;
        if (isDead) { if (aniCount > 1) aniCount = 1; }
        else { if (aniCount > 3) aniCount = 0; }
        aniTime = GetTickCount();
    }

    bool isRestingCorpse = (isDead && gravity == 0 && abs(velocity.x) < 0.5f);

    if (isDead || isHit) {
        // 🌟 [핵심 수정] 비행 몹 시체도 땅에 닿으면 동결
        if (!isRestingCorpse) {
            pos.y += gravity; gravity += 0.5f;
            pos.x += velocity.x; velocity.x *= 0.9f;

            if (!isDead) {
                startPos.x += velocity.x;
                startPos.y += gravity;
            }
        }

        RECT temp;
        for (auto& w : coll.m_Walls) {
            if (IntersectRect(&temp, &m_rc, &w)) {
                if (gravity >= 0 && (m_rc.bottom - 20) <= w.top) {
                    pos.y = w.top - 30.0f;
                    gravity = 0;
                    velocity.x = 0;
                }
            }
        }
    }
    else {
        DWORD t = GetTickCount() - spawnTime;
        float speed = 0.0015f;
        float nextX = startPos.x + 250.0f * sin(t * speed);
        float nextY = startPos.y + 80.0f * sin(t * speed * 2.0f);

        if (nextX > pos.x) dir = -1; else dir = 1;
        pos.x = nextX; pos.y = nextY;
    }

    SetRect(&m_rc, pos.x - 30, pos.y - 30, pos.x + 30, pos.y + 30);
}