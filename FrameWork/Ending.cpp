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
	// 엔딩 씬이 실행 중인데 버튼이 숨어있다면? -> 보여줘라!
	if (m_hBtn != NULL && !IsWindowVisible(m_hBtn))
	{
		ShowWindow(m_hBtn, SW_SHOW);
	}
}

void Ending::Draw()
{
	// 배경 그리기
	m_Bg.Render(0, 0, 0, 1, 1);
}

void Ending::OnMessage(MSG* msg)
{
	if (msg->message == WM_COMMAND)
	{
		if (LOWORD(msg->wParam) == ID_BTN_THANKS)
		{
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