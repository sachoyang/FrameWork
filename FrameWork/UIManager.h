#pragma once
#include "Include.h"

class UIManager
{
public:
    UIManager();
    ~UIManager();

    bool m_bLargeMap; // M키를 눌렀을 때 맵 확대 및 일시정지 여부

    // 체력 UI 이미지
    Sprite heartFull;
    Sprite heartEmpty;

    void Init();
    void Update();
    void Draw();

    void DrawMinimap();
};

extern UIManager uiMng;