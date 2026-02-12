#include "Include.h"

MapManager mapMng;


MapManager::MapManager()
{
	m_Stage = 1;
	m_pCurrentMapChunk = nullptr;
}

MapManager::~MapManager()
{
}

void MapManager::Init()
{
	srand((unsigned int)time(NULL)); // 랜덤 시드 초기화
	/*int i,j;
	char FileName[256];
	
	for(i = 0; i<6; i++ )
	{
		sprintf_s(FileName, "./resource/Img/map1/BG_Hades_1/BG_Hades_%04d.tga", i + 1);
		m_MapImg1_1[i].Create( FileName ,false,D3DCOLOR_XRGB(0,0,0));
		
	}

	j = 1;
	for(i = 0; i<45; i++ )
	{
		sprintf_s(FileName, "./resource/Img/map1/BG_Hades_2/BG_Hades_0002_%06d.tga", j);
		m_MapImg1_2[i].Create(FileName, false, D3DCOLOR_XRGB(0, 0, 0));
		j+=2;
		
	}*/
#pragma region MapData_Init
	char FileName[256];
	// -------------------------------------------------------
	// [1번 맵] - 고정 시작 맵 (기존에 쓰던 그 맵)
	// -------------------------------------------------------
	m_MapList[1].id = 1;
	m_MapList[1].layerCount = 2; // 배경 2장 쓴다고 가정

	// 배경 1 (가장 뒤)
	sprintf_s(FileName, "./resource/Img/map1/BG_Hades_1/BG_Hades_0001.tga");
	m_MapList[1].bgLayer[0].Create(FileName, false, 0);

	// 배경 2 (그 앞)
	sprintf_s(FileName, "./resource/Img/map1/BG_Hades_1/BG_Hades_0002.tga");
	m_MapList[1].bgLayer[1].Create(FileName, false, 0);

	// -------------------------------------------------------
	// [2번 맵] - 테스트용 다른 맵 (배경 색이나 이미지를 다르게)
	// -------------------------------------------------------
	m_MapList[2].id = 2;
	m_MapList[2].layerCount = 1;

	// 2번 맵은 그냥 다른 이미지 하나
	sprintf_s(FileName, "./resource/Img/map1/BG_Hades_1/BG_Hades_0003.tga");
	m_MapList[2].bgLayer[0].Create(FileName, false, 0);

	// -------------------------------------------------------
	// [3번 맵] 
	// -------------------------------------------------------
	m_MapList[3].id = 3;
	m_MapList[3].layerCount = 1;
	sprintf_s(FileName, "./resource/Img/map1/Ch1_maps/map01.png");
	m_MapList[3].bgLayer[0].Create(FileName, false, 0);
	// -------------------------------------------------------
	// [4번 맵] 
	// -------------------------------------------------------
	m_MapList[4].id = 2;
	m_MapList[4].layerCount = 1;
	sprintf_s(FileName, "./resource/Img/map1/Ch1_maps/map02.png");
	m_MapList[4].bgLayer[0].Create(FileName, false, 0);

#pragma endregion

//#pragma region MapData 연결	설정
//	// -------------------------------------------------------
//	// [맵 연결 설정] (나중에는 여기가 랜덤 알고리즘으로 대체됨)
//	// -------------------------------------------------------
//	// 1번 맵 초기화 (일단 다 0으로 막음)
//	for (int i = 0; i < 4; i++) m_MapList[1].nextMapID[i] = 0;
//
//	// 1번 맵의 [오른쪽] -> 2번 맵
//	m_MapList[1].nextMapID[DIR_RIGHT] = 2;
//
//	// 2번 맵 초기화
//	for (int i = 0; i < 4; i++) m_MapList[2].nextMapID[i] = 0;
//
//	// 2번 맵의 [왼쪽] -> 1번 맵 (다시 돌아올 수 있게)
//	m_MapList[2].nextMapID[DIR_LEFT] = 1;
//
////#pragma endregion

	// [랜덤 맵 생성] 
	CreateRandomMap();

	// [초기 시작] 1번 맵으로 시작!
	ChangeMap(1);
}

void MapManager::CreateRandomMap()
{
	// [1] 초기화: 모든 맵의 연결 정보를 0(막힘)으로 리셋
	for (int i = 1; i <= 10; i++)
	{
		for (int j = 0; j < 5; j++) m_MapList[i].nextMapID[j] = 0;
	}

	// [2] 가상의 격자판 만들기 (20x20 크기)
	// 0이면 빈칸, 1이상이면 맵 ID
	int grid[20][20] = { 0, };

	// [3] 1번 맵을 정중앙에 배치
	int startX = 10;
	int startY = 10;
	grid[startY][startX] = 1;

	// [4] 2번 맵부터 10번 맵까지 순서대로 붙이기 (Random Walk 변형)
	int currentMapCount = 1;
	int maxMapCount = 4; // 일단 5개만 만들어봅시다 (최대 10개)

	while (currentMapCount < maxMapCount)
	{
		// 이미 배치된 맵 중 하나를 랜덤으로 고름 (거기서 가지를 뻗기 위해)
		int targetX = startX;
		int targetY = startY;

		// (간단하게 1번 방 근처에서 계속 뻗어나가는 방식)
		// 더 복잡한 미로를 원하면 '배치된 방 목록'을 리스트로 관리해서 뽑아야 함.
		// 여기서는 "랜덤 워크" 방식으로 빈 공간 찾을 때까지 이동

		int dir = rand() % 4 + 1; // 1~4 랜덤 방향
		int nextX = targetX;
		int nextY = targetY;

		if (dir == DIR_UP)    nextY--;
		else if (dir == DIR_DOWN)  nextY++;
		else if (dir == DIR_LEFT)  nextX--;
		else if (dir == DIR_RIGHT) nextX++;

		// 그 자리가 비어있으면 새 맵 배치!
		if (grid[nextY][nextX] == 0)
		{
			currentMapCount++;
			grid[nextY][nextX] = currentMapCount; // 2번, 3번... 맵 ID 할당

			// 여기서 탐색 위치를 갱신해줌 (다음 맵은 이 맵 옆에 붙을 수도 있게)
			startX = nextX;
			startY = nextY;
		}
	}

	// [5] 격자판을 보고 실제 연결(Link) 정보 입력
	for (int y = 1; y < 19; y++)
	{
		for (int x = 1; x < 19; x++)
		{
			int myID = grid[y][x];
			if (myID == 0) continue; // 맵 없음

			// 상(Up) 확인 -> grid[y-1][x]
			if (grid[y - 1][x] != 0)
				m_MapList[myID].nextMapID[DIR_UP] = grid[y - 1][x];

			// 하(Down) 확인 -> grid[y+1][x]
			if (grid[y + 1][x] != 0)
				m_MapList[myID].nextMapID[DIR_DOWN] = grid[y + 1][x];

			// 좌(Left) 확인 -> grid[y][x-1]
			if (grid[y][x - 1] != 0)
				m_MapList[myID].nextMapID[DIR_LEFT] = grid[y][x - 1];

			// 우(Right) 확인 -> grid[y][x+1]
			if (grid[y][x + 1] != 0)
				m_MapList[myID].nextMapID[DIR_RIGHT] = grid[y][x + 1];
		}
	}
}

void MapManager::ChangeMap(int mapID)
{
	if (mapID < 1 || mapID > 10) return;

	m_pCurrentMapChunk = &m_MapList[mapID];
	// [필수] 맵 바뀔 때 상태 리셋
	//knight.isDashing = false;
	//knight.gravity = 0;
	//knight.isMove = false;
	// 1. Clear existing walls
	coll.ClearWalls();

	RECT rc;
	int thickness = 100; // Wall thickness
	int floorY = SCREEN_HEIGHT - thickness; // The top Y of the floor (e.g., 500 if height is 600)

	// =============================================================
	// 1. FLOOR (Bottom) Handling
	// =============================================================
	if (m_pCurrentMapChunk->nextMapID[DIR_DOWN] == 0)
	{
		// [No Path] Solid floor
		// Top: 500, Bottom: 600 (or slightly more to catch falling)
		SetRect(&rc, 0, floorY, SCREEN_WITH, SCREEN_HEIGHT + 50);
		coll.AddWall(rc);
	}
	else
	{
		// [Path Exists] Hole in the middle
		int holeSize = 200;
		int midX = SCREEN_WITH / 2;

		// Left Floor Piece (0 to 300)
		SetRect(&rc, 0, floorY, midX - (holeSize / 2), SCREEN_HEIGHT + 50);
		coll.AddWall(rc);

		// Right Floor Piece (500 to 800)
		SetRect(&rc, midX + (holeSize / 2), floorY, SCREEN_WITH, SCREEN_HEIGHT + 50);
		coll.AddWall(rc);
	}

	// =============================================================
	// 2. CEILING (Top) Handling
	// =============================================================
	if (m_pCurrentMapChunk->nextMapID[DIR_UP] == 0)
	{
		// [No Path] Solid ceiling
		SetRect(&rc, 0, -50, SCREEN_WITH, thickness); // -50 to 100
		coll.AddWall(rc);
	}
	// Else: No ceiling wall (allow jumping up)

	// =============================================================
	// 3. LEFT WALL Handling
	// =============================================================
	if (m_pCurrentMapChunk->nextMapID[DIR_LEFT] == 0)
	{
		// [No Path] Solid left wall
		SetRect(&rc, -50, 0, thickness, SCREEN_HEIGHT);
		coll.AddWall(rc);
	}
	else
	{
		// [Path Exists] Doorway (Wall only on top part)
		// Adjust 300 to control door height
		SetRect(&rc, -50, 0, thickness, floorY - 200);
		coll.AddWall(rc);
	}

	// =============================================================
	// 4. RIGHT WALL Handling
	// =============================================================
	if (m_pCurrentMapChunk->nextMapID[DIR_RIGHT] == 0)
	{
		// [No Path] Solid right wall
		SetRect(&rc, SCREEN_WITH - thickness, 0, SCREEN_WITH + 50, SCREEN_HEIGHT);
		coll.AddWall(rc);
	}
	else
	{
		// [Path Exists] Doorway
		SetRect(&rc, SCREEN_WITH - thickness, 0, SCREEN_WITH + 50, floorY - 200);
		coll.AddWall(rc);
	}
}

void MapManager::Update(double frame)
{
	/*if(GetTickCount64() - m_MapImg1_1_ani1 > frame)
	{
		if(m_Stage==1)
		{
			m_MapImg1_1_ani1Count++;
			if(m_MapImg1_1_ani1Count > 44) m_MapImg1_1_ani1Count = 0;
		}

		m_MapImg1_1_ani1 = GetTickCount64();
	}*/
	if (m_pCurrentMapChunk == nullptr) return;

	// 1. 오른쪽으로 나갈 때 (플레이어 x > 화면너비)
	if (knight.pos.x > SCREEN_WITH)
	{
		// 갈 곳이 있는지 확인
		int nextMap = m_pCurrentMapChunk->nextMapID[DIR_RIGHT];

		if (nextMap != 0) // 연결된 맵이 있다!
		{
			ChangeMap(nextMap);     // 맵 교체
			knight.pos.x = 50.0f;   // 플레이어를 왼쪽 끝으로 이동 (입장 연출)
		}
		else // 막힌 길이다!
		{
			knight.pos.x = SCREEN_WITH; // 못 나가게 막음
		}
	}

	// 2. 왼쪽으로 나갈 때 (플레이어 x < 0)
	else if (knight.pos.x < 0)
	{
		int nextMap = m_pCurrentMapChunk->nextMapID[DIR_LEFT];

		if (nextMap != 0)
		{
			ChangeMap(nextMap);
			knight.pos.x = SCREEN_WITH - 50.0f; // 오른쪽 끝에서 등장
		}
		else
		{
			knight.pos.x = 0; // 막힘
		}
	}

	// [추가] 3. 위로 올라갈 때 (플레이어 y < 0)
	if (knight.pos.y < 0)
	{
		int nextMap = m_pCurrentMapChunk->nextMapID[DIR_UP];
		if (nextMap != 0)
		{
			ChangeMap(nextMap);
			// 아래쪽 끝에서 솟아오름
			knight.pos.y = SCREEN_HEIGHT - 100.0f;
		}
		else
		{
			knight.pos.y = 0; // 천장에 머리 쿵
		}
	}

	// [추가] 4. 아래로 떨어질 때 (플레이어 y > 화면높이)
	else if (knight.pos.y > SCREEN_HEIGHT)
	{
		int nextMap = m_pCurrentMapChunk->nextMapID[DIR_DOWN];
		if (nextMap != 0)
		{
			ChangeMap(nextMap);
			// 위쪽 끝에서 떨어짐
			knight.pos.y = 50.0f;
		}
		else
		{
			// 낭떠러지? (게임 오버 처리하거나 못 가게 막음)
			// 여기서는 일단 바닥에 걸치게
			knight.pos.y = SCREEN_HEIGHT;
			knight.grounded = true; // 땅 밟은 판정
		}
	}
}

void MapManager::Draw()
{
	/*if(m_Stage==1)
	{
		m_MapImg1_1[0].Render(posX, posY, 0, 1, 1);
		m_MapImg1_1[1].Render(posX, posY, 0, 1, 1);
		m_MapImg1_1[3].Render(posX+270, posY+70, 0, 1, 1);
		m_MapImg1_2[m_MapImg1_1_ani1Count].Render(posX, posY, 0, 1.5, 1.5);
		m_MapImg1_1[2].Render(posX+190, posY, 0, 1, 1);
		m_MapImg1_1[4].Render(posX+900, posY, 0, 1, 1);
		m_MapImg1_1[5].Render(posX, posY, 0, 1, 1);

	}*/

	if (m_pCurrentMapChunk == nullptr) return;

	// 현재 맵의 모든 레이어 그리기
	for (int i = 0; i < m_pCurrentMapChunk->layerCount; i++)
	{
		// 좌표는 (0,0)에 그립니다. (카메라 적용 전)
		m_pCurrentMapChunk->bgLayer[i].Render(0, 0, 0, 1, 1);
	}
}