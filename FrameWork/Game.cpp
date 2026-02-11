#include "Include.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init()
{
	mapMng.Init();
	//bird.Init();
	knight.Init();
	//player.Init();
	coll.Init();
	sound.Init();
	Gmanager.Init();
	// 데이타 베이스///////////////////
	sql.Init();
}

void Game::Draw()
{
	mapMng.Draw();
	//bird.Draw();
	knight.Draw();
	//player.Draw();
	coll.Draw();
	Gmanager.Draw();
	// 데이타 베이스///////////////////
	sql.Draw();
}

// Chap, 재정의 함수 호출
void Game::Update(double frame)
{
	//static int a = 0;
	//if(a == 0) 
	//{
	//	GameTime = GetTickCount();
	//	a = 1;
	//}
	static DWORD a = 0;

	if (GetTickCount64() - a > frame)
	{
		// 공부용
		Camera::GetInstance()->Update();
		key.Update();
		//bird.Update();
		knight.Update();
		//player.Update();
		coll.Update();
		// 입 맛에 맞게
		mapMng.Update(130);
		Gmanager.Update();
		// 데이타 베이스///////////////////
		// 입 맛에 맞게 (여기선 안쓰임..프레임 값이 필요 할때만.. 그냥 방법만...)
		sql.Update(frame+3000);

		a = GetTickCount64();
	}

}


void Game::OnMessage( MSG* msg )
{
	// 게임 씬에서 특수 키 처리 (ESC로 일시정지 등)
	switch (msg->message)
	{
	case WM_KEYDOWN:
		switch (msg->wParam)
		{
		case VK_ESCAPE:
			// 메뉴로 돌아가기 예시
			g_Mng.n_Chap = MENU;
			break;

		case '1':
			mapMng.ChangeMap(1);
			break;
		case '2':
			mapMng.ChangeMap(2);
			break;
		}
		break;
	}
}