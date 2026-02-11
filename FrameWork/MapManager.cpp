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

	// 2번 맵은 그냥 다른 이미지 하나만 띄워봅시다.
	// (이미지가 없다면 기존 거 재활용하거나 색깔만 바꿔서 Create)
	sprintf_s(FileName, "./resource/Img/map1/BG_Hades_1/BG_Hades_0003.tga");
	m_MapList[2].bgLayer[0].Create(FileName, false, 0);

	// -------------------------------------------------------
	// [맵 연결 설정] (나중에는 여기가 랜덤 알고리즘으로 대체됨)
	// -------------------------------------------------------
	// 1번 맵 초기화 (일단 다 0으로 막음)
	for (int i = 0; i < 4; i++) m_MapList[1].nextMapID[i] = 0;

	// 1번 맵의 [오른쪽] -> 2번 맵
	m_MapList[1].nextMapID[DIR_RIGHT] = 2;

	// 2번 맵 초기화
	for (int i = 0; i < 4; i++) m_MapList[2].nextMapID[i] = 0;

	// 2번 맵의 [왼쪽] -> 1번 맵 (다시 돌아올 수 있게)
	m_MapList[2].nextMapID[DIR_LEFT] = 1;

	// [초기 시작] 1번 맵으로 시작!
	ChangeMap(1);
}

void MapManager::ChangeMap(int mapID)
{
	// 범위 체크
	if (mapID < 1 || mapID > 9) return;

	// 현재 맵 포인터 교체
	m_pCurrentMapChunk = &m_MapList[mapID];

	// (나중에 여기서 플레이어 위치 재설정이나 몬스터 리젠 등 처리)
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