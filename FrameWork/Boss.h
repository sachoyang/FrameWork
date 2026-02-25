#pragma once
#include "Include.h"

// 보스가 가질 수 있는 행동 상태(State) 목록
enum BOSS_STATE {
    B_STATE_SLEEP,          // 배경에서 잠들어 있음
    B_STATE_AWAKE_ROAR,     // 깨어나서 포효함
    B_STATE_IDLE,           // 대기 (기사 관찰)
    B_STATE_WALK,           // 기사를 향해 뚜벅뚜벅 걷기
    B_STATE_MELEE,          // 근접 2연타 (가로 -> 세로)
    B_STATE_ROLL_DASH,      // 지상 구르기 돌진
    B_STATE_ROLL_BOUNCE,    // 탱탱볼 점프 돌진
    B_STATE_ROLL_BACK       // 뒤로 구르기 (거리 벌리기)
};

class BossEnemy : public Enemy
{
public:
    int bossID; // 1번, 2번, 3번 보스 식별용 (누가 깨어날지 결정)
    int state;
    DWORD stateStartTime;

    // 추가 변수 (탱탱볼 바운스 카운트 등 추후 사용)
    int bounceCount;

    BossEnemy(int id);
    virtual ~BossEnemy();

    void Init(float x, float y) override;
    void Update() override;
    void Draw() override;
    void TakeDamage(int damage, int hitDir) override;

    // 상태를 안전하게 변경하는 헬퍼 함수
    void ChangeState(int newState);
};