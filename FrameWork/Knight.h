#pragma once
#include "Include.h"

class Knight
{
public:
	Sprite Knightimg[6];

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

};

extern Knight knight;