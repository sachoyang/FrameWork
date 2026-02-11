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
        if (!grounded) pos.y += gravity;

        if (m_rc.left < coll.m_rc.right && coll.m_rc.left < m_rc.right && m_rc.top < coll.m_rc.bottom && coll.m_rc.top < m_rc.bottom) grounded = true;

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