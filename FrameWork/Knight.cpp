#include "Include.h"

Knight knight;


Knight::Knight()
{
    m_KnightAniTime = 0;
    m_KnightCount = 0;
    isMove = 0;
    isLookup = 0;
    isLookdown = 0;
    pos.x = 200;
    pos.y = 100;
    gravity = 5.75f;
    grounded = false;
    dir = -1;

	//대시 관련 초기화
    isDashing = false;
    canAirDash = true;
    dashStartTime = 0;
    dashCooldownTime = 0;
    dashSpeed = 25.0f; // 대시 속도 (걷기보다 훨씬 빠르게)
}

Knight::~Knight()
{
}

void Knight::Init()
{
    char FileName[256];

    for (int i = 0; i < 3; i++)
    {
        sprintf_s(FileName, "./resource/Img/knight1/walk%02d.png", i + 1);
        Knightimg[i].Create(FileName, false, D3DCOLOR_XRGB(0, 0, 0));
    }
    sprintf_s(FileName, "./resource/Img/knight1/stay01.png");
    Knightimg[3].Create(FileName, false, D3DCOLOR_XRGB(0, 0, 0));
    sprintf_s(FileName, "./resource/Img/knight1/Lookup01.png");
    Knightimg[4].Create(FileName, false, D3DCOLOR_XRGB(0, 0, 0));
    D3DXGetImageInfoFromFile(FileName, &imagesinfo);

    sprintf_s(FileName, "./resource/Img/knight1/Lookdown01.png");
    Knightimg[5].Create(FileName, false, D3DCOLOR_XRGB(0, 0, 0));

    // 6: 점프 준비, 7: 상승, 8: 꼭대기, 9: 하강, 10: 착지
    sprintf_s(FileName, "./resource/Img/knight1/jump01.png");
    Knightimg[6].Create(FileName, false, 0);

    sprintf_s(FileName, "./resource/Img/knight1/jump02.png");
    Knightimg[7].Create(FileName, false, 0);

    sprintf_s(FileName, "./resource/Img/knight1/jump03.png");
    Knightimg[8].Create(FileName, false, 0);

    sprintf_s(FileName, "./resource/Img/knight1/fall01.png");
    Knightimg[9].Create(FileName, false, 0);

    sprintf_s(FileName, "./resource/Img/knight1/fall02.png");
    Knightimg[10].Create(FileName, false, 0);

    sprintf_s(FileName, "./resource/Img/knight1/dash01.png"); // 준비
    Knightimg[11].Create(FileName, false, 0);

    sprintf_s(FileName, "./resource/Img/knight1/dash02.png"); // 대시 중
    Knightimg[12].Create(FileName, false, 0);

    sprintf_s(FileName, "./resource/Img/knight1/dash03.png"); // 공중 느낌
    Knightimg[13].Create(FileName, false, 0);

    sprintf_s(FileName, "./resource/Img/knight1/dash04.png"); // 정지
    Knightimg[14].Create(FileName, false, 0);

    sprintf_s(FileName, "./resource/Img/knight1/attack01.png"); // 공격 준비
    Knightimg[15].Create(FileName, false, 0);

    sprintf_s(FileName, "./resource/Img/knight1/attack02.png"); // 칼 휘두름 (히트박스 발생)
    Knightimg[16].Create(FileName, false, 0);

    sprintf_s(FileName, "./resource/Img/knight1/attack03.png"); // 공격 마무리
    Knightimg[17].Create(FileName, false, 0);

    sprintf_s(FileName, "./resource/Img/knight1/attackup01.png"); Knightimg[18].Create(FileName, false, 0);
    sprintf_s(FileName, "./resource/Img/knight1/attackup02.png"); Knightimg[19].Create(FileName, false, 0);
    sprintf_s(FileName, "./resource/Img/knight1/attackup03.png"); Knightimg[20].Create(FileName, false, 0);

    sprintf_s(FileName, "./resource/Img/knight1/attackdown01.png"); Knightimg[21].Create(FileName, false, 0);
    sprintf_s(FileName, "./resource/Img/knight1/attackdown02.png"); Knightimg[22].Create(FileName, false, 0);
    sprintf_s(FileName, "./resource/Img/knight1/attackdown03.png"); Knightimg[23].Create(FileName, false, 0);

    sprintf_s(FileName, "./resource/Img/effect/hit.png");
    hitEffect[0].Create(FileName, false, 0);

    sprintf_s(FileName, "./resource/Img/effect/unhit.png");
    unhitEffect[0].Create(FileName, false, 0);

    sprintf_s(FileName, "./resource/Img/effect/hitdown.png");
    hitEffect[1].Create(FileName, false, 0);

    sprintf_s(FileName, "./resource/Img/effect/unhitdown.png");
    unhitEffect[1].Create(FileName, false, 0);

    hp = 8;
	isInvincible = false;
	isKnockback = false;
    isAttackHit = false;
    isAttacking = false;
    attackStartTime = 0;
    lastAttackTime = 0; // 쿨타임 초기화
    SetRect(&attackBox, 0, 0, 0, 0);

    KnightMove();
}


// Knight.cpp

void Knight::Update()
{
    if (GetTickCount() - m_KnightAniTime > 10)
    {
        // 무적 시간 1.5초(1500ms) 해제 로직
        if (isInvincible && (GetTickCount() - invincibleTime > 1500)) {
            isInvincible = false;
        }
        // ========================================================
        // 0단계: 공격 상태 및 히트박스(AttackBox) 업데이트
        // ========================================================
        if (isAttacking)
        {
            DWORD attackTime = GetTickCount() - attackStartTime;

            // 공격 지속 시간 (예: 300ms = 0.3초 동안 공격)
            if (attackTime > 300)
            {
                isAttacking = false;
                SetRect(&attackBox, 0, 0, 0, 0); // 공격이 끝나면 히트박스 소멸
            }
            else
            {
                // [히트박스 위치 계산]
                if (attackType == 0) // 측면 공격
                {
                    if (dir == -1) { // 오른쪽
                        attackBox.left = pos.x + 10; attackBox.right = pos.x + 160;
                    }
                    else {         // 왼쪽 (dir == 1)
                        attackBox.left = pos.x - 160; attackBox.right = pos.x - 10;
                    }
                    attackBox.top = pos.y - 70; attackBox.bottom = pos.y + 30;
                }
                else if (attackType == 1) // 상단 공격
                {
                    attackBox.left = pos.x - 60; attackBox.right = pos.x + 60;
                    attackBox.top = pos.y - 160; attackBox.bottom = pos.y - 10; // 머리 위로 길게
                }
                else if (attackType == 2) // 하단 공격
                {
                    attackBox.left = pos.x - 60; attackBox.right = pos.x + 60;
                    attackBox.top = pos.y + 10; attackBox.bottom = pos.y + 150; // 발 밑으로 길게
                }

                // [포고 점프] 하단 찍기 중에 무언가(적/오브젝트)를 맞췄다면?
                if (attackType == 2 && isAttackHit)
                {
                    gravity = -13.0f; // 공중에서 위로 강하게 튕겨오름! (체공 시간 벌기)
                    //isAttackHit = false; // 연달아 무한 점프하지 않도록 판정 1회 소모
                }
            }
        }
        else
        {
            SetRect(&attackBox, 0, 0, 0, 0);
        }

        // ========================================================
        // 1단계: 수평(X) 이동 및 충돌 처리 (걷기 + 대시 + 넉백)
        // ========================================================
        float moveX = 0.0f;
		float knockMoveX = 0.0f; //넉백 이동량

        // [대시 중]
        if (isDashing)
        {
            if (GetTickCount() - dashStartTime > 200) // 대시 지속시간 끝
            {
                isDashing = false;
                gravity = 0;
            }
            else
            {
                moveX = -(dir * dashSpeed); // 대시 이동량 계산
            }
        }

        // [넉백 중 이동량 계산]
        if (isKnockback) {
            knockMoveX = knockVelocity.x;
            knockVelocity.x *= 0.85f;
            if (abs(knockVelocity.x) < 0.5f) { knockVelocity.x = 0; isKnockback = false; }
        }
        // [걷기 상태] (Key.cpp에서 이미 pos.x를 건드려서 왔으므로 변화량을 추적해야 함)
        // 하지만 지금 구조상 Key.cpp가 pos.x를 직접 바꾸므로, 
        // 여기서 "이동 후 위치"가 벽이라면 "이동 전"으로 되돌리는 방식을 씁니다.

        // 1-1. 일단 대시 이동 적용 (걷기는 Key.cpp에서 이미 적용됨)
        if (isDashing) pos.x += moveX;
		if (isKnockback) pos.x += knockMoveX; // 넉백 이동 적용
        // 1-2. 현재 위치로 박스 갱신
        KnightMove();

        // 1-3. [수평 충돌 검사] (바닥에 걸리는 것 방지 위해 위아래를 줄임!)
        RECT wallCheckRect = m_rc;
        wallCheckRect.top += 10;    // 머리 쪽 여유 (천장에 머리 박고 걸을 때 걸림 방지)
        wallCheckRect.bottom -= 10; // 발 쪽 여유 (바닥에 1px 박혀있을 때 걸림 방지)

        bool isHitWall = false;
        RECT tempRect;

        for (auto& wall : coll.m_Walls)
        {
            if (IntersectRect(&tempRect, &wallCheckRect, &wall))
            {
                isHitWall = true;
                break;
            }
        }

        // 1-4. 벽에 부딪혔다면? -> 튕겨내기!
        if (isHitWall)
        {
            if (isDashing)
            {
                // 대시 중 벽 충돌 -> 대시 종료 및 원상복구
                isDashing = false;
                pos.x -= moveX;
            }
            if (isKnockback) { // 넉백 중에 벽을 만났다면?
                pos.x -= knockMoveX; // 파고든 만큼 다시 빼냅니다!
                knockVelocity.x = 0; // 벽에 박았으니 밀려나는 힘 소멸
            }
            if(isMove)
            {
                // 걷기 중 벽 충돌 -> Key.cpp가 옮긴 걸 취소해야 함
                // Key.cpp에서 3.0f만큼 움직였다고 가정하고 반대로 밈
                // (정확히 하려면 prevX를 저장해야 하지만, 간단히 반대 방향으로 밈)
                
                if (dir == 1) pos.x += 3.0f; // 왼쪽 보고 있었으면 오른쪽으로 밈
                else pos.x -= 3.0f;          // 오른쪽 보고 있었으면 왼쪽으로 밈
            }

            // 위치 복구 후 박스 재갱신
            KnightMove();
        }


        // ========================================================
        // 2단계: 수직(Y) 이동 및 충돌 처리 (중력 + 점프)
        // ========================================================

        // 2-1. 중력 적용 (대시 중엔 무시)
        if (!isDashing)
        {
            if (!grounded) pos.y += gravity;
        }

        // 2-2. Y축 이동 후 박스 갱신
        KnightMove();

        // 2-3. [수직 충돌 검사]
        bool isLanded = false;

        if (!isDashing) // 대시 중엔 바닥 착지 안 함 (공중 부양)
        {
            for (auto& wall : coll.m_Walls)
            {
                if (IntersectRect(&tempRect, &m_rc, &wall))
                {
                    // 바닥 착지 판정 (내려오고 있고 + 발이 벽 윗면 근처)
                    if (gravity >= 0 && (m_rc.bottom - 30) < wall.top)
                    {
                        grounded = true;
                        canAirDash = true; // 공중 대시 리필
                        isLanded = true;
                        gravity = 0;

                        // [떨림 방지 +1.0f 적용]
                        pos.y = (float)wall.top - (imagesinfo.Height - 70) + 1.0f;

                        // 위치 보정 후 최종 박스 갱신
                        KnightMove();

                        break;
                    }
                    // 머리 찧기 판정 (올라가고 있고 + 머리 쪽이 부딪힘)
                    else if (gravity < 0 && (m_rc.top + 30) > wall.bottom)
                    {
                        gravity = 0; // 상승 힘 제거 (머리 쿵)

                        // 머리 위치 보정: 벽의 바닥(wall.bottom) 바로 아래로 내림
                        // m_rc.top = pos.y - 40 이므로, pos.y = wall.bottom + 40
                        pos.y = (float)wall.bottom + 40.0f;

                        // 박스 갱신
                        KnightMove();

                    }
                }
            }
        }

        if (!isLanded) grounded = false;

        // 2-4. 중력 가속
        if (!grounded && !isDashing)
        {
            gravity += 0.6f;
            if (gravity > 15.0f) gravity = 15.0f;
        }


        // ========================================================
        // 3단계: 애니메이션 처리
        // ========================================================
        if (GetTickCount() - m_KnightAniTime > 50)
        {
            // 공격 애니메이션이 최우선!
            if (isAttacking)
            {
                DWORD attackTime = GetTickCount() - attackStartTime;
                int frameOffset = 0;
                if (attackTime < 100)      frameOffset = 0;
                else if (attackTime < 200) frameOffset = 1;
                else                       frameOffset = 2;

                // 공격 타입에 따라 15, 18, 21번대 애니메이션을 각각 재생
                if (attackType == 0)      m_KnightCount = 15 + frameOffset;
                else if (attackType == 1) m_KnightCount = 18 + frameOffset;
                else if (attackType == 2) m_KnightCount = 21 + frameOffset;
            }
            else if (isDashing)
            {
                DWORD dashTime = GetTickCount() - dashStartTime;
                if (dashTime < 50)       m_KnightCount = 11;
                else if (dashTime < 100) m_KnightCount = 12;
                else if (dashTime < 150) m_KnightCount = 13;
                else                     m_KnightCount = 14;
            }
            else if (grounded)
            {
                if (isMove) { m_KnightCount++; if (m_KnightCount > 2) m_KnightCount = 0; }
                else if (isLookup) m_KnightCount = 4;
                else if (isLookdown) m_KnightCount = 5;
                else m_KnightCount = 3;
            }
            else
            {
                if (gravity < -12.0f) m_KnightCount = 6;
                else if (gravity < -4.0f) m_KnightCount = 7;
                else if (gravity < 4.0f) m_KnightCount = 8;
                else m_KnightCount = 9;
            }
            m_KnightAniTime = GetTickCount();
        }
    }
}

//void Knight::Update()
//{
//    if (GetTickCount() - m_KnightAniTime > 10)
//    {
//
//
//        // Gravity
//        if (!grounded) pos.y += gravity;
//
//        // Collision Check
//        bool isCollided = false; // 이번 프레임에 땅에 닿았는지 체크
//        RECT tempRect;
//
//        // coll.m_Walls 리스트를 순회
//        for (auto& wall : coll.m_Walls)
//        {
//            if (IntersectRect(&tempRect, &m_rc, &wall))
//            {
//                // [착지 판정] 떨어지는 중이고(gravity >= 0), 발이 벽 윗부분에 걸쳤을 때
//                // (m_rc.bottom - 20)은 발바닥 조금 위, (wall.top + 20)은 벽 윗면 조금 아래
//                // 즉, "발이 벽 윗모서리를 타고 넘었을 때"만 착지로 인정
//                if (gravity >= 0 && (m_rc.bottom - 30) < wall.top)
//                {
//                    // 1. 땅에 닿음 처리
//                    grounded = true;
//                    isCollided = true;
//                    gravity = 0; // 중력 초기화 (안 그러면 계속 빨라짐)
//
//                    // 2. 위치 보정 (스냅)
//                    // 캐릭터를 벽 바로 위로 끌어올림 (1px 정도 겹치게 해서 다음 프레임에도 충돌 유지)
//                    pos.y = (float)wall.top - (imagesinfo.Height - 70) + 1.0f;
//
//                    // 3. 보정된 위치로 m_rc 다시 갱신 (중요!)
//                    m_rc.left = pos.x - 40;
//                    m_rc.top = pos.y - 40;
//                    m_rc.right = pos.x + imagesinfo.Width - 50;
//                    m_rc.bottom = pos.y + imagesinfo.Height - 70;
//
//                    break; // 땅 하나만 밟으면 되니까 루프 종료
//                }
//            }
//        }
//
//        // 충돌한 벽이 하나도 없으면 공중에 뜬 것
//        if (isCollided == false)
//        {
//            grounded = false;
//        }
//
//        // 공중에 있을 때만 중력 가속
//        if (!grounded)
//        {
//            gravity += 0.5f; // 0.8f는 너무 빠를 수 있어 조절함
//            if (gravity > 15.0f) gravity = 15.0f; // 최대 낙하 속도 제한 (안전장치)
//        }
//
//
//        m_rc.left = pos.x-40;
//        m_rc.top = pos.y-40;
//        m_rc.right = pos.x + imagesinfo.Width-50;
//        m_rc.bottom = pos.y + imagesinfo.Height-70;
//
//        //if (GetTickCount() - m_KnightAniTime > 100)
//        //{
//        //    if (isMove)
//        //    {
//        //        m_KnightCount++;
//        //        if (m_KnightCount > 2) m_KnightCount = 0;
//        //    }
//        //    else if (isLookup)
//        //    {
//        //        m_KnightCount = 4;
//        //    }
//        //    else if (isLookdown)
//        //    {
//        //        m_KnightCount = 5;
//        //    }
//        //    else
//        //    {
//        //        m_KnightCount = 3;
//        //    }
//        //    m_KnightAniTime = GetTickCount();
//        //}
//        ////m_KnightAniTime = GetTickCount();
//
//        // =========================================================
//        // [애니메이션 상태 머신] (속도에 따라 이미지 교체)
//        // =========================================================
//        if (GetTickCount() - m_KnightAniTime > 50) // 애니메이션 속도
//        {
//            if (grounded)
//            {
//                // [땅]
//                if (isMove)
//                {
//                    m_KnightCount++;
//                    if (m_KnightCount > 2) m_KnightCount = 0;
//                }
//                else if (isLookup) m_KnightCount = 4;
//                else if (isLookdown) m_KnightCount = 5;
//                else m_KnightCount = 3; // 기본 대기
//            }
//            else
//            {
//                // [공중] 수직 속도(gravity)에 따라 이미지 변경
//                // gravity는 위로 갈수록 음수(-), 아래로 갈수록 양수(+)
//
//                if (gravity < -12.0f)      m_KnightCount = 6; // 점프 준비 (빠른 상승)
//                else if (gravity < -4.0f)  m_KnightCount = 7; // 상승 중
//                else if (gravity < 4.0f)   m_KnightCount = 8; // 꼭대기 (체공)
//                else if (gravity < 12.0f)  m_KnightCount = 9; // 하강 시작
//                else                       m_KnightCount = 9; // 착지 준비(fall02는 착지 순간 연출용이라 일단 fall01 유지)
//
//                // 참고: fall02(착지)는 grounded가 true가 된 직후 잠깐 보여줘야 해서 로직이 복잡하므로 
//                // 일단 하강 이미지를 계속 쓰도록 했습니다.
//            }
//            m_KnightAniTime = GetTickCount();
//        }
//    }
//    
//}

void Knight::Draw()
{
    if (Gmanager.m_GameStart == true)
    {
        // 🌟 무적 시간일 때 0.1초 단위로 깜빡이기 (짝수 틱에만 렌더링 무시)
        if (isInvincible && ((GetTickCount() / 100) % 2 == 0)) {
            // 이 프레임은 기사를 그리지 않고 넘김 (깜빡깜빡)
        }
        else 
        {
            float drawingOffsetY = 0.0f;
            if (m_KnightCount == 5)
            {
                // 이 숫자를 조절해서 발을 땅에 맞춤 
                // 이미지가 납작한 만큼 더해줘야 함. (예: 10 ~ 30 사이)
                drawingOffsetY = 20.0f;
            }
            Knightimg[m_KnightCount].Render(pos.x - CAM->GetX(), pos.y - CAM->GetY() + drawingOffsetY, 0, dir, 1, 1);


            // =======================================================
            // 2. hit / unhit 상태에 따른 공격 이펙트 그리기
            // =======================================================

            if (isAttacking)
            {
                float renderX = pos.x - CAM->GetX();
                float renderY = pos.y - CAM->GetY();

                if (attackType == 0) // [측면 공격]
                {
                    float effectOffsetX = 60.0f;
                    float effectOffsetY = -30.0f;

                    if (dir == -1) // 오른쪽 (반전 필요)
                    {
                        if (isAttackHit) hitEffect[0].Render(renderX + effectOffsetX, renderY + effectOffsetY, 0, -1, 1, 1);
                        else             unhitEffect[0].Render(renderX + effectOffsetX, renderY + effectOffsetY, 0, -1, 1, 1);
                    }
                    else // 왼쪽 (dir == 1, 원본)
                    {
                        if (isAttackHit) hitEffect[0].Render(renderX - effectOffsetX, renderY + effectOffsetY, 0, 1, 1, 1);
                        else             unhitEffect[0].Render(renderX - effectOffsetX, renderY + effectOffsetY, 0, 1, 1, 1);
                    }
                }
                else // [상단 / 하단 공격] -> hitEffect[1] 사용
                {
                    float effectOffsetX = 0.0f; // 몸의 중심에서 나감
                    float effectOffsetY = (attackType == 1) ? -80.0f : 50.0f; // 위 공격은 마이너스, 아래는 플러스 좌표

                    // 기본이 하단 이펙트이므로, 상단 공격(1)일 때는 Y축 크기를 -1로 주어 위아래 반전시킴!
                    float scaleY = (attackType == 1) ? -1.0f : 1.0f;

                    if (isAttackHit) hitEffect[1].Render(renderX + effectOffsetX, renderY + effectOffsetY, 0, dir, scaleY, 1);
                    else             unhitEffect[1].Render(renderX + effectOffsetX, renderY + effectOffsetY, 0, dir, scaleY, 1);
                }
            }

            //if (isAttacking)
            //{
            //    float effectOffsetX = -30.0f;
            //    float effectOffsetY = -10.0f;
            //    float renderX = pos.x - CAM->GetX();
            //    float renderY = pos.y - CAM->GetY() + effectOffsetY;
            //    if (dir == 1) // 왼쪽을 보고 쏠 때
            //    {
            //        if (isAttackHit) hitEffect[0].Render(renderX + effectOffsetX, renderY, 0, 1, 1, 1);
            //        else             unhitEffect[0].Render(renderX + effectOffsetX, renderY, 0, 1, 1, 1);
            //    }
            //    else // 오른쪽을 보고 쏠 때 (반전)
            //    {
            //        if (isAttackHit) hitEffect[0].Render(renderX - effectOffsetX, renderY, 0, -1, 1, 1);
            //        else             unhitEffect[0].Render(renderX - effectOffsetX, renderY, 0, -1, 1, 1);
            //    }
            //}

            if (grounded)
            {
                coll.BoxSow(m_rc, 0, -5, 0xffff0000);
            }
            else
            {
                coll.BoxSow(m_rc, 0, -5);
            }

            if (isAttacking)
            {
                coll.BoxSow(attackBox, 0, 0, D3DCOLOR_ARGB(255, 255, 255, 0)); // 노란색
            }
        }
    }
}

void Knight::KnightMove()
{
    m_rc.left = pos.x - 30;
    m_rc.top = pos.y - 40;
    m_rc.right = pos.x + imagesinfo.Width - 40;
    m_rc.bottom = pos.y + imagesinfo.Height - 70;
}

// 점프 시작 함수 (Key.cpp에서 호출)
void Knight::JumpStart()
{
    if (grounded) // 땅에 있을 때만 점프 가능
    {
        // 최대 점프력 설정 (값이 클수록 높이 뜀)
        gravity = -20.0f;
        grounded = false;

        // 점프 소리 재생 (필요 시)
        // sound.Play("Jump");
    }
}

// 가변 점프 (키 뗐을 때 속도 자르기)
void Knight::JumpCut()
{
    // 공중에서 상승 중일 때(음수)만 속도를 깎음
    if (!grounded && gravity < -5.0f)
    {
        // 현재 상승 속도를 절반으로 줄여버림 -> 뚝 떨어지는 느낌 구현
        gravity *= 0.5f;
    }
}

// 대시 시작
void Knight::DashStart()
{
    DWORD curTime = GetTickCount64();

    // 1. 쿨타임 체크 (0.5초 = 500ms)
    if (curTime - dashCooldownTime < 500) return;

    // 2. 공중 사용 제한 체크
    if (!grounded && !canAirDash) return;

    // 3. 대시
    isDashing = true;
    dashStartTime = curTime;
    dashCooldownTime = curTime;
    gravity = 0; // 중력 제거 (일직선 이동)

    // 공중에서 썼다면 기회 소진
    if (!grounded) canAirDash = false;
}

void Knight::AttackStart()
{
    if (isAttacking || isDashing) return;

    // 🌟 1. 전체 공격 쿨타임 체크 (500ms)
    if (GetTickCount() - lastAttackTime < 500) return;

    isAttacking = true;
    isAttackHit = false;
    attackStartTime = GetTickCount();
    lastAttackTime = GetTickCount(); // 쿨타임 타이머 시작

    // 🌟 2. 키 입력 조합으로 공격 타입 결정
    if (!grounded && isLookdown) {
        attackType = 2; // 공중 + 아래키 = 하단 찍기
    }
    else if (isLookup) {
        attackType = 1; // 위키 = 상단 공격
    }
    else {
        attackType = 0; // 일반 측면 공격
    }
}

void Knight::TakeDamage(int damage, int hitDir)
{
    if (isInvincible || isDashing) return;

    hp -= damage;
    if (hp <= 0) hp = 0; // 나중에 게임오버 처리용

    isInvincible = true;
    invincibleTime = GetTickCount();

    // 기사 넉백 (공중으로 띄우고 뒤로 밀어냄)
    isKnockback = true;
    knockVelocity.x = hitDir * 8.0f;
    gravity = -8.0f;
    grounded = false;
}