#pragma once
#include "Include.h"

// 부모 클래스 (모든 적들의 공통 기능)
class Enemy
{
public:
    int type;           // 1: 지상, 2: 비행
    D3DXVECTOR2 pos;    // 현재 위치
    RECT m_rc;          // 피격 박스 (몸체)
    int hp;             // 체력
    int dir;            // 바라보는 방향 (1: 왼쪽, -1: 오른쪽)
    bool isDead;        // 죽었는가?

    // 피격 및 넉백 관련
    bool isHit;
    DWORD hitStartTime;
    D3DXVECTOR2 velocity; // 넉백 속도
    float gravity;

    // 애니메이션
    Sprite img[5];      // 이동 애니메이션 (넉넉히 5개)
    Sprite deadImg[2];  // 시체 애니메이션
    int aniCount;
    DWORD aniTime;

    Enemy() : hp(3), isDead(false), isHit(false), dir(1), gravity(0), type(1), aniCount(0), aniTime(0) {}    virtual ~Enemy() {}

    virtual void Init(float x, float y) = 0; // 자식들이 각자 구현할 초기화
    virtual void Update() = 0;               // 각자의 행동 패턴

    // 공통 렌더링 및 데미지 함수
    virtual void Draw();
    virtual void TakeDamage(int damage, int hitDir);

    virtual bool CanDealDamage() { return true; }
    virtual bool IsTargetable() { return true; }
};

// 지상 몹 (1번)
class GroundEnemy : public Enemy
{
public:
    float speed;
    GroundEnemy() { type = 1; speed = 2.0f; }
    void Init(float x, float y) override;
    void Update() override;
};

// 비행 몹 (2번)
class FlyEnemy : public Enemy
{
public:
    D3DXVECTOR2 startPos; // 기준점 (8자 비행의 중심)
    DWORD spawnTime;
    FlyEnemy() { type = 2; }
    void Init(float x, float y) override;
    void Update() override;
};