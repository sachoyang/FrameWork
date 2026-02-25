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

    if (hp <= 0) 
    {
        hp = 0;
        isDead = true;
        aniCount = 0; // 시체 프레임 시작
        gravity = -5.0f; // 죽을 때도 살짝 위로 튀며 사망
    }
    else 
    {
        // 🌟 타격감 2번: 통통 튀어오르는 넉백!
        velocity.x = hitDir * 5.0f; // 맞은 방향으로 밀려남
        gravity = -7.0f;            // 위로 튀어오름
    }
}

void Enemy::Draw()
{
    D3DCOLOR color = 0xFFFFFFFF;
    if (isHit && !isDead) {
        if ((GetTickCount() - hitStartTime) < 200) color = D3DCOLOR_ARGB(255, 255, 50, 50);
        else isHit = false;
    }

    float renderX = pos.x - CAM->GetX();
    float renderY = pos.y - CAM->GetY();

    if (isDead) 
    {
        // 🌟 [핵심 수정] 살아있을 때와 죽었을 때의 이미지 높이 차이를 계산해서,
        // 시체의 발바닥(바닥)이 살아있을 때의 발바닥 위치와 정확히 일치하도록 아래로 내려줍니다!
        float liveHeight = img[0].imagesinfo.Height;
        float deadHeight = deadImg[aniCount].imagesinfo.Height;
        float yOffset = (liveHeight - deadHeight) / 2.0f; // 높이 차이의 절반만큼 내림

        deadImg[aniCount].SetColor((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF, 255);
        deadImg[aniCount].Render(renderX, renderY + yOffset, 0, dir, 1, 1);
    }
    else 
    {
        img[aniCount].SetColor((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF, 255);
        img[aniCount].Render(renderX, renderY, 0, dir, 1, 1);
    }

    // 디버그 박스
    if (Gmanager.m_GameStart == true) 
    {
         coll.BoxSow(m_rc, 0, 0, D3DCOLOR_ARGB(255, 255, 0, 255)); 
    }
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

    // 허공 정점(gravity=0)에서 얼지 않도록, '바닥에 닿아 속도가 0.0f가 된 순간'만 동결!
    bool isRestingCorpse = (isDead && velocity.x == 0.0f);

    if (!isRestingCorpse) {
        pos.y += gravity;
        gravity += 0.5f; if (gravity > 10.0f) gravity = 10.0f;

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

    bool hitWall = false, hitFloor = false;
    RECT temp;

    for (auto& w : coll.m_Walls) {
        if (!isDead && IntersectRect(&temp, &nextRc, &w)) hitWall = true;
        if (!isDead && IntersectRect(&temp, &cliffRc, &w)) hitFloor = true;

        if (IntersectRect(&temp, &m_rc, &w)) {
            if (gravity >= 0 && (m_rc.bottom - 20) <= w.top) {
                pos.y = w.top - 40.0f;
                gravity = 0;
                if (isDead) velocity.x = 0.0f; // 바닥에 닿는 순간 속도 0 -> 다음 프레임부터 완벽 동결!
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

    bool isRestingCorpse = (isDead && velocity.x == 0.0f);

    if (isDead || isHit) {
        if (!isRestingCorpse) {
            pos.y += gravity; gravity += 0.5f;
            pos.x += velocity.x; velocity.x *= 0.9f;

            if (!isDead) {
                // 🌟 [핵심 수정] 넉백 당하는 동안 계속 흘러가는 시간에 맞춰 중심점(startPos)을 역산합니다!
                // 이렇게 하면 무적(isHit)이 끝났을 때 텔레포트하지 않고 현재 위치에서 궤도를 자연스럽게 이어갑니다.
                DWORD t = GetTickCount() - spawnTime;
                float speed = 0.0015f;
                startPos.x = pos.x - 250.0f * sin(t * speed);
                startPos.y = pos.y - 80.0f * sin(t * speed * 2.0f);
            }
        }

        RECT temp;
        for (auto& w : coll.m_Walls) {
            if (IntersectRect(&temp, &m_rc, &w)) {
                if (gravity >= 0 && (m_rc.bottom - 20) <= w.top) {
                    pos.y = w.top - 30.0f;
                    gravity = 0;
                    velocity.x = 0.0f; // 바닥 닿음 인증!
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