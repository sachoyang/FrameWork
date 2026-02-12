#pragma once
#include "Include.h"

class Knight
{
public:
	Sprite Knightimg[15];

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

	bool isDashing;         // 현재 대시 중인가?
	bool canAirDash;        // 공중 대시 가능한가? (점프 후 1회 제한)

	DWORD dashStartTime;    // 대시 시작 시간 (지속시간 체크용)
	DWORD dashCooldownTime; // 대시 쿨타임 체크용

	float dashSpeed;        // 대시 속도

	void Init();
	void Update();
	void Draw();

	// 점프 관련 함수
	void JumpStart(); // Z키 누를 때 (점프 시작)
	void JumpCut();   // Z키 뗄 때 (속도 줄이기)
	// 대시 시작 함수
	void DashStart();
};

extern Knight knight;