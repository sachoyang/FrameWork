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
            // 🌟 2. 방 모양 결정 (네모 & 직각 꺽쇠로 완벽한 상자 형태 구현)
            // =========================================================
            char roomChar[10] = "■";

            bool isLeft = true, isRight = true, isTop = true, isBottom = true;
            if (x > 0 && mapMng.m_Grid[y][x - 1] == rID) isLeft = false;
            if (x < 5 && mapMng.m_Grid[y][x + 1] == rID) isRight = false;
            if (y > 0 && mapMng.m_Grid[y - 1][x] == rID) isTop = false;
            if (y < 5 && mapMng.m_Grid[y + 1][x] == rID) isBottom = false;

            if (isLeft && isRight && isTop && isBottom) {
                strcpy_s(roomChar, "■"); // 1x1 일반 네모
            }
            else {
                // 2x2 맵 (직각 꺽쇠)
                if (isTop && isLeft && !isRight && !isBottom) strcpy_s(roomChar, "┌");
                else if (isTop && isRight && !isLeft && !isBottom) strcpy_s(roomChar, "┐");
                else if (isBottom && isLeft && !isRight && !isTop) strcpy_s(roomChar, "└");
                else if (isBottom && isRight && !isLeft && !isTop) strcpy_s(roomChar, "┘");

                // 🌟 2x1 맵 (가로 네모 꺽쇠) -> [ 와 ] 사용
                else if (isTop && isBottom && isLeft && !isRight) strcpy_s(roomChar, "[");
                else if (isTop && isBottom && isRight && !isLeft) strcpy_s(roomChar, "]");

                // 🌟 1x2 맵 (세로 네모 꺽쇠) -> ┌ 와 └ 사용 (길쭉한 네모 느낌)
                else if (isLeft && isRight && isTop && !isBottom) strcpy_s(roomChar, "┌");
                else if (isLeft && isRight && isBottom && !isTop) strcpy_s(roomChar, "└");
            }

            // 3. 시작 방/보스 방 S, B 표시
            if (rID == 1) {
                strcpy_s(roomChar, "S");
            }
            else if (room->prefabID == 4 || room->prefabID == 16) {
                strcpy_s(roomChar, "B");
            }

            // 4. 색상 결정 
            D3DCOLOR color = D3DCOLOR_ARGB(255, 180, 180, 180);

            if (rID == 1) color = D3DCOLOR_ARGB(255, 100, 200, 255);
            else if (room->prefabID == 4 || room->prefabID == 16) color = D3DCOLOR_ARGB(255, 255, 100, 100);

            if (rID == mapMng.m_pCurrentMapChunk->id) {
                color = D3DCOLOR_ARGB(255, 50, 255, 50);
            }

            dv_font.DrawString(roomChar, drawX, drawY, color);

            // =========================================================
            // 5. 실제 연결된 문(통로)만 선으로 연결 및 대형 맵 병합
            // =========================================================
            D3DCOLOR doorColor = D3DCOLOR_ARGB(255, 100, 100, 100);

            if (x + 1 < 6)
            {
                int rightRoomID = mapMng.m_Grid[y][x + 1];
                if (rightRoomID == rID) {
                    // 나랑 같은 맵 병합
                    dv_font.DrawString(m_bLargeMap ? "===" : "==", drawX + (spacingX * 0.5f), drawY, color);
                }
                else if (rightRoomID != 0 && room->nextMapID[DIR_RIGHT] == rightRoomID) {
                    // 다른 방 얇은 통로
                    dv_font.DrawString("-", drawX + (spacingX * 0.45f), drawY, doorColor);
                }
            }

            if (y + 1 < 6)
            {
                int downRoomID = mapMng.m_Grid[y + 1][x];
                if (downRoomID == rID) {
                    // 나랑 같은 맵 병합
                    dv_font.DrawString("||", drawX + 8, drawY + (spacingY * 0.4f), color);
                }
                else if (downRoomID != 0 && room->nextMapID[DIR_DOWN] == downRoomID) {
                    // 다른 방 얇은 통로
                    dv_font.DrawString("|", drawX + 4, drawY + (spacingY * 0.45f), doorColor);
                }
            }
        }
    }
}