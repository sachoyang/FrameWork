#include "Include.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init()
{
	coll.Init();
	CAM->Init();
	CAM->SetTarget(&knight.pos);
	mapMng.Init();
	//bird.Init();
	knight.Init();
	//player.Init();
	sound.Init();
	Gmanager.Init();
	EFFECT->Init();
	uiMng.Init();
	// 데이타 베이스///////////////////
	//sql.Init();
}

void Game::Draw()
{
	mapMng.Draw();
	//bird.Draw();
	knight.Draw();
	EFFECT->Draw();
	//player.Draw();
	coll.Draw();
	Gmanager.Draw();
	uiMng.Draw();
	// 데이타 베이스///////////////////
	//sql.Draw();
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
		//Camera::GetInstance()->Update();
		if (uiMng.m_bLargeMap) {
			uiMng.Update();
			return;
		}		

		key.Update();
		uiMng.Update(); // UI업데이트

		TIMEMGR->UpdateTime();
		// =======================================================
		// [타임 컨트롤 발동] 엔진이 멈추라고 하면 아래 인게임 로직은 건너뜀
		// =======================================================
		if (!TIMEMGR->IsTimeStopped())
		{
			// 시간의 영향을 받는 진짜 인게임 오브젝트들
			knight.Update();
			coll.Update();
			mapMng.Update(130);
			Gmanager.Update();

			EFFECT->Update();

			CAM->Update(); // 카메라 업데이트
		}
		////bird.Update();
		//knight.Update();
		////player.Update();
		//coll.Update();
		//// 입 맛에 맞게
		//mapMng.Update(130);
		//Gmanager.Update();

		//CAM->Update(); // 모든 오브젝트 움직이고 나서 업데이트되야 함
		// 데이타 베이스///////////////////
		// 입 맛에 맞게 (여기선 안쓰임..프레임 값이 필요 할때만.. 그냥 방법만...)
		//sql.Update(frame+3000);

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

		// =======================================================
		// 디버그 모드 단축키 (Page Up / Page Down)
		// =======================================================
		case VK_PRIOR: // Page Up 키 (이전 프리팹)
			mapMng.LoadDebugPrefab(mapMng.m_DebugPrefabID - 1);
			break;

		case VK_NEXT:  // Page Down 키 (다음 프리팹)
			mapMng.LoadDebugPrefab(mapMng.m_DebugPrefabID + 1);
			break;
		// =======================================================
		// 미니맵 (M)
		// =======================================================
			// M 키를 누르면 맵 확대 & 일시정지 토글
		case 'M':
			uiMng.m_bLargeMap = !uiMng.m_bLargeMap;
			break;

		case VK_OEM_2: // '/' 키
			coll.isDebugDraw = !coll.isDebugDraw;
			break;
		}
		break;
	}
}