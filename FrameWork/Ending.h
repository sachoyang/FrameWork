#pragma once
#include "Include.h"

class Ending : public Chap
{
public:
	Ending();
	~Ending();

	Sprite m_Bg;          // 클리어 화면 이미지
	RECT   m_rtThanks;    // Special Thanks 버튼 영역

	virtual void Init();
	virtual void Update(double frame);
	virtual void Draw();
	virtual void OnMessage(MSG* msg);
};