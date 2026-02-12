#pragma once
#include "Include.h"

class Knight
{
public:
	Sprite Knightimg[11];

	float gravity;
	bool grounded;

public:
	Knight();
	~Knight();

	int dir;

	D3DXIMAGE_INFO imagesinfo;
	RECT m_rc;
	D3DXVECTOR2 pos;

	DWORD m_KnightAniTime;
	int m_KnightCount;

	bool isMove;
	bool isLookup, isLookdown;

	void Init();
	void Update();
	void Draw();

	// [추가] 점프 관련 함수
	void JumpStart(); // Z키 누를 때 (점프 시작)
	void JumpCut();   // Z키 뗄 때 (속도 줄이기)

};

extern Knight knight;