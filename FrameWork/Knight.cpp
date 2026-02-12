#include "Include.h"

Knight knight;


Knight::Knight()
{
    m_KnightAniTime = 0;
    m_KnightCount = 0;
    isMove = 0;
    isLookup = 0;
    isLookdown = 0;
    pos.x = 100;
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

    // =========================================================
    // [추가] 점프 및 낙하 이미지 로딩 (6 ~ 10번)
    // =========================================================
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

    m_rc.left = pos.x-40;
    m_rc.top = pos.y-40;
    m_rc.right = pos.x + imagesinfo.Width-50;
    m_rc.bottom = pos.y + imagesinfo.Height-70;
}


void Knight::Update()
{
    if (GetTickCount() - m_KnightAniTime > 10)
    {
        // =========================================================
        // [대시 로직]
        // =========================================================
        if (isDashing)
        {
            // 1. 대시 지속 시간 체크 (예: 0.2초 동안 대시)
            if (GetTickCount() - dashStartTime > 200)
            {
                isDashing = false;
                gravity = 0; // 대시 끝나면 정지 상태에서 낙하 시작
            }
            else
            {
                // 2. 대시 이동 (보는 방향으로 직진)
                pos.x -= dir * dashSpeed;

                // 3. 수평 충돌 체크 (앞에 벽이 있나?)
                // 일단 이동한 위치로 m_rc 갱신
                m_rc.left = pos.x - 40;
                m_rc.top = pos.y - 40;
                m_rc.right = pos.x + imagesinfo.Width - 50;
                m_rc.bottom = pos.y + imagesinfo.Height - 70;

                // 바닥 충돌을 피하기 위해 검사 박스를 위아래로 조금 줄입니다.
                RECT wallCheckRect = m_rc;
                wallCheckRect.top += 10;    // 머리 쪽 여유
                wallCheckRect.bottom -= 10; // 발 쪽 여유 (바닥 닿지 않게)

                RECT tempRect;
                for (auto& wall : coll.m_Walls)
                {
                    if (IntersectRect(&tempRect, &wallCheckRect, &wall))
                    {
                        // 벽에 부딪힘 -> 대시 즉시 종료 & 위치 복구
                        isDashing = false;
                        pos.x -= dir * dashSpeed; // 뚫고 들어간 만큼 뺌
                        break;
                    }
                }
            }
        }
        else
        {
            // [일반 상태] (대시 중이 아닐 때만 중력 & 일반 이동 적용)

            // 1. 중력 적용
            if (!grounded) pos.y += gravity;

            // 2. 수직 충돌 체크 (기존 코드)
            bool isCollided = false;
            RECT tempRect;

            // 이동한 y좌표 기준 m_rc 갱신
            m_rc.left = pos.x - 40;
            m_rc.top = pos.y - 40;
            m_rc.right = pos.x + imagesinfo.Width - 50;
            m_rc.bottom = pos.y + imagesinfo.Height - 70;

            for (auto& wall : coll.m_Walls)
            {
                if (IntersectRect(&tempRect, &m_rc, &wall))
                {
                    // 바닥 착지 로직
                    if (gravity >= 0 && (m_rc.bottom - 30) < wall.top)
                    {
                        grounded = true;
                        canAirDash = true; // [중요] 땅 밟으면 공중대시 리필!
                        isCollided = true;
                        gravity = 0;
                        pos.y = (float)wall.top - (imagesinfo.Height - 70) + 1.0f; // 1px 겹침 유지
                        break;
                    }
                }
            }
            if (!isCollided) grounded = false;

            // 3. 중력 가속
            if (!grounded)
            {
                gravity += 0.6f;
                if (gravity > 15.0f) gravity = 15.0f;
            }
        }

        // 공통: 충돌 박스 최종 갱신
        m_rc.left = pos.x - 40;
        m_rc.top = pos.y - 40;
        m_rc.right = pos.x + imagesinfo.Width - 50;
        m_rc.bottom = pos.y + imagesinfo.Height - 70;


        // =========================================================
        // [애니메이션]
        // =========================================================
        if (GetTickCount() - m_KnightAniTime > 50)
        {
            if (isDashing)
            {
                // 대시 애니메이션 (시간 흐름에 따라 11 -> 12 -> 13 -> 14)
                DWORD dashTime = GetTickCount() - dashStartTime;

                if (dashTime < 50)       m_KnightCount = 11; // 준비
                else if (dashTime < 100) m_KnightCount = 12; // 슝~
                else if (dashTime < 150) m_KnightCount = 13; // 공중 부양
                else                     m_KnightCount = 14; // 정지 동작
            }
            else if (grounded)
            {
                // ... (기존 땅 애니메이션) ...
                if (isMove) { m_KnightCount++; if (m_KnightCount > 2) m_KnightCount = 0; }
                else if (isLookup) m_KnightCount = 4;
                else if (isLookdown) m_KnightCount = 5;
                else m_KnightCount = 3;
            }
            else
            {
                // ... (기존 공중 애니메이션) ...
                if (gravity < -12.0f) m_KnightCount = 6;
                else if (gravity < -4.0f) m_KnightCount = 7;
                else if (gravity < 4.0f) m_KnightCount = 8;
                else m_KnightCount = 9;
            }
            m_KnightAniTime = GetTickCount();
        }
    }
}
//
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
        float drawingOffsetY = 0.0f;
        if (m_KnightCount == 5)
        {
            // ▼ 이 숫자를 조절해서 발을 땅에 맞추세요! 
            // 이미지가 납작한 만큼 더해줘야 합니다. (예: 10 ~ 30 사이)
            drawingOffsetY = 20.0f;
        }
        Knightimg[m_KnightCount].Render(this->pos.x, this->pos.y + drawingOffsetY, 0, dir, 1, 1);

        /*if (isMove == 1)
        {
            Knightimg[m_KnightCount].Render(this->pos.x, this->pos.y, 0, dir, 1, 1);
        }
        else
        {
            Knightimg[m_KnightCount].Render(this->pos.x, this->pos.y, 0, dir, 1, 1);
        }*/
        if (grounded)
        {
            coll.BoxSow(m_rc, 0, -5, 0xffff0000);
        }
        else
        {
            coll.BoxSow(m_rc, 0, -5);
        }
    }
}

// [추가] 점프 시작 함수 (Key.cpp에서 호출)
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

// [추가] 가변 점프 (키 뗐을 때 속도 자르기)
void Knight::JumpCut()
{
    // 공중에서 상승 중일 때(음수)만 속도를 깎음
    if (!grounded && gravity < -5.0f)
    {
        // 현재 상승 속도를 절반으로 줄여버림 -> 뚝 떨어지는 느낌 구현
        gravity *= 0.5f;
    }
}

// [추가] 대시 시작
void Knight::DashStart()
{
    DWORD curTime = GetTickCount();

    // 1. 쿨타임 체크 (0.5초 = 500ms)
    if (curTime - dashCooldownTime < 500) return;

    // 2. 공중 사용 제한 체크
    if (!grounded && !canAirDash) return;

    // 3. 대시 발동!
    isDashing = true;
    dashStartTime = curTime;
    dashCooldownTime = curTime;
    gravity = 0; // 중력 제거 (일직선 이동)

    // 공중에서 썼다면 기회 소진
    if (!grounded) canAirDash = false;
}