#pragma once
#include "Include.h"

class Bird
{
	Sprite Bdimg[2];

public:
	Bird();
	~Bird();

	float x, y;
	int dir;

	DWORD m_BirdAniTime;
	int m_BirdCount;

	float rot;

	void Init();
	void Update();
	void Draw();

};

extern Bird bird;