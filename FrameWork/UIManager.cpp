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
	m_bLargeMap = false; // 게임 시작 시에는 항상 미니맵 모드로 시작
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
    float startX = m_bLargeMap ? SCREEN_WITH / 2.0f - 180.0f : SCREEN_WITH - 230.0f;
    float startY = m_bLargeMap ? SCREEN_HEIGHT / 2.0f - 180.0f : 40.0f;
    float spacingX = m_bLargeMap ? 70.0f : 35.0f;
    float spacingY = m_bLargeMap ? 70.0f : 35.0f;

    if (m_bLargeMap) {
        dv_font.DrawString("== WORLD MAP ==", startX + 70, startY - 50, D3DCOLOR_ARGB(255, 255, 255, 0));
    }

    for (int y = 0; y < 6; y++)
    {
        for (int x = 0; x < 6; x++)
        {
            int rID = mapMng.m_Grid[y][x];
            if (rID == 0) continue;

            MapChunk* room = &mapMng.m_MapList[rID];

            float drawX = startX + x * spacingX;
            float drawY = startY + y * spacingY;

            // =========================================================
            // 🌟 1. 방 모양 결정 (모두 심플한 네모로 통일!)
            // =========================================================
            char roomChar[10] = "■";

            // 시작 방(S)과 보스 방(B)만 예외 처리
            if (rID == 1) {
                strcpy_s(roomChar, "S");
            }
            else if (room->prefabID == 4 || room->prefabID == 16) {
                strcpy_s(roomChar, "B");
            }

            // =========================================================
            // 2. 색상 결정 
            // =========================================================
            D3DCOLOR color = D3DCOLOR_ARGB(255, 180, 180, 180); // 일반 방 (회색)

            if (rID == 1) color = D3DCOLOR_ARGB(255, 100, 200, 255); // 시작 (파랑)
            else if (room->prefabID == 4 || room->prefabID == 16) color = D3DCOLOR_ARGB(255, 255, 100, 100); // 보스 (빨강)

            // 현재 위치는 빛나는 초록색
            if (rID == mapMng.m_pCurrentMapChunk->id) {
                color = D3DCOLOR_ARGB(255, 50, 255, 50);
            }

            // 기호 렌더링
            dv_font.DrawString(roomChar, drawX, drawY, color);

            // =========================================================
            // 3. 맵 연결선 그리기
            // =========================================================
            D3DCOLOR doorColor = D3DCOLOR_ARGB(255, 100, 100, 100);

            // [오른쪽 검사]
            if (x + 1 < 6)
            {
                int rightRoomID = mapMng.m_Grid[y][x + 1];

                if (rightRoomID == rID) {
                    // 👉 나랑 같은 맵(큰 방)이면 굵은 선으로 끈끈하게 연결
                    dv_font.DrawString(m_bLargeMap ? "===" : "==", drawX + (spacingX * 0.5f), drawY, color);
                }
                else if (rightRoomID != 0 && room->nextMapID[DIR_RIGHT] == rightRoomID) {
                    // 👉 다른 방으로 이어지는 통로(문)면 얇은 선(-)으로 연결
                    dv_font.DrawString("-", drawX + (spacingX * 0.45f), drawY, doorColor);
                }
            }

            // [아래쪽 검사]
            if (y + 1 < 6)
            {
                int downRoomID = mapMng.m_Grid[y + 1][x];

                if (downRoomID == rID) {
                    // 👉 나랑 같은 맵(큰 방)이면 굵은 선
                    dv_font.DrawString("||", drawX + 8, drawY + (spacingY * 0.4f), color);
                }
                else if (downRoomID != 0 && room->nextMapID[DIR_DOWN] == downRoomID) {
                    // 👉 다른 방으로 이어지는 통로(문)면 얇은 선(|)
                    dv_font.DrawString("|", drawX + 4, drawY + (spacingY * 0.45f), doorColor);
                }
            }
        }
    }
}