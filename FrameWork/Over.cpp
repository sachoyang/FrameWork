#include "Include.h"


Over::Over()
{
}

Over::~Over()
{
}

void Over::Init()
{
	m_Title.Create("./resource/Img/UI/gameover_text.png", false, 0);
	D3DXGetImageInfoFromFile("./resource/Img/UI/gameover_text.png", &imagesinfo);

	m_Alpha = 0; // 투명하게 시작 (서서히 나타남)
}

// Chap, 재정의 함수 호출
void Over::Update(double frame)
{
	// 페이드 인 연출만 담당
	if (m_Alpha < 255) {
		m_Alpha += 2;
		if (m_Alpha > 255) m_Alpha = 255;
	}
}

void Over::Draw()
{
	// 1. 게임오버 이미지 그리기 (Alpha값 적용)
	m_Title.SetColor(255, 255, 255, m_Alpha);

	m_Title.Render((SCREEN_WITH - imagesinfo.Width) / 2, 0, 0, 1, 1); // 화면 중앙 배치 (좌표 조절 필요)

	// 2. 안내 문구 출력 (연출이 끝났을 때만 표시)
	if (m_Alpha >= 255)
	{
		// 깜빡이는 효과를 위해 시간 활용 (선택사항)
		if ((GetTickCount() / 500) % 2 == 0)
		{
			// 화면 하단 중앙에 안내 메시지 출력
			dv_font.DrawString("PRESS ANY BUTTON TO TITLE", SCREEN_WITH / 2 - 150, 600, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
}

void Over::OnMessage( MSG* msg )
{
	// 연출이 다 끝난 뒤에만 입력을 받음
	if (m_Alpha >= 255)
	{
		switch (msg->message)
		{
		case WM_KEYDOWN: // 키보드 아무거나 눌렀을 때
			g_Mng.n_Chap = MENU;
			break;

		case WM_LBUTTONDOWN: // 마우스 클릭했을 때
			g_Mng.n_Chap = MENU;
			break;
		}
	}
}