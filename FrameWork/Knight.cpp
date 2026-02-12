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

    m_rc.left = pos.x-40;
    m_rc.top = pos.y-40;
    m_rc.right = pos.x + imagesinfo.Width-50;
    m_rc.bottom = pos.y + imagesinfo.Height-70;
}

void Knight::Update()
{
    if (GetTickCount() - m_KnightAniTime > 10)
    {
        // Gravity
        if (!grounded) pos.y += gravity;

        // Collision Check
        bool isCollided = false; // 이번 프레임에 땅에 닿았는지 체크
        RECT tempRect;

        // coll.m_Walls 리스트를 순회
        for (auto& wall : coll.m_Walls)
        {
            if (IntersectRect(&tempRect, &m_rc, &wall))
            {
                // [착지 판정] 떨어지는 중이고(gravity >= 0), 발이 벽 윗부분에 걸쳤을 때
                // (m_rc.bottom - 20)은 발바닥 조금 위, (wall.top + 20)은 벽 윗면 조금 아래
                // 즉, "발이 벽 윗모서리를 타고 넘었을 때"만 착지로 인정
                if (gravity >= 0 && (m_rc.bottom - 30) < wall.top)
                {
                    // 1. 땅에 닿음 처리
                    grounded = true;
                    isCollided = true;
                    gravity = 0; // 중력 초기화 (안 그러면 계속 빨라짐)

                    // 2. 위치 보정 (스냅)
                    // 캐릭터를 벽 바로 위로 끌어올림 (1px 정도 겹치게 해서 다음 프레임에도 충돌 유지)
                    //pos.y = (float)wall.top - (imagesinfo.Height - 70);

                    // 3. 보정된 위치로 m_rc 다시 갱신 (중요!)
                    m_rc.left = pos.x - 40;
                    m_rc.top = pos.y - 40;
                    m_rc.right = pos.x + imagesinfo.Width - 50;
                    m_rc.bottom = pos.y + imagesinfo.Height - 70;

                    break; // 땅 하나만 밟으면 되니까 루프 종료
                }
            }
        }

        // 충돌한 벽이 하나도 없으면 공중에 뜬 것
        if (isCollided == false)
        {
            grounded = false;
        }

        // 공중에 있을 때만 중력 가속
        if (!grounded)
        {
            gravity += 0.5f; // 0.8f는 너무 빠를 수 있어 조절함
            if (gravity > 15.0f) gravity = 15.0f; // 최대 낙하 속도 제한 (안전장치)
        }


        m_rc.left = pos.x-40;
        m_rc.top = pos.y-40;
        m_rc.right = pos.x + imagesinfo.Width-50;
        m_rc.bottom = pos.y + imagesinfo.Height-70;
        if (GetTickCount() - m_KnightAniTime > 100)
        {
            if (isMove)
            {
                m_KnightCount++;
                if (m_KnightCount > 2) m_KnightCount = 0;
            }
            else if (isLookup)
            {
                m_KnightCount = 4;
            }
            else if (isLookdown)
            {
                m_KnightCount = 5;
            }
            else
            {
                m_KnightCount = 3;
            }
            m_KnightAniTime = GetTickCount();
        }
        //m_KnightAniTime = GetTickCount();
    }
    
}

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