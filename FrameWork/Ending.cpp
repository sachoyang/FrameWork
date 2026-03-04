#include "Include.h"

Ending::Ending() {}
Ending::~Ending() {}

void Ending::Init()
{
	// 1. 엔딩 이미지 로드 (경로는 실제 파일에 맞춰주세요!)
	// 예: ./resource/Img/UI/Clear.png
	m_Bg.Create("./resource/Img/UI/Clear.png", false, 0);

	// 2. Special Thanks 버튼 영역 설정 (이미지 상의 버튼 위치에 맞춰 좌표 수정 필요!)
	// 예: 화면 중앙 하단쯤 (x: 500~780, y: 600~650)
	SetRect(&m_rtThanks, 500, 600, 780, 650);
}

void Ending::Update(double frame)
{
	// 특별한 업데이트 로직 없음 (이미지만 보여줌)
}

void Ending::Draw()
{
	// 배경 그리기
	m_Bg.Render(0, 0, 0, 1, 1);

	// (디버그용: 버튼 위치 확인하려면 주석 해제)
	dv_font.DrawString("Special Thanks", m_rtThanks.left, m_rtThanks.top, D3DCOLOR_ARGB(255, 255, 0, 0));
}

void Ending::OnMessage(MSG* msg)
{
	switch (msg->message)
	{
	case WM_LBUTTONDOWN:
	{
		POINT pt;
		pt.x = LOWORD(msg->lParam);
		pt.y = HIWORD(msg->lParam);

		// Special Thanks 버튼 클릭 시 HTML 파일 열기!
		if (PtInRect(&m_rtThanks, pt))
		{
			// 현재 폴더에 있는 Thanks.html을 기본 브라우저로 엽니다.
			// (파일이 프로젝트 폴더나 실행 파일 옆에 있어야 함)
			ShellExecute(NULL, "open", "Thanks.html", NULL, NULL, SW_SHOWNORMAL);
		}
		// 그 외 화면을 클릭하면 메뉴로 복귀
		else
		{
			g_Mng.n_Chap = MENU;
		}
	}
	break;

	// 키보드 아무거나 눌러도 메뉴로
	case WM_KEYDOWN:
		g_Mng.n_Chap = MENU;
		break;
	}
}