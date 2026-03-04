#include "Include.h"

Ending::Ending() {}
Ending::~Ending() {}

void Ending::Init()
{
	m_hBtn = NULL;
	m_bShow = false;
	// 1. 엔딩 이미지 로드 (경로는 실제 파일에 맞춰주세요!)
	// 예: ./resource/Img/UI/Clear.png
	m_Bg.Create("./resource/Img/UI/Clear.png", false, 0);

	// 2. WinAPI 버튼 생성
	m_hBtn = CreateWindow(
		"BUTTON", "Special Thanks",
		WS_CHILD  | BS_PUSHBUTTON, 
		500, 600, 200, 50,

		// 🌟 [핵심 수정] 전역 g_hWnd 대신, 이미 핸들을 가지고 있는 dv_font를 사용!
		dv_font.d3dpp.hDeviceWindow, // 부모 윈도우 핸들

		(HMENU)ID_BTN_THANKS,
		GetModuleHandle(NULL),
		NULL
	);

	// 폰트 설정
	HFONT hFont = CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, "굴림");
	SendMessage(m_hBtn, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));

	LONG style = GetWindowLong(dv_font.d3dpp.hDeviceWindow, GWL_STYLE);
	SetWindowLong(dv_font.d3dpp.hDeviceWindow, GWL_STYLE, style | WS_CLIPCHILDREN);

	// 버튼을 화면 맨 앞으로 끄집어 냅니다 (Z-Order 상위 보장)
	SetWindowPos(m_hBtn, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void Ending::Update(double frame)
{
	// 🌟 엔딩 씬이 실행 중인데 버튼이 숨어있다면? -> 보여줘라!
	if (m_hBtn != NULL && !IsWindowVisible(m_hBtn))
	{
		ShowWindow(m_hBtn, SW_SHOW);
	}
}

void Ending::Draw()
{
	// 배경 그리기
	m_Bg.Render(0, 0, 0, 1, 1);

	// (디버그용: 버튼 위치 확인하려면 주석 해제)
	//coll.BoxSow(m_rtThanks, 0, 0, D3DCOLOR_ARGB(255, 255, 0, 0));
}

void Ending::OnMessage(MSG* msg)
{
	if (msg->message == WM_COMMAND)
	{
		if (LOWORD(msg->wParam) == ID_BTN_THANKS)
		{
			// 1. HTML 열기 (경로 문제 해결)
			// 파일이 프로젝트 폴더(cpp 파일들 있는 곳)에 있는지 꼭 확인하세요!
			// 혹시 안 열리면 절대 경로("C:\\...")로 테스트 해보세요.
			INT_PTR result = (INT_PTR)ShellExecute(NULL, "open", "./Thanks.html", NULL, NULL, SW_SHOWNORMAL);

			if (result <= 32) {
				// 실패 시 메시지 박스로 알려줌 (디버깅용)
				MessageBox(NULL, "Thanks.html 파일을 찾을 수 없습니다!", "오류", MB_OK);
			}

			// 2. 🌟 [핵심] 메뉴로 가기 전에 버튼을 숨겨야 합니다!
			ShowWindow(m_hBtn, SW_HIDE);

			// 3. 씬 전환
			g_Mng.n_Chap = MENU;
		}
	}
}