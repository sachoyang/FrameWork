#include "Include.h"

UIManager uiMng;

UIManager::UIManager()
{
    m_bLargeMap = false; // 기본은 우측 상단 미니맵 모드
}

UIManager::~UIManager()
{
}

void UIManager::Init()
{
}

void UIManager::Update()
{
    // 나중에 체력 애니메이션 등이 생기면 여기서 처리
}

void UIManager::Draw()
{
    if (!Gmanager.m_GameStart) return;

    // 1. 미니맵 그리기
    DrawMinimap();

    // 2. 향후 체력바, 스킬 아이콘 등은 여기에 추가
}

void UIManager::DrawMinimap()
{
    // M키 상태에 따라 맵 크기와 위치 변경
    float startX = m_bLargeMap ? SCREEN_WITH / 2.0f - 150.0f : SCREEN_WITH - 200.0f;
    float startY = m_bLargeMap ? SCREEN_HEIGHT / 2.0f - 150.0f : 30.0f;
    float spacingX = m_bLargeMap ? 60.0f : 25.0f;
    float spacingY = m_bLargeMap ? 60.0f : 25.0f;

    if (m_bLargeMap) {
        dv_font.DrawString("== WORLD MAP ==", startX + 50, startY - 40, D3DCOLOR_ARGB(255, 255, 255, 0));
    }

    for (int y = 0; y < 6; y++)
    {
        for (int x = 0; x < 6; x++)
        {
            int rID = mapMng.m_Grid[y][x];
            if (rID == 0) continue; // 방이 없는 칸은 패스

            MapChunk* room = &mapMng.m_MapList[rID];

            float drawX = startX + x * spacingX;
            float drawY = startY + y * spacingY;

            // 1. 방 모양과 색상 설정
            char roomChar[10] = "[ ]";
            D3DCOLOR color = D3DCOLOR_ARGB(255, 150, 150, 150); // 일반 방 (회색)

            if (room->prefabID == 8) {
                strcpy_s(roomChar, "[S]");
                color = D3DCOLOR_ARGB(255, 100, 200, 255); // 시작 방 (파랑)
            }
            if (room->prefabID == 4 || room->prefabID == 16) {
                strcpy_s(roomChar, "[B]");
                color = D3DCOLOR_ARGB(255, 255, 100, 100); // 보스 방 (빨강)
            }
            if (rID == mapMng.m_pCurrentMapChunk->id) {
                strcpy_s(roomChar, "[*]");
                color = D3DCOLOR_ARGB(255, 100, 255, 100); // 현재 위치 (초록)
            }

            // 방 그리기
            dv_font.DrawString(roomChar, drawX, drawY, color);

            // 2. 실제 연결된 문(길)만 정확하게 선으로 그리기
            D3DCOLOR doorColor = D3DCOLOR_ARGB(255, 100, 100, 100);

            // 오른쪽으로 통로가 진짜 열려있을 때만
            if (room->nextMapID[DIR_RIGHT] != 0) {
                dv_font.DrawString("-", drawX + (spacingX * 0.6f), drawY, doorColor);
            }
            // 아래쪽으로 통로가 진짜 열려있을 때만
            if (room->nextMapID[DIR_DOWN] != 0) {
                dv_font.DrawString("|", drawX + 8, drawY + (spacingY * 0.5f), doorColor);
            }
        }
    }
}