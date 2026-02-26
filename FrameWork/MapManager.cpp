#include "Include.h"
#include <vector>

MapManager mapMng;

int MapManager::GetRoomGridX(int roomID) {
	for (int y = 0; y < 6; y++)
		for (int x = 0; x < 6; x++)
			if (m_Grid[y][x] == roomID) return x;
	return 0;
}
int MapManager::GetRoomGridY(int roomID) {
	for (int y = 0; y < 6; y++)
		for (int x = 0; x < 6; x++)
			if (m_Grid[y][x] == roomID) return y;
	return 0;
}

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

	// 프리펩 초기화 먼저 진행 후 맵 생성 (맵 생성 시 프리펩 정보가 필요)
	InitPrefabs();

	// [랜덤 맵 생성] 
	CreateRandomMap();

	// [초기 시작] 1번 맵으로 시작!
	ChangeMap(1);
}

//void MapManager::InitPrefabs()
//{
//	RECT rc;
//	int thickness = 100;
//	// ====================================================================
//	// [프리팹 1번] 위(UP)만 뚫린 방
//	// 사이즈: 1칸 x 2칸 (1280 x 1600)
//	// 지형: 바닥부터 꼭대기 출구까지 8개의 발판을 징검다리처럼 밟고 올라감
//	// ====================================================================
//	{
//		int id = DOOR_UP; // 1
//		m_Prefabs[id].typeID = id;
//		m_Prefabs[id].gridW = 1; // 1칸
//		m_Prefabs[id].gridH = 2; // 2칸
//		m_Prefabs[id].width = SCREEN_WITH;         // 1280
//		m_Prefabs[id].height = SCREEN_HEIGHT * 2;  // 1600
//		m_Prefabs[id].layerCount = 1;
//		// 배경 이미지 할당 (파일 이름은 나중에 맞게 수정)
//		// m_Prefabs[id].bgLayer[0].Create("...", false, 0); 
//
//		int MW = m_Prefabs[id].width;
//		int MH = m_Prefabs[id].height;
//		int floorY = MH - thickness; // 1500
//
//		// 1. 외곽선 (바닥 막힘, 좌우 막힘)
//		SetRect(&rc, 0, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, -50, 0, thickness, MH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - thickness, 0, MW + 50, MH); m_Prefabs[id].walls.push_back(rc);
//
//		// 2. 천장 (가운데 300px 뚫림)
//		SetRect(&rc, 0, -50, MW / 2 - 150, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 150, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
//
//		// 3. 내부 플랫폼 (점프 높이 200~250 간격으로 지그재그 배치)
//		int pH = 30; // 두께
//		SetRect(&rc, 150, 1250, 350, 1250 + pH); m_Prefabs[id].walls.push_back(rc); // 1층(좌)
//		SetRect(&rc, 500, 1050, 800, 1050 + pH); m_Prefabs[id].walls.push_back(rc); // 2층(중)
//		SetRect(&rc, 900, 850, 1150, 850 + pH);  m_Prefabs[id].walls.push_back(rc); // 3층(우)
//		SetRect(&rc, 500, 650, 700, 650 + pH);   m_Prefabs[id].walls.push_back(rc); // 4층(중)
//		SetRect(&rc, 100, 450, 400, 450 + pH);   m_Prefabs[id].walls.push_back(rc); // 5층(좌)
//		SetRect(&rc, 450, 250, 800, 250 + pH);   m_Prefabs[id].walls.push_back(rc); // 6층(중앙 출구 밑)
//	}
//
//	// ====================================================================
//	// [프리팹 2번] 아래(DOWN)만 뚫린 방
//	// 사이즈: 1칸 x 1칸 (1280 x 800)
//	// 지형: 바닥 가운데 낭떠러지만 있는 좁은 방 (떨어지면 다음 맵)
//	// ====================================================================
//	{
//		int id = DOOR_DOWN; // 2
//		m_Prefabs[id].typeID = id;
//		m_Prefabs[id].gridW = 1;
//		m_Prefabs[id].gridH = 1;
//		m_Prefabs[id].width = SCREEN_WITH;
//		m_Prefabs[id].height = SCREEN_HEIGHT;
//		m_Prefabs[id].layerCount = 1;
//
//		int MW = m_Prefabs[id].width;
//		int MH = m_Prefabs[id].height;
//		int floorY = MH - thickness; // 700
//
//		// 1. 외곽선 (천장 막힘, 좌우 막힘)
//		SetRect(&rc, 0, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, -50, 0, thickness, MH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - thickness, 0, MW + 50, MH); m_Prefabs[id].walls.push_back(rc);
//
//		// 2. 바닥 (가운데 300px 뚫림)
//		SetRect(&rc, 0, floorY, MW / 2 - 150, MH + 50); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 150, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
//
//		// 3. 구멍 위를 안전하게 건널 수 있는 부서진 다리 (플랫폼)
//		SetRect(&rc, 200, 500, 450, 530); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 850, 500, 1100, 530); m_Prefabs[id].walls.push_back(rc);
//	}
//
//	// ====================================================================
//	// [프리팹 3번] 위, 아래(UP, DOWN) 뚫린 방
//	// 사이즈: 2칸 x 2칸 (2560 x 1600) - 거대한 웅덩이 형태
//	// 출구 위치: 상단은 정중앙, 하단도 정중앙
//	// 지형: 양옆으로 나뉘어진 거대 발판과 중앙 공중 섬들
//	// ====================================================================
//	{
//		int id = DOOR_UP | DOOR_DOWN; // 3
//		m_Prefabs[id].typeID = id;
//		m_Prefabs[id].gridW = 2; // 2칸
//		m_Prefabs[id].gridH = 2; // 2칸
//		m_Prefabs[id].width = SCREEN_WITH * 2;        // 2560
//		m_Prefabs[id].height = SCREEN_HEIGHT * 2;     // 1600
//		m_Prefabs[id].layerCount = 1;
//
//		int MW = m_Prefabs[id].width;
//		int MH = m_Prefabs[id].height;
//		int floorY = MH - thickness; // 1500
//
//		// 1. 좌우 벽 꽉 막음
//		SetRect(&rc, -50, 0, thickness, MH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - thickness, 0, MW + 50, MH); m_Prefabs[id].walls.push_back(rc);
//
//		// 2. 천장(뚫림) & 바닥(뚫림)
//		SetRect(&rc, 0, -50, MW / 2 - 150, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 150, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
//
//		SetRect(&rc, 0, floorY, MW / 2 - 200, MH + 50); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 200, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
//
//		// 3. 지형: 거대 협곡 (양쪽 벽에 붙은 큰 발판들)
//		SetRect(&rc, 0, 1100, 600, 1150); m_Prefabs[id].walls.push_back(rc); // 좌 하단 거대 발판
//		SetRect(&rc, MW - 600, 800, MW, 850); m_Prefabs[id].walls.push_back(rc); // 우 중단 거대 발판
//		SetRect(&rc, 0, 500, 500, 550); m_Prefabs[id].walls.push_back(rc);   // 좌 상단 거대 발판
//
//		// 4. 중앙 공중 섬들 (다리 역할)
//		SetRect(&rc, 800, 1300, 1050, 1330); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1500, 1050, 1750, 1080); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 900, 700, 1200, 730); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 - 150, 300, MW / 2 + 150, 330); m_Prefabs[id].walls.push_back(rc); // 출구 바로 밑
//	}
//
//	// ====================================================================
//	// [프리팹 4번] 왼쪽(LEFT)만 뚫린 방
//	// 사이즈: 2칸 x 2칸 (2560 x 1600)
//	// 출구 위치: "좌측 하단"에 문이 있음 (y = 800 ~ 1600 사이)
//	// 지형: 거대한 피라미드 계단이 중앙에 위치함
//	// ====================================================================
//	{
//		int id = DOOR_LEFT; // 4
//		m_Prefabs[id].typeID = id;
//		m_Prefabs[id].gridW = 2; // 2칸
//		m_Prefabs[id].gridH = 2; // 2칸
//		m_Prefabs[id].width = SCREEN_WITH * 2;  // 2560
//		m_Prefabs[id].height = SCREEN_HEIGHT * 2; // 1600
//		m_Prefabs[id].layerCount = 1;
//
//		int MW = m_Prefabs[id].width;
//		int MH = m_Prefabs[id].height;
//		int floorY = MH - thickness; // 1500
//
//		// 1. 천장, 바닥, 우측 벽 막힘
//		SetRect(&rc, 0, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 0, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - thickness, 0, MW + 50, MH); m_Prefabs[id].walls.push_back(rc);
//
//		// 2. 왼쪽 벽 (뚫리긴 했는데 '하단 1칸' 쪽만 문을 뚫습니다!)
//		// 윗부분 벽 막음 (y = 0 ~ 800)
//		SetRect(&rc, -50, 0, thickness, SCREEN_HEIGHT); m_Prefabs[id].walls.push_back(rc);
//		// 아랫부분 벽 (문 높이인 바닥에서 200px 위까지만 막음)
//		SetRect(&rc, -50, SCREEN_HEIGHT, thickness, floorY - 200); m_Prefabs[id].walls.push_back(rc);
//
//		// 3. 지형: 거대한 중앙 피라미드 제단
//		SetRect(&rc, 700, 1300, 1900, floorY); m_Prefabs[id].walls.push_back(rc); // 1단 (매우 넓음)
//		SetRect(&rc, 850, 1100, 1750, 1300); m_Prefabs[id].walls.push_back(rc);   // 2단
//		SetRect(&rc, 1000, 900, 1600, 1100); m_Prefabs[id].walls.push_back(rc);   // 3단
//		SetRect(&rc, 1150, 700, 1450, 900);  m_Prefabs[id].walls.push_back(rc);   // 꼭대기
//
//		// 4. 꼭대기 위 공중에 떠있는 장식용/도전용 발판
//		SetRect(&rc, 300, 800, 500, 830); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - 500, 600, MW - 300, 630); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 - 100, 350, MW / 2 + 100, 380); m_Prefabs[id].walls.push_back(rc);
//	}
//
//	// ====================================================================
//	// [프리팹 5번] 위, 왼쪽 (UP, LEFT) 뚫린 방
//	// 조합: DOOR_UP(1) | DOOR_LEFT(4) = 5
//	// 사이즈: 2칸 x 2칸 (2560 x 1600)
//	// 출구: 좌측은 '하단', 상단은 '중앙'
//	// 지형: 왼쪽 아래에서 들어와서 오른쪽으로 크게 돌며 위로 올라가는 나선형 구조
//	// ====================================================================
//	{
//		int id = DOOR_UP | DOOR_LEFT; // 5
//		m_Prefabs[id].typeID = id;
//		m_Prefabs[id].gridW = 2;
//		m_Prefabs[id].gridH = 2;
//		m_Prefabs[id].width = SCREEN_WITH * 2;   // 2560
//		m_Prefabs[id].height = SCREEN_HEIGHT * 2; // 1600
//		m_Prefabs[id].layerCount = 1;
//
//		int MW = m_Prefabs[id].width;
//		int MH = m_Prefabs[id].height;
//		int floorY = MH - thickness; // 1500
//
//		// 1. 외곽선 (바닥 막힘, 우측 벽 막힘)
//		SetRect(&rc, 0, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - thickness, 0, MW + 50, MH); m_Prefabs[id].walls.push_back(rc);
//
//		// 2. 천장 뚫기 (중앙)
//		SetRect(&rc, 0, -50, MW / 2 - 150, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 150, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
//
//		// 3. 왼쪽 벽 뚫기 (좌측 하단 1칸만)
//		SetRect(&rc, -50, 0, thickness, SCREEN_HEIGHT); m_Prefabs[id].walls.push_back(rc); // 상단 막음
//		SetRect(&rc, -50, SCREEN_HEIGHT, thickness, floorY - 200); m_Prefabs[id].walls.push_back(rc); // 하단 뚫음
//
//		// 4. 지형: 우회전 나선형 계단
//		int pH = 30;
//		SetRect(&rc, 500, 1300, 900, 1300 + pH); m_Prefabs[id].walls.push_back(rc);   // 1층
//		SetRect(&rc, 1100, 1100, 1600, 1100 + pH); m_Prefabs[id].walls.push_back(rc); // 2층
//		SetRect(&rc, 1800, 900, 2400, 900 + pH); m_Prefabs[id].walls.push_back(rc);   // 3층 (우측 끝)
//		SetRect(&rc, 1400, 650, 1700, 650 + pH); m_Prefabs[id].walls.push_back(rc);   // 4층 (다시 왼쪽으로)
//		SetRect(&rc, 900, 450, 1200, 450 + pH); m_Prefabs[id].walls.push_back(rc);    // 5층
//		SetRect(&rc, MW / 2 - 150, 250, MW / 2 + 150, 250 + pH); m_Prefabs[id].walls.push_back(rc); // 6층 (출구 밑)
//	}
//
//	// ====================================================================
//	// [프리팹 6번] 아래, 왼쪽 (DOWN, LEFT) 뚫린 방
//	// 조합: DOOR_DOWN(2) | DOOR_LEFT(4) = 6
//	// 사이즈: 1칸 x 2칸 (1280 x 1600) 세로 긴 방
//	// 출구: 좌측은 '상단', 하단은 '중앙'
//	// 지형: 왼쪽 위에서 들어와 조심스럽게 아래로 떨어지며 내려가야 하는 절벽
//	// ====================================================================
//	{
//		int id = DOOR_DOWN | DOOR_LEFT; // 6
//		m_Prefabs[id].typeID = id;
//		m_Prefabs[id].gridW = 1;
//		m_Prefabs[id].gridH = 2;
//		m_Prefabs[id].width = SCREEN_WITH;       // 1280
//		m_Prefabs[id].height = SCREEN_HEIGHT * 2; // 1600
//		m_Prefabs[id].layerCount = 1;
//
//		int MW = m_Prefabs[id].width;
//		int MH = m_Prefabs[id].height;
//		int floorY = MH - thickness; // 1500
//
//		// 1. 외곽선 (천장 막힘, 우측 벽 막힘)
//		SetRect(&rc, 0, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - thickness, 0, MW + 50, MH); m_Prefabs[id].walls.push_back(rc);
//
//		// 2. 바닥 뚫기 (중앙)
//		SetRect(&rc, 0, floorY, MW / 2 - 150, MH + 50); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 150, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
//
//		// 3. 왼쪽 벽 뚫기 (🌟이번엔 '좌측 상단'에 문이 있습니다!)
//		// 상단 문 뚫음 (y=0 ~ 600)
//		SetRect(&rc, -50, 0, thickness, 600); m_Prefabs[id].walls.push_back(rc);
//		// 하단 막음 (y=800 ~ 끝)
//		SetRect(&rc, -50, 800, thickness, MH); m_Prefabs[id].walls.push_back(rc);
//
//		// 4. 지형: 떨어지면서 밟아야 하는 절벽형 구조
//		SetRect(&rc, 0, 750, 400, 800); m_Prefabs[id].walls.push_back(rc); // 들어오자마자 밟는 곳
//		SetRect(&rc, 700, 950, 1000, 1000); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 200, 1200, 500, 1250); m_Prefabs[id].walls.push_back(rc);
//	}
//
//	// ====================================================================
//	// [프리팹 7번] 위, 아래, 왼쪽 (UP, DOWN, LEFT) 뚫린 방
//	// 조합: 1 | 2 | 4 = 7
//	// 사이즈: 2칸 x 2칸 (2560 x 1600)
//	// 출구: 상/하단은 '중앙', 좌측은 '하단'
//	// 지형: 거대한 십자 교차로 느낌. 아슬아슬한 구름 다리들.
//	// ====================================================================
//	{
//		int id = DOOR_UP | DOOR_DOWN | DOOR_LEFT; // 7
//		m_Prefabs[id].typeID = id;
//		m_Prefabs[id].gridW = 2;
//		m_Prefabs[id].gridH = 2;
//		m_Prefabs[id].width = SCREEN_WITH * 2;
//		m_Prefabs[id].height = SCREEN_HEIGHT * 2;
//		m_Prefabs[id].layerCount = 1;
//
//		int MW = m_Prefabs[id].width;
//		int MH = m_Prefabs[id].height;
//		int floorY = MH - thickness;
//
//		// 1. 우측 벽(막힘)
//		SetRect(&rc, MW - thickness, 0, MW + 50, MH); m_Prefabs[id].walls.push_back(rc);
//
//		// 2. 천장 & 바닥 뚫기 (중앙)
//		SetRect(&rc, 0, -50, MW / 2 - 150, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 150, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 0, floorY, MW / 2 - 150, MH + 50); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 150, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
//
//		// 3. 왼쪽 벽 뚫기 (하단)
//		SetRect(&rc, -50, 0, thickness, SCREEN_HEIGHT); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, -50, SCREEN_HEIGHT, thickness, floorY - 200); m_Prefabs[id].walls.push_back(rc);
//
//		// 4. 지형: 거대 교차로 다리
//		int pH = 30;
//		// 메인 중앙 다리 (좌우로 길게)
//		SetRect(&rc, 400, 1300, MW - 400, 1350); m_Prefabs[id].walls.push_back(rc);
//		// 위로 올라가는 징검다리
//		SetRect(&rc, MW / 2 + 300, 1050, MW / 2 + 600, 1050 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 - 400, 800, MW / 2 - 100, 800 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 100, 550, MW / 2 + 400, 550 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 - 150, 300, MW / 2 + 150, 300 + pH); m_Prefabs[id].walls.push_back(rc);
//	}
//
//	// ====================================================================
//	// [프리팹 8번] 오른쪽만 뚫린 방 (시작 방으로 쓸 예정)
//	// 조합: DOOR_RIGHT (8)
//	// ====================================================================
//	{
//		int id = DOOR_RIGHT; // id = 8
//
//		m_Prefabs[id].typeID = id;
//
//		m_Prefabs[id].gridW = 1;
//		m_Prefabs[id].gridH = 1;
//
//		m_Prefabs[id].width = SCREEN_WITH;   // 1280 고정
//		m_Prefabs[id].height = SCREEN_HEIGHT;  // 800 고정
//		m_Prefabs[id].layerCount = 1;
//
//		// 배경 이미지 로드 (파일 경로는 임시로 적어둠. 나중에 맞는 이미지로 교체)
//		char FileName[256];
//		sprintf_s(FileName, "./resource/Img/map1/Ch1_maps/map01.png");
//		m_Prefabs[id].bgLayer[0].Create(FileName, false, 0);
//
//		// -- 이 방의 고유 콜라이더(벽) 찍어내기 --
//		int MW = m_Prefabs[id].width;
//		int MH = m_Prefabs[id].height;
//		int floorY = MH - thickness;
//
//		// 1. 바닥 (아래 안 뚫렸으니 통짜 바닥)
//		SetRect(&rc, 0, floorY, MW, MH + 50);
//		m_Prefabs[id].walls.push_back(rc);
//		// 2. 천장 (위 안 뚫렸으니 통짜 천장)
//		SetRect(&rc, 0, -50, MW, thickness);
//		m_Prefabs[id].walls.push_back(rc);
//		// 3. 왼쪽 벽 (왼쪽 안 뚫렸으니 통짜 벽)
//		SetRect(&rc, -50, 0, thickness, MH);
//		m_Prefabs[id].walls.push_back(rc);
//		// 4. 오른쪽 벽 (오른쪽 뚫렸음! -> 문 높이(floorY - 200)까지만 벽 생성)
//		SetRect(&rc, MW - thickness, 0, MW + 50, floorY - 200);
//		m_Prefabs[id].walls.push_back(rc);
//	}
//	// ====================================================================
//	// [프리팹 9번] 위, 오른쪽 (UP, RIGHT) 뚫린 방
//	// 조합: DOOR_UP(1) | DOOR_RIGHT(8) = 9
//	// 사이즈: 1칸 x 2칸 (1280 x 1600)
//	// 출구: 상단은 '중앙', 우측은 '하단'
//	// 지형: 우측에서 들어와 좁은 탑을 지그재그로 기어올라가는 구조
//	// ====================================================================
//	{
//		int id = DOOR_UP | DOOR_RIGHT; // 9
//		m_Prefabs[id].typeID = id;
//		m_Prefabs[id].gridW = 1;
//		m_Prefabs[id].gridH = 2;
//		m_Prefabs[id].width = SCREEN_WITH;
//		m_Prefabs[id].height = SCREEN_HEIGHT * 2;
//		m_Prefabs[id].layerCount = 1;
//
//		int MW = m_Prefabs[id].width;
//		int MH = m_Prefabs[id].height;
//		int floorY = MH - thickness;
//
//		// 1. 외곽선 (바닥 막힘, 좌측 벽 막힘)
//		SetRect(&rc, 0, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, -50, 0, thickness, MH); m_Prefabs[id].walls.push_back(rc);
//
//		// 2. 천장 뚫기 (중앙)
//		SetRect(&rc, 0, -50, MW / 2 - 150, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 150, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
//
//		// 3. 오른쪽 벽 뚫기 (우측 하단)
//		SetRect(&rc, MW - thickness, 0, MW + 50, SCREEN_HEIGHT); m_Prefabs[id].walls.push_back(rc); // 상단 막음
//		SetRect(&rc, MW - thickness, SCREEN_HEIGHT, MW + 50, floorY - 200); m_Prefabs[id].walls.push_back(rc); // 하단 뚫음
//
//		// 4. 지형: 탑 등반 구조
//		int pH = 30;
//		SetRect(&rc, MW - 400, 1250, MW - 100, 1250 + pH); m_Prefabs[id].walls.push_back(rc); // 1단 (우)
//		SetRect(&rc, 100, 1000, 400, 1000 + pH); m_Prefabs[id].walls.push_back(rc);           // 2단 (좌)
//		SetRect(&rc, MW / 2 - 100, 750, MW / 2 + 200, 750 + pH); m_Prefabs[id].walls.push_back(rc); // 3단 (중)
//		SetRect(&rc, 150, 500, 450, 500 + pH); m_Prefabs[id].walls.push_back(rc);             // 4단 (좌)
//		SetRect(&rc, MW / 2 - 150, 250, MW / 2 + 150, 250 + pH); m_Prefabs[id].walls.push_back(rc); // 5단 (출구 밑)
//	}
//	// 필요하다면 중앙에 작은 장식용 발판 하나 추가 (예시)
//	// SetRect(&rc, MW/2 - 100, floorY - 150, MW/2 + 100, floorY - 120);
//	// m_Prefabs[id].walls.push_back(rc);
//	
//	// ====================================================================
//	// [프리팹 10번] 아래, 오른쪽 (DOWN, RIGHT) 뚫린 방
//	// 조합: DOOR_DOWN(2) | DOOR_RIGHT(8) = 10
//	// 사이즈: 2칸 x 1칸 (2560 x 800) 가로 긴 방
//	// 출구: 하단은 '중앙', 우측은 '하단'
//	// 지형: 거대한 낭떠러지를 밧줄 다리처럼 듬성듬성 건너가는 지형
//	// ====================================================================
//	{
//		int id = DOOR_DOWN | DOOR_RIGHT; // 10
//		m_Prefabs[id].typeID = id;
//		m_Prefabs[id].gridW = 2;
//		m_Prefabs[id].gridH = 1;
//		m_Prefabs[id].width = SCREEN_WITH * 2;
//		m_Prefabs[id].height = SCREEN_HEIGHT;
//		m_Prefabs[id].layerCount = 1;
//
//		int MW = m_Prefabs[id].width;
//		int MH = m_Prefabs[id].height;
//		int floorY = MH - thickness;
//
//		// 1. 외곽선 (천장 막힘, 좌측 벽 막힘)
//		SetRect(&rc, 0, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, -50, 0, thickness, MH); m_Prefabs[id].walls.push_back(rc);
//
//		// 2. 바닥 뚫기 (중앙 넓게)
//		SetRect(&rc, 0, floorY, MW / 2 - 200, MH + 50); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 200, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
//
//		// 3. 우측 벽 뚫기
//		SetRect(&rc, MW - thickness, 0, MW + 50, floorY - 200); m_Prefabs[id].walls.push_back(rc);
//
//		// 4. 지형: 구멍 위를 건너가는 징검다리
//		int pH = 30;
//		SetRect(&rc, 900, 600, 1100, 600 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1200, 500, 1400, 500 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1500, 650, 1700, 650 + pH); m_Prefabs[id].walls.push_back(rc);
//	}
//
//	// ====================================================================
//	// [프리팹 11번] 위, 아래, 오른쪽 (UP, DOWN, RIGHT) 뚫린 방
//	// 조합: 1 | 2 | 8 = 11
//	// 사이즈: 1칸 x 2칸 (1280 x 1600) 세로 긴 방
//	// 출구: 상/하단은 '중앙', 우측은 '하단'
//	// 지형: 수직으로 긴 엘리베이터 통로 느낌
//	// ====================================================================
//	{
//		int id = DOOR_UP | DOOR_DOWN | DOOR_RIGHT; // 11
//		m_Prefabs[id].typeID = id;
//		m_Prefabs[id].gridW = 1;
//		m_Prefabs[id].gridH = 2;
//		m_Prefabs[id].width = SCREEN_WITH;
//		m_Prefabs[id].height = SCREEN_HEIGHT * 2;
//		m_Prefabs[id].layerCount = 1;
//
//		int MW = m_Prefabs[id].width;
//		int MH = m_Prefabs[id].height;
//		int floorY = MH - thickness;
//
//		// 1. 좌측 벽 (막힘)
//		SetRect(&rc, -50, 0, thickness, MH); m_Prefabs[id].walls.push_back(rc);
//
//		// 2. 천장 & 바닥 (뚫림)
//		SetRect(&rc, 0, -50, MW / 2 - 150, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 150, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 0, floorY, MW / 2 - 150, MH + 50); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 150, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
//
//		// 3. 우측 벽 (하단 뚫림)
//		SetRect(&rc, MW - thickness, 0, MW + 50, SCREEN_HEIGHT); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - thickness, SCREEN_HEIGHT, MW + 50, floorY - 200); m_Prefabs[id].walls.push_back(rc);
//
//		// 4. 지형: 지그재그 플랫폼
//		int pH = 30;
//		SetRect(&rc, MW - 400, 1300, MW - 100, 1300 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 150, 1050, 450, 1050 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - 500, 800, MW - 200, 800 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 200, 550, 500, 550 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 - 150, 300, MW / 2 + 150, 300 + pH); m_Prefabs[id].walls.push_back(rc);
//	}
//
//	// ====================================================================
//	// [프리팹 12번] 왼쪽, 오른쪽 (LEFT, RIGHT) 뚫린 방
//	// 조합: 4 | 8 = 12
//	// 사이즈: 2칸 x 1칸 (2560 x 800) 가로 긴 방
//	// 출구: 좌/우측 모두 하단
//	// 지형: 긴 복도. 중간에 높은 장애물이 있어 넘어가야 함.
//	// ====================================================================
//	{
//		int id = DOOR_LEFT | DOOR_RIGHT; // 12
//		m_Prefabs[id].typeID = id;
//		m_Prefabs[id].gridW = 2;
//		m_Prefabs[id].gridH = 1;
//		m_Prefabs[id].width = SCREEN_WITH * 2;
//		m_Prefabs[id].height = SCREEN_HEIGHT;
//		m_Prefabs[id].layerCount = 1;
//
//		int MW = m_Prefabs[id].width;
//		int MH = m_Prefabs[id].height;
//		int floorY = MH - thickness;
//
//		// 1. 천장 & 바닥 (막힘)
//		SetRect(&rc, 0, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 0, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
//
//		// 2. 좌/우 벽 (뚫림)
//		SetRect(&rc, -50, 0, thickness, floorY - 200); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - thickness, 0, MW + 50, floorY - 200); m_Prefabs[id].walls.push_back(rc);
//
//		// 3. 지형: 복도 중앙을 막는 거대 바리케이드 (점프해서 넘어가야 함)
//		SetRect(&rc, 1100, 450, 1460, floorY); m_Prefabs[id].walls.push_back(rc);
//		// 장애물 넘기 편하게 보조 발판
//		SetRect(&rc, 800, 600, 1000, 630); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1560, 600, 1760, 630); m_Prefabs[id].walls.push_back(rc);
//	}
//
//	// ====================================================================
//	// [프리팹 13번] 위, 왼쪽, 오른쪽 (UP, LEFT, RIGHT) 뚫린 방
//	// 조합: 1 | 4 | 8 = 13
//	// 사이즈: 2칸 x 2칸 (2560 x 1600)
//	// 출구: 상단은 '중앙', 좌/우측은 '하단'
//	// 지형: 거대한 산 느낌. 좌/우에서 들어와 산 정상(위)으로 올라감.
//	// ====================================================================
//	{
//		int id = DOOR_UP | DOOR_LEFT | DOOR_RIGHT; // 13
//		m_Prefabs[id].typeID = id;
//		m_Prefabs[id].gridW = 2;
//		m_Prefabs[id].gridH = 2;
//		m_Prefabs[id].width = SCREEN_WITH * 2;
//		m_Prefabs[id].height = SCREEN_HEIGHT * 2;
//		m_Prefabs[id].layerCount = 1;
//
//		int MW = m_Prefabs[id].width;
//		int MH = m_Prefabs[id].height;
//		int floorY = MH - thickness;
//
//		// 1. 바닥 (막힘)
//		SetRect(&rc, 0, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
//
//		// 2. 천장 (뚫림)
//		SetRect(&rc, 0, -50, MW / 2 - 150, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 150, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
//
//		// 3. 좌/우 벽 (하단 뚫림)
//		SetRect(&rc, -50, 0, thickness, SCREEN_HEIGHT); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, -50, SCREEN_HEIGHT, thickness, floorY - 200); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - thickness, 0, MW + 50, SCREEN_HEIGHT); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - thickness, SCREEN_HEIGHT, MW + 50, floorY - 200); m_Prefabs[id].walls.push_back(rc);
//
//		// 4. 지형: 거대 산맥 (중앙 밀집형 발판)
//		int pH = 30;
//		SetRect(&rc, 900, 1300, 1660, floorY); m_Prefabs[id].walls.push_back(rc); // 1단 베이스
//		SetRect(&rc, 600, 1050, 900, 1050 + pH); m_Prefabs[id].walls.push_back(rc);  // 좌측 등반로
//		SetRect(&rc, 1660, 1050, 1960, 1050 + pH); m_Prefabs[id].walls.push_back(rc); // 우측 등반로
//		SetRect(&rc, 1000, 800, 1560, 800 + pH); m_Prefabs[id].walls.push_back(rc);  // 중앙 2단
//		SetRect(&rc, 1150, 550, 1410, 550 + pH); m_Prefabs[id].walls.push_back(rc);  // 중앙 3단
//		SetRect(&rc, MW / 2 - 150, 300, MW / 2 + 150, 300 + pH); m_Prefabs[id].walls.push_back(rc); // 출구 밑
//	}
//
//	// ====================================================================
//	// [프리팹 14번] 아래, 왼쪽, 오른쪽 (DOWN, LEFT, RIGHT) 뚫린 방
//	// 조합: 2 | 4 | 8 = 14
//	// 사이즈: 2칸 x 1칸 (2560 x 800)
//	// 출구: 하단은 '중앙', 좌/우측은 '하단'
//	// 지형: 바닥이 무너진 긴 다리.
//	// ====================================================================
//	{
//		int id = DOOR_DOWN | DOOR_LEFT | DOOR_RIGHT; // 14
//		m_Prefabs[id].typeID = id;
//		m_Prefabs[id].gridW = 2;
//		m_Prefabs[id].gridH = 1;
//		m_Prefabs[id].width = SCREEN_WITH * 2;
//		m_Prefabs[id].height = SCREEN_HEIGHT;
//		m_Prefabs[id].layerCount = 1;
//
//		int MW = m_Prefabs[id].width;
//		int MH = m_Prefabs[id].height;
//		int floorY = MH - thickness;
//
//		// 1. 천장 (막힘)
//		SetRect(&rc, 0, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
//
//		// 2. 바닥 (크게 뚫림)
//		SetRect(&rc, 0, floorY, MW / 2 - 300, MH + 50); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 300, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
//
//		// 3. 좌/우 벽 (뚫림)
//		SetRect(&rc, -50, 0, thickness, floorY - 200); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - thickness, 0, MW + 50, floorY - 200); m_Prefabs[id].walls.push_back(rc);
//
//		// 4. 지형: 흔들다리 (작은 발판 여러 개)
//		int pH = 30;
//		SetRect(&rc, 800, 650, 1000, 650 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1150, 550, 1410, 550 + pH); m_Prefabs[id].walls.push_back(rc); // 중앙 (높음)
//		SetRect(&rc, 1560, 650, 1760, 650 + pH); m_Prefabs[id].walls.push_back(rc);
//	}
//
//	// ====================================================================
//	// [프리팹 15번] 상, 하, 좌, 우 모두 뚫린 방 (The Nexus)
//	// 조합: 1 | 2 | 4 | 8 = 15
//	// 사이즈: 2칸 x 2칸 (2560 x 1600)
//	// 지형: 사방이 뚫린 거대한 메인 홀. 공중 정원 느낌.
//	// ====================================================================
//	{
//		int id = DOOR_UP | DOOR_DOWN | DOOR_LEFT | DOOR_RIGHT; // 15
//		m_Prefabs[id].typeID = id;
//		m_Prefabs[id].gridW = 2;
//		m_Prefabs[id].gridH = 2;
//		m_Prefabs[id].width = SCREEN_WITH * 2;
//		m_Prefabs[id].height = SCREEN_HEIGHT * 2;
//		m_Prefabs[id].layerCount = 1;
//
//		int MW = m_Prefabs[id].width;
//		int MH = m_Prefabs[id].height;
//		int floorY = MH - thickness;
//
//		// 1. 천장 & 바닥 (중앙 뚫림)
//		SetRect(&rc, 0, -50, MW / 2 - 200, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 200, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 0, floorY, MW / 2 - 200, MH + 50); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 200, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
//
//		// 2. 좌/우 벽 (하단 뚫림)
//		SetRect(&rc, -50, 0, thickness, SCREEN_HEIGHT); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, -50, SCREEN_HEIGHT, thickness, floorY - 200); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - thickness, 0, MW + 50, SCREEN_HEIGHT); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - thickness, SCREEN_HEIGHT, MW + 50, floorY - 200); m_Prefabs[id].walls.push_back(rc);
//
//		// 3. 지형: 거대 +자형 발판 구조
//		int pH = 30;
//
//		// 중앙 허브 정거장
//		SetRect(&rc, 1000, 1100, 1560, 1100 + pH); m_Prefabs[id].walls.push_back(rc);
//
//		// 하단 낙하 방지용 작은 발판
//		SetRect(&rc, 1150, 1350, 1410, 1350 + pH); m_Prefabs[id].walls.push_back(rc);
//
//		// 좌/우에서 중앙으로 이어지는 다리
//		SetRect(&rc, 400, 1250, 800, 1250 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1760, 1250, 2160, 1250 + pH); m_Prefabs[id].walls.push_back(rc);
//
//		// 위로 올라가는 계단
//		SetRect(&rc, 800, 850, 1100, 850 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1460, 600, 1760, 600 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 - 150, 350, MW / 2 + 150, 350 + pH); m_Prefabs[id].walls.push_back(rc); // 출구 바로 밑
//	}
//}

//void MapManager::InitPrefabs()
//{
//	RECT rc;
//	int thickness = 100;
//	int pH = 30; // 발판 두께
//
//	// ====================================================================
//	// [프리팹 1번] 위(UP) 
//	// ====================================================================
//	{
//		int id = DOOR_UP;
//		m_Prefabs[id].typeID = id;
//		m_Prefabs[id].gridW = 1; m_Prefabs[id].gridH = 2;
//		m_Prefabs[id].width = SCREEN_WITH; m_Prefabs[id].height = SCREEN_HEIGHT * 2;
//		m_Prefabs[id].layerCount = 1;
//
//		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;
//
//		SetRect(&rc, 0, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc); // 바닥
//		SetRect(&rc, -50, 0, thickness, MH); m_Prefabs[id].walls.push_back(rc);  // 좌벽
//		SetRect(&rc, MW - thickness, 0, MW + 50, MH); m_Prefabs[id].walls.push_back(rc); // 우벽
//		SetRect(&rc, 0, -50, MW / 2 - 150, thickness); m_Prefabs[id].walls.push_back(rc); // 천장(좌)
//		SetRect(&rc, MW / 2 + 150, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc); // 천장(우)
//
//		// [발판 추가] 촘촘한 지그재그 & 중앙 보조 발판 (150px 간격)
//		SetRect(&rc, 150, 1350, 400, 1350 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 850, 1200, 1100, 1200 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 450, 1050, 750, 1050 + pH); m_Prefabs[id].walls.push_back(rc); // 중앙
//		SetRect(&rc, 100, 900, 350, 900 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 850, 750, 1150, 750 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 450, 600, 700, 600 + pH); m_Prefabs[id].walls.push_back(rc); // 중앙
//		SetRect(&rc, 200, 450, 450, 450 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 - 150, 300, MW / 2 + 150, 300 + pH); m_Prefabs[id].walls.push_back(rc); // 출구 밑
//	}
//
//	// ====================================================================
//	// [프리팹 2번] 아래(DOWN) 
//	// ====================================================================
//	{
//		int id = DOOR_DOWN;
//		m_Prefabs[id].typeID = id;
//		m_Prefabs[id].gridW = 1; m_Prefabs[id].gridH = 1;
//		m_Prefabs[id].width = SCREEN_WITH; m_Prefabs[id].height = SCREEN_HEIGHT;
//		m_Prefabs[id].layerCount = 1;
//
//		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;
//
//		SetRect(&rc, 0, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc); // 천장
//		SetRect(&rc, -50, 0, thickness, MH); m_Prefabs[id].walls.push_back(rc); // 좌
//		SetRect(&rc, MW - thickness, 0, MW + 50, MH); m_Prefabs[id].walls.push_back(rc); // 우
//		SetRect(&rc, 0, floorY, MW / 2 - 150, MH + 50); m_Prefabs[id].walls.push_back(rc); // 바닥(좌)
//		SetRect(&rc, MW / 2 + 150, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc); // 바닥(우)
//
//		SetRect(&rc, 300, 500, 500, 500 + pH); m_Prefabs[id].walls.push_back(rc); // 보조 발판
//		SetRect(&rc, 750, 500, 950, 500 + pH); m_Prefabs[id].walls.push_back(rc);
//	}
//
//	// ====================================================================
//	// [프리팹 3번] 위, 아래(UP, DOWN) 
//	// ====================================================================
//	{
//		int id = DOOR_UP | DOOR_DOWN;
//		m_Prefabs[id].typeID = id;
//		m_Prefabs[id].gridW = 2; m_Prefabs[id].gridH = 2;
//		m_Prefabs[id].width = SCREEN_WITH * 2; m_Prefabs[id].height = SCREEN_HEIGHT * 2;
//		m_Prefabs[id].layerCount = 1;
//
//		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;
//
//		SetRect(&rc, -50, 0, thickness, MH); m_Prefabs[id].walls.push_back(rc); // 좌
//		SetRect(&rc, MW - thickness, 0, MW + 50, MH); m_Prefabs[id].walls.push_back(rc); // 우
//		SetRect(&rc, 0, -50, MW / 2 - 150, thickness); m_Prefabs[id].walls.push_back(rc); // 천장
//		SetRect(&rc, MW / 2 + 150, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 0, floorY, MW / 2 - 200, MH + 50); m_Prefabs[id].walls.push_back(rc); // 바닥
//		SetRect(&rc, MW / 2 + 200, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
//
//		// [발판 추가] 웅덩이 구조 + 촘촘한 공중 섬
//		SetRect(&rc, 0, 1150, 600, 1200); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - 600, 1000, MW, 1050); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 0, 850, 500, 900); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - 500, 650, MW, 700); m_Prefabs[id].walls.push_back(rc);
//
//		SetRect(&rc, 800, 1350, 1000, 1350 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1300, 1200, 1500, 1200 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 900, 1050, 1100, 1050 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1400, 850, 1600, 850 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1000, 650, 1200, 650 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1200, 450, 1450, 450 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 - 150, 300, MW / 2 + 150, 300 + pH); m_Prefabs[id].walls.push_back(rc); // 출구 밑
//	}
//
//	// ====================================================================
//	// [프리팹 4번] 왼쪽(LEFT)만 뚫린 방 -> 🌟 보스 대기실로 변경!
//	// 사이즈: 1칸 x 1칸 (1280 x 800)
//	// 특징: 평탄한 지형, 정중앙에 보스방으로 가는 텔레포터 제단 존재
//	// ====================================================================
//	{
//		int id = DOOR_LEFT; // 4
//		m_Prefabs[id].typeID = id;
//		m_Prefabs[id].gridW = 1;
//		m_Prefabs[id].gridH = 1;
//		m_Prefabs[id].width = SCREEN_WITH;
//		m_Prefabs[id].height = SCREEN_HEIGHT;
//		m_Prefabs[id].layerCount = 1;
//
//		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;
//		char FileName[256];
//		sprintf_s(FileName, "./resource/Img/map1/Ch1_maps/map_bossin.png");
//		m_Prefabs[id].bgLayer[0].Create(FileName, false, 0);
//		// 1. 외곽선 세팅
//		SetRect(&rc, 0, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc); // 천장 막음
//		SetRect(&rc, 0, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc); // 바닥 막음
//		SetRect(&rc, MW - thickness, 0, MW + 50, MH); m_Prefabs[id].walls.push_back(rc); // 우측 막음
//
//		// 2. 왼쪽 문 뚫기
//		SetRect(&rc, -50, 0, thickness, floorY - 200); m_Prefabs[id].walls.push_back(rc);
//
//		// 3. 지형: 정중앙에 텔레포터 제단 (살짝 솟아오른 형태)
//		SetRect(&rc, MW / 2 - 150, floorY - 30, MW / 2 + 150, floorY); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 - 100, floorY - 60, MW / 2 + 100, floorY - 30); m_Prefabs[id].walls.push_back(rc);
//	}
//
//	// ====================================================================
//	// [프리팹 5번] 위, 왼쪽 (UP, LEFT)
//	// ====================================================================
//	{
//		int id = DOOR_UP | DOOR_LEFT;
//		m_Prefabs[id].typeID = id;
//		m_Prefabs[id].gridW = 2; m_Prefabs[id].gridH = 2;
//		m_Prefabs[id].width = SCREEN_WITH * 2; m_Prefabs[id].height = SCREEN_HEIGHT * 2;
//		m_Prefabs[id].layerCount = 1;
//
//		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;
//
//		SetRect(&rc, 0, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - thickness, 0, MW + 50, MH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 0, -50, MW / 2 - 150, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 150, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, -50, 0, thickness, SCREEN_HEIGHT); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, -50, SCREEN_HEIGHT, thickness, floorY - 200); m_Prefabs[id].walls.push_back(rc);
//
//		// [발판 추가] 촘촘한 나선형
//		SetRect(&rc, 300, 1350, 600, 1350 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 800, 1200, 1100, 1200 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1300, 1050, 1600, 1050 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1800, 900, 2200, 900 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1400, 750, 1650, 750 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1000, 600, 1250, 600 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 800, 450, 1050, 450 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 - 150, 300, MW / 2 + 150, 300 + pH); m_Prefabs[id].walls.push_back(rc);
//	}
//
//	// ====================================================================
//	// [프리팹 6번] 아래, 왼쪽 (DOWN, LEFT) - 🌟 왼쪽 문이 위(TOP)에 있음
//	// ====================================================================
//	{
//		int id = DOOR_DOWN | DOOR_LEFT;
//		m_Prefabs[id].typeID = id;
//		m_Prefabs[id].gridW = 1; m_Prefabs[id].gridH = 2;
//		m_Prefabs[id].width = SCREEN_WITH; m_Prefabs[id].height = SCREEN_HEIGHT * 2;
//		m_Prefabs[id].layerCount = 1;
//
//		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;
//
//		SetRect(&rc, 0, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - thickness, 0, MW + 50, MH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 0, floorY, MW / 2 - 150, MH + 50); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 150, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, -50, 0, thickness, 600); m_Prefabs[id].walls.push_back(rc); // 상단 문 (뚫림)
//		SetRect(&rc, -50, 800, thickness, MH); m_Prefabs[id].walls.push_back(rc); // 하단 막음
//
//		// [발판 추가] 떨어지면서 밟기 (촘촘)
//		SetRect(&rc, 100, 750, 350, 750 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 500, 900, 800, 900 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 250, 1050, 500, 1050 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 650, 1200, 950, 1200 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 300, 1350, 550, 1350 + pH); m_Prefabs[id].walls.push_back(rc);
//	}
//
//	// ====================================================================
//	// [프리팹 7번] 위, 아래, 왼쪽 (UP, DOWN, LEFT) 
//	// ====================================================================
//	{
//		int id = DOOR_UP | DOOR_DOWN | DOOR_LEFT;
//		m_Prefabs[id].typeID = id;
//		m_Prefabs[id].gridW = 2; m_Prefabs[id].gridH = 2;
//		m_Prefabs[id].width = SCREEN_WITH * 2; m_Prefabs[id].height = SCREEN_HEIGHT * 2;
//		m_Prefabs[id].layerCount = 1;
//
//		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;
//
//		SetRect(&rc, MW - thickness, 0, MW + 50, MH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 0, -50, MW / 2 - 150, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 150, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 0, floorY, MW / 2 - 150, MH + 50); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 150, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, -50, 0, thickness, SCREEN_HEIGHT); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, -50, SCREEN_HEIGHT, thickness, floorY - 200); m_Prefabs[id].walls.push_back(rc);
//
//		// [발판 추가] 촘촘한 구름 다리
//		SetRect(&rc, 300, 1350, 600, 1350 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 750, 1200, 1050, 1200 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 100, 1050, MW / 2 + 400, 1050 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 - 200, 900, MW / 2 + 50, 900 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 150, 750, MW / 2 + 450, 750 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 - 300, 600, MW / 2 - 50, 600 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 50, 450, MW / 2 + 350, 450 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 - 150, 300, MW / 2 + 150, 300 + pH); m_Prefabs[id].walls.push_back(rc);
//	}
//
//	// ====================================================================
//	// [프리팹 8번] 오른쪽 (RIGHT) - 시작 방
//	// ====================================================================
//	{
//		int id = DOOR_RIGHT;
//		m_Prefabs[id].typeID = id;
//		m_Prefabs[id].gridW = 1; m_Prefabs[id].gridH = 1;
//		m_Prefabs[id].width = SCREEN_WITH; m_Prefabs[id].height = SCREEN_HEIGHT;
//		m_Prefabs[id].layerCount = 1;
//
//		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;
//
//		SetRect(&rc, 0, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 0, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, -50, 0, thickness, MH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - thickness, 0, MW + 50, floorY - 200); m_Prefabs[id].walls.push_back(rc); // 문
//	}
//
//	// ====================================================================
//	// [프리팹 9번] 위, 오른쪽 (UP, RIGHT)
//	// ====================================================================
//	{
//		int id = DOOR_UP | DOOR_RIGHT;
//		m_Prefabs[id].typeID = id;
//		m_Prefabs[id].gridW = 1; m_Prefabs[id].gridH = 2;
//		m_Prefabs[id].width = SCREEN_WITH; m_Prefabs[id].height = SCREEN_HEIGHT * 2;
//		m_Prefabs[id].layerCount = 1;
//
//		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;
//
//		SetRect(&rc, 0, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, -50, 0, thickness, MH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 0, -50, MW / 2 - 150, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 150, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - thickness, 0, MW + 50, SCREEN_HEIGHT); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - thickness, SCREEN_HEIGHT, MW + 50, floorY - 200); m_Prefabs[id].walls.push_back(rc);
//
//		// [발판 추가] 촘촘한 좌우 이동
//		SetRect(&rc, MW - 400, 1350, MW - 100, 1350 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 200, 1200, 500, 1200 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 600, 1050, 900, 1050 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 150, 900, 450, 900 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2, 750, MW / 2 + 300, 750 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 150, 600, 450, 600 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 500, 450, 800, 450 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 - 150, 300, MW / 2 + 150, 300 + pH); m_Prefabs[id].walls.push_back(rc);
//	}
//
//	// ====================================================================
//	// [프리팹 10번] 아래, 오른쪽 (DOWN, RIGHT) 
//	// ====================================================================
//	{
//		int id = DOOR_DOWN | DOOR_RIGHT;
//		m_Prefabs[id].typeID = id;
//		m_Prefabs[id].gridW = 2; m_Prefabs[id].gridH = 1;
//		m_Prefabs[id].width = SCREEN_WITH * 2; m_Prefabs[id].height = SCREEN_HEIGHT;
//		m_Prefabs[id].layerCount = 1;
//
//		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;
//
//		SetRect(&rc, 0, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, -50, 0, thickness, MH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 0, floorY, MW / 2 - 200, MH + 50); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 200, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - thickness, 0, MW + 50, floorY - 200); m_Prefabs[id].walls.push_back(rc);
//
//		// [발판 추가] 촘촘한 징검다리
//		SetRect(&rc, 600, 600, 800, 600 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1000, 500, 1200, 500 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1400, 600, 1600, 600 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1800, 500, 2000, 500 + pH); m_Prefabs[id].walls.push_back(rc);
//	}
//
//	// ====================================================================
//	// [프리팹 11번] 위, 아래, 오른쪽 (UP, DOWN, RIGHT) 
//	// ====================================================================
//	{
//		int id = DOOR_UP | DOOR_DOWN | DOOR_RIGHT;
//		m_Prefabs[id].typeID = id;
//		m_Prefabs[id].gridW = 1; m_Prefabs[id].gridH = 2;
//		m_Prefabs[id].width = SCREEN_WITH; m_Prefabs[id].height = SCREEN_HEIGHT * 2;
//		m_Prefabs[id].layerCount = 1;
//
//		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;
//
//		SetRect(&rc, -50, 0, thickness, MH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 0, -50, MW / 2 - 150, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 150, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 0, floorY, MW / 2 - 150, MH + 50); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 150, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - thickness, 0, MW + 50, SCREEN_HEIGHT); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - thickness, SCREEN_HEIGHT, MW + 50, floorY - 200); m_Prefabs[id].walls.push_back(rc);
//
//		// [발판 추가] 수직 엘리베이터 (150 간격)
//		SetRect(&rc, 600, 1350, 900, 1350 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 150, 1200, 450, 1200 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 700, 1050, 1000, 1050 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 200, 900, 500, 900 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 650, 750, 950, 750 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 150, 600, 450, 600 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 600, 450, 900, 450 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 - 150, 300, MW / 2 + 150, 300 + pH); m_Prefabs[id].walls.push_back(rc);
//	}
//
//	// ====================================================================
//	// [프리팹 12번] 왼쪽, 오른쪽 (LEFT, RIGHT) 
//	// ====================================================================
//	{
//		int id = DOOR_LEFT | DOOR_RIGHT;
//		m_Prefabs[id].typeID = id;
//		m_Prefabs[id].gridW = 2; m_Prefabs[id].gridH = 1;
//		m_Prefabs[id].width = SCREEN_WITH * 2; m_Prefabs[id].height = SCREEN_HEIGHT;
//		m_Prefabs[id].layerCount = 1;
//
//		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;
//
//		SetRect(&rc, 0, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 0, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, -50, 0, thickness, floorY - 200); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - thickness, 0, MW + 50, floorY - 200); m_Prefabs[id].walls.push_back(rc);
//
//		// [발판 추가] 바리케이드 완화
//		SetRect(&rc, 1000, 500, 1560, floorY); m_Prefabs[id].walls.push_back(rc); // 중앙 장애물 (낮춤)
//		SetRect(&rc, 600, 600, 900, 630); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1660, 600, 1960, 630); m_Prefabs[id].walls.push_back(rc);
//	}
//
//	// ====================================================================
//	// [프리팹 13번] 위, 왼쪽, 오른쪽 (UP, LEFT, RIGHT) 
//	// ====================================================================
//	{
//		int id = DOOR_UP | DOOR_LEFT | DOOR_RIGHT;
//		m_Prefabs[id].typeID = id;
//		m_Prefabs[id].gridW = 2; m_Prefabs[id].gridH = 2;
//		m_Prefabs[id].width = SCREEN_WITH * 2; m_Prefabs[id].height = SCREEN_HEIGHT * 2;
//		m_Prefabs[id].layerCount = 1;
//
//		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;
//
//		SetRect(&rc, 0, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 0, -50, MW / 2 - 150, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 150, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, -50, 0, thickness, SCREEN_HEIGHT); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, -50, SCREEN_HEIGHT, thickness, floorY - 200); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - thickness, 0, MW + 50, SCREEN_HEIGHT); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - thickness, SCREEN_HEIGHT, MW + 50, floorY - 200); m_Prefabs[id].walls.push_back(rc);
//
//		// [발판 추가] 산맥 등반로 완화 (150 간격)
//		SetRect(&rc, 900, 1350, 1660, floorY); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 500, 1200, 800, 1200 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1760, 1200, 2060, 1200 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 850, 1050, 1150, 1050 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1410, 1050, 1710, 1050 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1000, 900, 1300, 900 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1260, 750, 1560, 750 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1000, 600, 1300, 600 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1150, 450, 1450, 450 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 - 150, 300, MW / 2 + 150, 300 + pH); m_Prefabs[id].walls.push_back(rc);
//	}
//
//	// ====================================================================
//	// [프리팹 14번] 아래, 왼쪽, 오른쪽 (DOWN, LEFT, RIGHT) 
//	// ====================================================================
//	{
//		int id = DOOR_DOWN | DOOR_LEFT | DOOR_RIGHT;
//		m_Prefabs[id].typeID = id;
//		m_Prefabs[id].gridW = 2; m_Prefabs[id].gridH = 1;
//		m_Prefabs[id].width = SCREEN_WITH * 2; m_Prefabs[id].height = SCREEN_HEIGHT;
//		m_Prefabs[id].layerCount = 1;
//
//		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;
//
//		SetRect(&rc, 0, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 0, floorY, MW / 2 - 300, MH + 50); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 300, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, -50, 0, thickness, floorY - 200); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - thickness, 0, MW + 50, floorY - 200); m_Prefabs[id].walls.push_back(rc);
//
//		// [발판 추가] 촘촘한 흔들다리
//		SetRect(&rc, 700, 650, 900, 650 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1050, 550, 1250, 550 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1400, 550, 1600, 550 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1750, 650, 1950, 650 + pH); m_Prefabs[id].walls.push_back(rc);
//	}
//
//	// ====================================================================
//	// [프리팹 15번] 상, 하, 좌, 우 모두 뚫림 
//	// ====================================================================
//	{
//		int id = DOOR_UP | DOOR_DOWN | DOOR_LEFT | DOOR_RIGHT;
//		m_Prefabs[id].typeID = id;
//		m_Prefabs[id].gridW = 2; m_Prefabs[id].gridH = 2;
//		m_Prefabs[id].width = SCREEN_WITH * 2; m_Prefabs[id].height = SCREEN_HEIGHT * 2;
//		m_Prefabs[id].layerCount = 1;
//
//		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;
//
//		SetRect(&rc, 0, -50, MW / 2 - 200, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 200, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 0, floorY, MW / 2 - 200, MH + 50); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 + 200, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, -50, 0, thickness, SCREEN_HEIGHT); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, -50, SCREEN_HEIGHT, thickness, floorY - 200); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - thickness, 0, MW + 50, SCREEN_HEIGHT); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW - thickness, SCREEN_HEIGHT, MW + 50, floorY - 200); m_Prefabs[id].walls.push_back(rc);
//
//		// [발판 추가] 거대하고 촘촘한 정거장
//		SetRect(&rc, 1000, 1200, 1560, 1200 + pH); m_Prefabs[id].walls.push_back(rc); // 중앙 베이스
//		SetRect(&rc, 1150, 1350, 1410, 1350 + pH); m_Prefabs[id].walls.push_back(rc);
//
//		SetRect(&rc, 300, 1300, 550, 1300 + pH); m_Prefabs[id].walls.push_back(rc); // 좌
//		SetRect(&rc, 700, 1250, 950, 1250 + pH); m_Prefabs[id].walls.push_back(rc);
//
//		SetRect(&rc, 2000, 1300, 2250, 1300 + pH); m_Prefabs[id].walls.push_back(rc); // 우
//		SetRect(&rc, 1600, 1250, 1850, 1250 + pH); m_Prefabs[id].walls.push_back(rc);
//
//		SetRect(&rc, 1000, 1050, 1250, 1050 + pH); m_Prefabs[id].walls.push_back(rc); // 위로 가는 길
//		SetRect(&rc, 1300, 900, 1550, 900 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1000, 750, 1250, 750 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1300, 600, 1550, 600 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, 1000, 450, 1250, 450 + pH); m_Prefabs[id].walls.push_back(rc);
//		SetRect(&rc, MW / 2 - 150, 300, MW / 2 + 150, 300 + pH); m_Prefabs[id].walls.push_back(rc);
//	}
//
//	// ====================================================================
//	// [프리팹 16번] 진짜 보스방!
//	// 조합: ROOM_BOSS (16)
//	// 사이즈: 2칸 x 1칸 (2560 x 800) 넓은 결전의 무대
//	// 특징: 사방이 꽉 막혀있음. 텔레포트 전용 방.
//	// ====================================================================
//	{
//		int id = ROOM_BOSS; // 16
//		m_Prefabs[id].typeID = id;
//		m_Prefabs[id].gridW = 2; // 보스전은 넓게 2칸!
//		m_Prefabs[id].gridH = 1;
//		m_Prefabs[id].width = SCREEN_WITH * 2;
//		m_Prefabs[id].height = SCREEN_HEIGHT;
//		m_Prefabs[id].layerCount = 1;
//
//		// 🌟 보스방만의 특별한 배경 이미지가 있다면 여기에 세팅 (지금은 임시)
//		char FileName[256];
//		sprintf_s(FileName, "./resource/Img/map1/Ch1_maps/map_bossroom01.png");
//		m_Prefabs[id].bgLayer[0].Create(FileName, false, 0);
//
//		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;
//
//		// 1. 사방을 완벽히 차단 (아무데도 못 나감)
//		SetRect(&rc, 0, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc); // 바닥
//		SetRect(&rc, 0, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);  // 천장
//		SetRect(&rc, -50, 0, thickness, MH); m_Prefabs[id].walls.push_back(rc);  // 좌측 벽
//		SetRect(&rc, MW - thickness, 0, MW + 50, MH); m_Prefabs[id].walls.push_back(rc); // 우측 벽
//
//		// 2. 지형: 보스전용 완전 평지 (또는 필요에 따라 양쪽 끝에 회피용 발판 추가)
//		SetRect(&rc, 350, 450, 550, 480); m_Prefabs[id].walls.push_back(rc); // 좌측 회피 발판
//		SetRect(&rc, MW - 550, 450, MW - 350, 480); m_Prefabs[id].walls.push_back(rc); // 우측 회피 발판
//	}
//
//	// ====================================================================
//	// [자동 스폰 위치 계산기] (모든 1~15번 방 일괄 적용)
//	// ====================================================================
//	for (int i = 1; i <= 15; i++)
//	{
//		if (m_Prefabs[i].typeID == 0) continue;
//
//		float pWidth = (float)m_Prefabs[i].width;
//		float pHeight = (float)m_Prefabs[i].height;
//
//		// 1. 위에서 떨어질 때 (중앙 상단)
//		m_Prefabs[i].spawnX[DIR_UP] = pWidth / 2.0f;
//		m_Prefabs[i].spawnY[DIR_UP] = 50.0f;
//
//		// 2. 아래에서 솟아오를 때 (중앙 하단)
//		m_Prefabs[i].spawnX[DIR_DOWN] = pWidth / 2.0f;
//		m_Prefabs[i].spawnY[DIR_DOWN] = pHeight - 150.0f;
//
//		// 3. 왼쪽 문으로 들어올 때 (대부분 좌측 하단 문)
//		m_Prefabs[i].spawnX[DIR_LEFT] = 50.0f;
//		m_Prefabs[i].spawnY[DIR_LEFT] = pHeight - 150.0f;
//
//		// 4. 오른쪽 문으로 들어올 때 (대부분 우측 하단 문)
//		m_Prefabs[i].spawnX[DIR_RIGHT] = pWidth - 100.0f;
//		m_Prefabs[i].spawnY[DIR_RIGHT] = pHeight - 150.0f;
//	}
//
//	// [예외 처리] 왼쪽 문이 '상단'에 뚫려있는 6번 프리팹만 Y좌표 수정!
//	m_Prefabs[6].spawnY[DIR_LEFT] = SCREEN_HEIGHT - 150.0f; // y = 650 (위쪽 칸)
//}

void MapManager::InitPrefabs()
{
	RECT rc;
	int pH = 30; // 발판 두께

	// 초기화
	for (int i = 0; i < 17; i++) {
		m_Prefabs[i].typeID = 0;
		m_Prefabs[i].walls.clear();
	}

	// ====================================================================
	// 🌟 [도어 소켓 룰] 🌟
	// 1. 좌/우 문: 항상 y = 700 위치에서 진입. (그래서 바닥 양옆에 300px짜리 발판이 무조건 있음)
	// 2. 하단 문: 항상 x = 300 ~ 980 사이의 680px 거대 구멍으로 떨어짐.
	// 3. 상단 문: 떨어질 때를 대비해 x = 490 ~ 790, y = 250 위치에 낙하 방지용 발판이 있음.
	// ====================================================================

	// [프리팹 1번] 1x1 기본 방
	{
		int id = 1;
		m_Prefabs[id].typeID = 1;
		m_Prefabs[id].gridW = 1; m_Prefabs[id].gridH = 1;
		m_Prefabs[id].width = SCREEN_WITH; m_Prefabs[id].height = SCREEN_HEIGHT;
		m_Prefabs[id].layerCount = 1;
		m_Prefabs[id].bgLayer[0].Create("./resource/Img/map1/Ch1_maps/map01.png", false, 0);

		// 🌟 도어 소켓 (안전 지대)
		//SetRect(&rc, 0, 770, 300, 770 + pH); m_Prefabs[id].walls.push_back(rc); // 좌측 문 앞
		//SetRect(&rc, 980, 770, 1280, 770 + pH); m_Prefabs[id].walls.push_back(rc); // 우측 문 앞
		SetRect(&rc, 490, 250, 790, 250 + pH); m_Prefabs[id].walls.push_back(rc); // 상단 문 낙하 방지

		// 내부 지형 (자유롭게 디자인)
		SetRect(&rc, 350, 500, 550, 500 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 730, 500, 930, 500 + pH); m_Prefabs[id].walls.push_back(rc);
	}

	// [프리팹 2번] 1x2 수직 방 (엘리베이터)
	{
		int id = 2;
		m_Prefabs[id].typeID = 2;
		m_Prefabs[id].gridW = 1; m_Prefabs[id].gridH = 2;
		m_Prefabs[id].width = SCREEN_WITH; m_Prefabs[id].height = SCREEN_HEIGHT * 2;
		m_Prefabs[id].layerCount = 1;
		m_Prefabs[id].bgLayer[0].Create("./resource/Img/map1/Ch1_maps/map02.png", false, 0);

		// 도어 소켓 일괄 생성 (2개 칸 각각에 대해)
		for (int gy = 0; gy < 2; gy++) {
			float cy = gy * SCREEN_HEIGHT;
			switch (gy)
			{
			case 0:
				SetRect(&rc, 0, cy + 750, 300, cy + 750 + pH); m_Prefabs[id].walls.push_back(rc);
				SetRect(&rc, 980, cy + 750, 1280, cy + 750 + pH); m_Prefabs[id].walls.push_back(rc);
				break;
			case 1:
				break;
			default:
				break;
			}
			//SetRect(&rc, 0, cy + 770, 300, cy + 770 + pH); m_Prefabs[id].walls.push_back(rc);
			//SetRect(&rc, 980, cy + 770, 1280, cy + 770 + pH); m_Prefabs[id].walls.push_back(rc);
			SetRect(&rc, 530, cy + 250, 750, cy + 250 + pH); m_Prefabs[id].walls.push_back(rc);
		}

		// 수직 연결 지형
		SetRect(&rc, 300, 1300, 500, 1300 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 780, 1100, 980, 1100 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 300, 850, 500, 850 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 490, 550, 790, 550 + pH); m_Prefabs[id].walls.push_back(rc);
	}

	// [프리팹 3번] 2x1 수평 방 (긴 복도/웅덩이)
	{
		int id = 3;
		m_Prefabs[id].typeID = 3;
		m_Prefabs[id].gridW = 2; m_Prefabs[id].gridH = 1;
		m_Prefabs[id].width = SCREEN_WITH * 2; m_Prefabs[id].height = SCREEN_HEIGHT;
		m_Prefabs[id].layerCount = 1;
		m_Prefabs[id].bgLayer[0].Create("./resource/Img/map1/Ch1_maps/map03.png", false, 0);

		for (int gx = 0; gx < 2; gx++) {
			float cx = gx * SCREEN_WITH;
			/*SetRect(&rc, cx, 700, cx + 300, 700 + pH); m_Prefabs[id].walls.push_back(rc);
			SetRect(&rc, cx + 980, 700, cx + 1280, 700 + pH); m_Prefabs[id].walls.push_back(rc);*/
			SetRect(&rc, cx + 490, 250, cx + 790, 250 + pH); m_Prefabs[id].walls.push_back(rc);
		}

		// 웅덩이 위 흔들다리 지형
		SetRect(&rc, 800, 600, 1000, 600 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 1180, 500, 1380, 500 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 1560, 600, 1760, 600 + pH); m_Prefabs[id].walls.push_back(rc);
	}

	// [프리팹 4번] 2x2 거대 방 (중앙 허브)
	{
		int id = 4;
		m_Prefabs[id].typeID = 4;
		m_Prefabs[id].gridW = 2; m_Prefabs[id].gridH = 2;
		m_Prefabs[id].width = SCREEN_WITH * 2; m_Prefabs[id].height = SCREEN_HEIGHT * 2;
		m_Prefabs[id].layerCount = 1;
		m_Prefabs[id].bgLayer[0].Create("./resource/Img/map1/Ch1_maps/map04.png", false, 0);

		for (int gy = 0; gy < 2; gy++) {
			for (int gx = 0; gx < 2; gx++) {
				float cx = gx * SCREEN_WITH; float cy = gy * SCREEN_HEIGHT;
				switch (gy)
				{
				case 0:
					SetRect(&rc, cx, cy + 750, cx + 300, cy + 750 + pH); m_Prefabs[id].walls.push_back(rc);
					SetRect(&rc, cx + 980, cy + 750, cx + 1280, cy + 750 + pH); m_Prefabs[id].walls.push_back(rc);
					break;
				case 1:
					break;
				default:
					break;
				}
				//SetRect(&rc, cx, cy + 770, cx + 300, cy + 770 + pH); m_Prefabs[id].walls.push_back(rc);
				//SetRect(&rc, cx + 980, cy + 770, cx + 1280, cy + 770 + pH); m_Prefabs[id].walls.push_back(rc);
				SetRect(&rc, cx + 490, cy + 250, cx + 790, cy + 250 + pH); m_Prefabs[id].walls.push_back(rc);
			}
		}

		// 플랫폼
		SetRect(&rc, 1000, 1100, 1560, 1100 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 600, 1300, 800, 1300 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 1760, 1300, 1960, 1300 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 250, 500, 550, 500 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 1550, 500, 1850, 500 + pH); m_Prefabs[id].walls.push_back(rc);
	}
	// [프리팹 5번] 1x1 기본 방 변형
	{
		int id = 5;
		m_Prefabs[id].typeID = 5;
		m_Prefabs[id].gridW = 1; m_Prefabs[id].gridH = 1;
		m_Prefabs[id].width = SCREEN_WITH; m_Prefabs[id].height = SCREEN_HEIGHT;
		m_Prefabs[id].layerCount = 1;
		m_Prefabs[id].bgLayer[0].Create("./resource/Img/map1/Ch1_maps/map01.png", false, 0);

		// 🌟 도어 소켓 (안전 지대)
		//SetRect(&rc, 0, 770, 300, 770 + pH); m_Prefabs[id].walls.push_back(rc); // 좌측 문 앞
		//SetRect(&rc, 980, 770, 1280, 770 + pH); m_Prefabs[id].walls.push_back(rc); // 우측 문 앞
		SetRect(&rc, 490, 250, 790, 250 + pH); m_Prefabs[id].walls.push_back(rc); // 상단 문 낙하 방지

		// 내부 지형 (자유롭게 디자인)
		SetRect(&rc, 440, 500, 840, 500 + pH); m_Prefabs[id].walls.push_back(rc);
		//SetRect(&rc, 200, 400, 400, 400 + pH); m_Prefabs[id].walls.push_back(rc);
		//SetRect(&rc, 880, 400, 1080, 400 + pH); m_Prefabs[id].walls.push_back(rc);
	}
	// [프리팹 6번] 보스 대기실 (고정 1x1, 우측에 텔레포터)
	{
		int id = 6;
		m_Prefabs[id].typeID = 6;
		m_Prefabs[id].gridW = 1; m_Prefabs[id].gridH = 1;
		m_Prefabs[id].width = SCREEN_WITH; m_Prefabs[id].height = SCREEN_HEIGHT;
		m_Prefabs[id].layerCount = 1;
		m_Prefabs[id].bgLayer[0].Create("./resource/Img/map1/Ch1_maps/map_bossin.png", false, 0);

		// 기본 도어 소켓 (왼쪽 문만 뚫릴 예정이므로 왼쪽만 깔아둬도 무방함)
		// SetRect(&rc, 0, 700, 300, 700 + pH); m_Prefabs[id].walls.push_back(rc);

		// 보스방 텔레포트 제단 (중앙)
		SetRect(&rc, 490, 550, 790, 580); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 540, 520, 740, 550); m_Prefabs[id].walls.push_back(rc);
	}

	// [프리팹 16번] 보스방 (고정 2x1) 
	{
		int id = ROOM_BOSS; // 16
		m_Prefabs[id].typeID = 16;
		m_Prefabs[id].gridW = 2; m_Prefabs[id].gridH = 1;
		m_Prefabs[id].width = SCREEN_WITH * 2; m_Prefabs[id].height = SCREEN_HEIGHT;
		m_Prefabs[id].layerCount = 1;
		m_Prefabs[id].bgLayer[0].Create("./resource/Img/map1/Ch1_maps/map_bossroom01.png", false, 0);

		//보스방은 텔레포트 전용이므로 소켓이 필요 없고 통짜 바닥만 깝니다.
		SetRect(&rc, 0, 700, 2560, 700 + pH); m_Prefabs[id].walls.push_back(rc);
	}
}

//void MapManager::CreateRandomMap()
//{
//	// [1] 맵 초기화
//	for (int i = 1; i <= 10; i++)
//	{
//		for (int j = 0; j < 5; j++) m_MapList[i].nextMapID[j] = 0;
//		m_MapList[i].prefabID = 0;
//	}
//	int grid[15][15] = { 0, };
//
//	// [2] 1번 방(시작 방) 배치: 무조건 '오른쪽'이 뚫린 8번 프리팹 사용
//	m_MapList[1].id = 1;
//	m_MapList[1].prefabID = DOOR_RIGHT; // 프리팹 8번
//	m_MapList[1].width = m_Prefabs[DOOR_RIGHT].width;
//	m_MapList[1].height = m_Prefabs[DOOR_RIGHT].height;
//	grid[7][7] = 1; // 15x15 그리드의 정중앙(7,7)에 배치
//
//	int currentMapCount = 1;
//	int maxMapCount = 10;
//	int failCount = 0; // 무한 루프 방지용
//
//	// [3] 레고 블록 조립 (절차적 생성 시작!)
//	while (currentMapCount < maxMapCount && failCount < 1000)
//	{
//		failCount++;
//
//		// 3-1. 이미 배치된 방들 중 무작위로 하나 선택
//		int randRoomID = (rand() % currentMapCount) + 1;
//		int pID = m_MapList[randRoomID].prefabID;
//
//		// 3-2. 그 방의 문 4방향 중 하나 랜덤 선택
//		int dirs[4] = { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT };
//		int checkDir = dirs[rand() % 4];
//
//		// 선택한 방향이 프리팹 도면상 진짜 뚫려있는 문인지 비트마스크로 확인
//		int bitFlag = 0;
//		if (checkDir == DIR_UP) bitFlag = DOOR_UP;
//		else if (checkDir == DIR_DOWN) bitFlag = DOOR_DOWN;
//		else if (checkDir == DIR_LEFT) bitFlag = DOOR_LEFT;
//		else if (checkDir == DIR_RIGHT) bitFlag = DOOR_RIGHT;
//
//		if ((pID & bitFlag) == 0) continue; // 꽉 막힌 벽이면 취소
//		if (m_MapList[randRoomID].nextMapID[checkDir] != 0) continue; // 이미 다른 방과 연결된 문이어도 취소
//
//		// 3-3. 이 방의 그리드 좌표 찾기 (방이 여러 칸을 차지하면 가장 좌측 상단 기준)
//		int rx = -1, ry = -1;
//		for (int y = 0; y < 15; y++) {
//			for (int x = 0; x < 15; x++) {
//				if (grid[y][x] == randRoomID) {
//					rx = x; ry = y; break;
//				}
//			}
//			if (rx != -1) break;
//		}
//
//		// 3-4. 새 방이 들어갈 타겟 좌표 계산
//		int targetX = rx, targetY = ry;
//		if (checkDir == DIR_UP) targetY--;
//		else if (checkDir == DIR_DOWN) targetY += m_Prefabs[pID].gridH; // 현재 방의 세로 칸 수만큼 밑으로
//		else if (checkDir == DIR_LEFT) targetX--;
//		else if (checkDir == DIR_RIGHT) targetX += m_Prefabs[pID].gridW; // 현재 방의 가로 칸 수만큼 우측으로
//
//		if (targetX < 0 || targetX >= 15 || targetY < 0 || targetY >= 15) continue; // 맵 밖으로 나가면 취소
//
//		// 3-5. 연결될 수 있는 "반대쪽 문"을 가진 프리팹들만 모으기
//		int oppositeBit = 0;
//		if (checkDir == DIR_UP) oppositeBit = DOOR_DOWN;
//		else if (checkDir == DIR_DOWN) oppositeBit = DOOR_UP;
//		else if (checkDir == DIR_LEFT) oppositeBit = DOOR_RIGHT;
//		else if (checkDir == DIR_RIGHT) oppositeBit = DOOR_LEFT;
//
//		std::vector<int> validPrefabs;
//		for (int i = 1; i <= 15; i++) {
//			if (m_Prefabs[i].typeID == 0) continue; // 아직 안 만든 도면 패스
//			if ((m_Prefabs[i].typeID & oppositeBit) != 0) { // 반대쪽 문이 있다면 후보에 추가!
//				validPrefabs.push_back(i);
//			}
//		}
//		if (validPrefabs.empty()) continue;
//
//		// 3-6. 후보 중 랜덤으로 하나 뽑아서 "그리드에 들어갈 공간(1x2, 2x2 등)이 있는지" 검사
//		int newPrefabID = validPrefabs[rand() % validPrefabs.size()];
//		int gw = m_Prefabs[newPrefabID].gridW;
//		int gh = m_Prefabs[newPrefabID].gridH;
//
//		bool isSpaceFree = true;
//		if (targetX + gw > 15 || targetY + gh > 15) isSpaceFree = false; // 배열 넘어가면 컷
//		else {
//			for (int y = 0; y < gh; y++) {
//				for (int x = 0; x < gw; x++) {
//					if (grid[targetY + y][targetX + x] != 0) {
//						isSpaceFree = false; // 다른 방이 이미 자리를 차지하고 있으면 컷
//						break;
//					}
//				}
//				if (!isSpaceFree) break;
//			}
//		}
//
//		if (!isSpaceFree) continue; // 공간 없으면 배치 포기하고 다시 처음부터
//
//		// ==========================================================
//		// 모든 조건 통과! 새 방을 생성하고 연결합니다.
//		// ==========================================================
//		currentMapCount++;
//		int newRoomID = currentMapCount;
//
//		// 그리드 영역 차지 (예: 2x2 방이면 4칸에 모두 newRoomID 기록)
//		for (int y = 0; y < gh; y++) {
//			for (int x = 0; x < gw; x++) {
//				grid[targetY + y][targetX + x] = newRoomID;
//			}
//		}
//
//		// 새 방의 정보 저장
//		m_MapList[newRoomID].id = newRoomID;
//		m_MapList[newRoomID].prefabID = newPrefabID;
//		m_MapList[newRoomID].width = m_Prefabs[newPrefabID].width;
//		m_MapList[newRoomID].height = m_Prefabs[newPrefabID].height;
//		m_MapList[newRoomID].layerCount = m_Prefabs[newPrefabID].layerCount;
//
//		// 배경 이미지도 프리팹에서 가져와서 연결
//		for (int layer = 0; layer < m_MapList[newRoomID].layerCount; layer++) {
//			m_MapList[newRoomID].bgLayer[layer] = m_Prefabs[newPrefabID].bgLayer[layer];
//		}
//
//		// 기존 방과 새 방의 문(포탈) 서로 연결!
//		m_MapList[randRoomID].nextMapID[checkDir] = newRoomID;
//
//		int oppositeDir = 0;
//		if (checkDir == DIR_UP) oppositeDir = DIR_DOWN;
//		else if (checkDir == DIR_DOWN) oppositeDir = DIR_UP;
//		else if (checkDir == DIR_LEFT) oppositeDir = DIR_RIGHT;
//		else if (checkDir == DIR_RIGHT) oppositeDir = DIR_LEFT;
//
//		m_MapList[newRoomID].nextMapID[oppositeDir] = randRoomID;
//	}
//}

//void MapManager::CreateRandomMap()
//{
//	bool bossPlaced = false;
//	int mapGenAttempts = 0;
//
//	// 보스방이 우측(X >= 4)에 무사히 깔릴 때까지 맵 생성을 반복 (최대 50번 시도)
//	while (!bossPlaced && mapGenAttempts < 50)
//	{
//		mapGenAttempts++;
//
//		// 1. 맵 초기화
//		for (int i = 1; i < 40; i++) {
//			for (int j = 0; j < 5; j++) m_MapList[i].nextMapID[j] = 0;
//			m_MapList[i].prefabID = 0;
//		}
//		// 그리드 초기화 (0 = 빈 칸)
//		for (int y = 0; y < 6; y++) {
//			for (int x = 0; x < 6; x++) {
//				m_Grid[y][x] = 0;
//			}
//		}
//		// 2. 시작 방(1번) 배치: 오른쪽(DOOR_RIGHT)만 뚫린 8번 프리팹
//		m_MapList[1].id = 1;
//		m_MapList[1].prefabID = 8;
//		m_MapList[1].width = m_Prefabs[8].width;
//		m_MapList[1].height = m_Prefabs[8].height;
//		m_MapList[1].layerCount = m_Prefabs[8].layerCount;
//
//		m_Grid[3][0] = 1; // 시작 위치: 좌측 중간
//
//		// 시작 방의 배경 이미지를 복사
//		for (int i = 0; i < m_Prefabs[8].layerCount; i++) {
//			m_MapList[1].bgLayer[i] = m_Prefabs[8].bgLayer[i];
//		}
//
//		// 3. 큐(Queue) 생성 (연결해야 할 문들을 저장)
//		struct OpenDoor { int rID, dir; };
//		std::vector<OpenDoor> q;
//		q.push_back({ 1, DIR_RIGHT }); // 1번 방의 오른쪽 문을 뚫어라!
//
//		int currentMapCount = 1;
//		int maxMaps = 36;
//		int failCount = 0;
//
//		// 4. 레고 블록 조립 시작
//		while (!q.empty() && failCount < 1000 && currentMapCount < maxMaps)
//		{
//			failCount++;
//
//			// 큐에서 무작위 문 하나 꺼내기
//			int qIdx = rand() % q.size();
//			OpenDoor d = q[qIdx];
//			q.erase(q.begin() + qIdx);
//
//			if (m_MapList[d.rID].nextMapID[d.dir] != 0) continue;
//
//			// 기준 방 원점 찾기
//			int rx = -1, ry = -1;
//			for (int y = 0; y < 6; y++) {
//				for (int x = 0; x < 6; x++) {
//					if (m_Grid[y][x] == d.rID) { rx = x; ry = y; break; }
//				}
//				if (rx != -1) break;
//			}
//			if (rx == -1) continue;
//
//			int gw_old = m_Prefabs[m_MapList[d.rID].prefabID].gridW;
//			int gh_old = m_Prefabs[m_MapList[d.rID].prefabID].gridH;
//
//			// [핵심 수정 1] 새 방은 "어떤 비트마스크(DOOR)"를 가져야 하는가?
//			int oppDoorBit = 0;
//			if (d.dir == DIR_UP) oppDoorBit = DOOR_DOWN;
//			if (d.dir == DIR_DOWN) oppDoorBit = DOOR_UP;
//			if (d.dir == DIR_LEFT) oppDoorBit = DOOR_RIGHT;
//			if (d.dir == DIR_RIGHT) oppDoorBit = DOOR_LEFT;
//
//			std::vector<int> validPrefabs;
//			bool forceBoss = false;
//
//			int testTargetX = rx;
//			if (d.dir == DIR_RIGHT) testTargetX = rx + gw_old;
//			else if (d.dir == DIR_LEFT) testTargetX = rx - 1;
//
//			// X가 4 이상 다다르면 4번 프리팹(보스 대기실) 확정!
//			if (!bossPlaced && testTargetX >= 4 && d.dir == DIR_RIGHT) {
//				validPrefabs.push_back(4);
//				forceBoss = true;
//			}
//			else {
//				for (int newID = 1; newID <= 15; newID++) {
//					if (newID == 4 || newID == 16) continue;
//					if (m_Prefabs[newID].typeID == 0) continue;
//
//					// 🌟 짝이 맞는 문이 있는지 정확한 비트마스크로 검사!
//					if ((m_Prefabs[newID].typeID & oppDoorBit) == 0) continue;
//
//					int gw_new = m_Prefabs[newID].gridW;
//					int gh_new = m_Prefabs[newID].gridH;
//
//					int targetX = rx, targetY = ry;
//					if (d.dir == DIR_UP) { targetY = ry - gh_new; targetX = rx; }
//					if (d.dir == DIR_DOWN) { targetY = ry + gh_old; targetX = rx; }
//					if (d.dir == DIR_LEFT) { targetX = rx - gw_new; targetY = ry; }
//					if (d.dir == DIR_RIGHT) { targetX = rx + gw_old; targetY = ry; }
//
//					if (targetX < 0 || targetX + gw_new > 6 || targetY < 0 || targetY + gh_new > 6) continue;
//
//					bool spaceFree = true;
//					for (int y = 0; y < gh_new; y++) {
//						for (int x = 0; x < gw_new; x++) {
//							if (m_Grid[targetY + y][targetX + x] != 0) spaceFree = false;
//						}
//					}
//					if (!spaceFree) continue;
//
//					bool sealBroken = false;
//					int dirs[4] = { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT };
//					for (int checkD : dirs) {
//						// 🌟 [핵심 수정 2] DIR을 DOOR 비트마스크로 안전하게 변환
//						int checkDoorBit = 0;
//						if (checkD == DIR_UP) checkDoorBit = DOOR_UP;
//						if (checkD == DIR_DOWN) checkDoorBit = DOOR_DOWN;
//						if (checkD == DIR_LEFT) checkDoorBit = DOOR_LEFT;
//						if (checkD == DIR_RIGHT) checkDoorBit = DOOR_RIGHT;
//
//						if ((m_Prefabs[newID].typeID & checkDoorBit) != 0) {
//							int adjX = targetX, adjY = targetY;
//							if (checkD == DIR_UP) adjY = targetY - 1;
//							if (checkD == DIR_DOWN) adjY = targetY + gh_new;
//							if (checkD == DIR_LEFT) adjX = targetX - 1;
//							if (checkD == DIR_RIGHT) adjX = targetX + gw_new;
//
//							if (adjX < 0 || adjX >= 6 || adjY < 0 || adjY >= 6) {
//								sealBroken = true; break;
//							}
//							if (m_Grid[adjY][adjX] != 0) {
//								int adjRoom = m_Grid[adjY][adjX];
//
//								int adjOppDoorBit = 0;
//								if (checkD == DIR_UP) adjOppDoorBit = DOOR_DOWN;
//								if (checkD == DIR_DOWN) adjOppDoorBit = DOOR_UP;
//								if (checkD == DIR_LEFT) adjOppDoorBit = DOOR_RIGHT;
//								if (checkD == DIR_RIGHT) adjOppDoorBit = DOOR_LEFT;
//
//								if ((m_MapList[adjRoom].prefabID & adjOppDoorBit) == 0) {
//									sealBroken = true; break; // 옆방이 꽉 막힌 벽이면 탈락!
//								}
//							}
//						}
//					}
//					if (sealBroken) continue;
//
//					// 맵이 너무 커지지 않게 가지치기 제한
//					int doorCount = 0;
//					if (m_Prefabs[newID].typeID & DOOR_UP) doorCount++;
//					if (m_Prefabs[newID].typeID & DOOR_DOWN) doorCount++;
//					if (m_Prefabs[newID].typeID & DOOR_LEFT) doorCount++;
//					if (m_Prefabs[newID].typeID & DOOR_RIGHT) doorCount++;
//
//					if (currentMapCount >= 25 && doorCount > 2) continue;
//					if (currentMapCount >= 30 && doorCount > 1) continue;
//
//					validPrefabs.push_back(newID);
//				}
//			}
//
//			if (validPrefabs.empty()) continue;
//
//			// 5. 조건 통과! 새 방 배치
//			int newPrefabID = validPrefabs[rand() % validPrefabs.size()];
//			int gw_new = m_Prefabs[newPrefabID].gridW;
//			int gh_new = m_Prefabs[newPrefabID].gridH;
//
//			int targetX = rx, targetY = ry;
//			if (d.dir == DIR_UP) { targetY = ry - gh_new; targetX = rx; }
//			if (d.dir == DIR_DOWN) { targetY = ry + gh_old; targetX = rx; }
//			if (d.dir == DIR_LEFT) { targetX = rx - gw_new; targetY = ry; }
//			if (d.dir == DIR_RIGHT) { targetX = rx + gw_old; targetY = ry; }
//
//			currentMapCount++;
//			int newRoomID = currentMapCount;
//
//			for (int y = 0; y < gh_new; y++) {
//				for (int x = 0; x < gw_new; x++) {
//					m_Grid[targetY + y][targetX + x] = newRoomID;
//				}
//			}
//
//			m_MapList[newRoomID].id = newRoomID;
//			m_MapList[newRoomID].prefabID = newPrefabID;
//			m_MapList[newRoomID].width = m_Prefabs[newPrefabID].width;
//			m_MapList[newRoomID].height = m_Prefabs[newPrefabID].height;
//			m_MapList[newRoomID].layerCount = m_Prefabs[newPrefabID].layerCount;
//			// 새로 만들어진 방의 배경 이미지도 완벽하게 복사해 옵니다!
//			for (int i = 0; i < m_Prefabs[newPrefabID].layerCount; i++) {
//				m_MapList[newRoomID].bgLayer[i] = m_Prefabs[newPrefabID].bgLayer[i];
//			}
//
//			// 6. 새로 깔린 방의 모든 문을 확인하여, 이미 있는 방이면 연결하고 아니면 큐에 넣음
//			int dirs[4] = { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT };
//			for (int checkD : dirs) {
//				int checkDoorBit = 0;
//				if (checkD == DIR_UP) checkDoorBit = DOOR_UP;
//				if (checkD == DIR_DOWN) checkDoorBit = DOOR_DOWN;
//				if (checkD == DIR_LEFT) checkDoorBit = DOOR_LEFT;
//				if (checkD == DIR_RIGHT) checkDoorBit = DOOR_RIGHT;
//
//				if ((m_Prefabs[newPrefabID].typeID & checkDoorBit) != 0) {
//					int adjX = targetX, adjY = targetY;
//					if (checkD == DIR_UP) adjY = targetY - 1;
//					if (checkD == DIR_DOWN) adjY = targetY + gh_new;
//					if (checkD == DIR_LEFT) adjX = targetX - 1;
//					if (checkD == DIR_RIGHT) adjX = targetX + gw_new;
//
//					if (adjX >= 0 && adjX < 6 && adjY >= 0 && adjY < 6 && m_Grid[adjY][adjX] != 0) {
//						int adjRoom = m_Grid[adjY][adjX];
//
//						int adjOppDir = 0;
//						if (checkD == DIR_UP) adjOppDir = DIR_DOWN;
//						if (checkD == DIR_DOWN) adjOppDir = DIR_UP;
//						if (checkD == DIR_LEFT) adjOppDir = DIR_RIGHT;
//						if (checkD == DIR_RIGHT) adjOppDir = DIR_LEFT;
//
//						int adjOppDoorBit = 0;
//						if (adjOppDir == DIR_UP) adjOppDoorBit = DOOR_UP;
//						if (adjOppDir == DIR_DOWN) adjOppDoorBit = DOOR_DOWN;
//						if (adjOppDir == DIR_LEFT) adjOppDoorBit = DOOR_LEFT;
//						if (adjOppDir == DIR_RIGHT) adjOppDoorBit = DOOR_RIGHT;
//
//						// 옆 방에도 마주보는 문이 있다면 포탈 연결!
//						if ((m_MapList[adjRoom].prefabID & adjOppDoorBit) != 0) {
//							m_MapList[newRoomID].nextMapID[checkD] = adjRoom;
//							m_MapList[adjRoom].nextMapID[adjOppDir] = newRoomID;
//						}
//					}
//					else {
//						q.push_back({ newRoomID, checkD }); // 연결할 빈 공간이 있다면 큐에 추가
//					}
//				}
//			}
//
//			if (forceBoss) bossPlaced = true;
//		}
//
//		// ==========================================================
//		// [3단계 추가] 루프가 다 끝나고 맵이 완성된 후, 
//		// m_MapList의 맨 마지막 칸(39번)을 "진짜 보스방" 전용으로 셋팅합니다!
//		// ==========================================================
//		m_MapList[39].id = 39;
//		m_MapList[39].prefabID = ROOM_BOSS; // 16번 프리팹
//		m_MapList[39].width = m_Prefabs[ROOM_BOSS].width;
//		m_MapList[39].height = m_Prefabs[ROOM_BOSS].height;
//		m_MapList[39].layerCount = m_Prefabs[ROOM_BOSS].layerCount;
//
//		for (int i = 0; i < m_Prefabs[ROOM_BOSS].layerCount; i++)
//		{
//			m_MapList[39].bgLayer[i] = m_Prefabs[ROOM_BOSS].bgLayer[i];
//		}
//
//		// ==========================================================
//		// 🌟 [추가] 4단계: 맵 생성 완료 후 전체 그리드를 스캔하여 문(Door) 기록!
//		// ==========================================================
//		for (int y = 0; y < 6; y++) for (int x = 0; x < 6; x++) { m_DoorRight[y][x] = false; m_DoorDown[y][x] = false; }
//
//		for (int y = 0; y < 6; y++) {
//			for (int x = 0; x < 6; x++) {
//				int roomA = m_Grid[y][x];
//				if (roomA == 0) continue;
//
//				// 우측 방과 연결되었는가?
//				if (x < 5) {
//					int roomB = m_Grid[y][x + 1];
//					if (roomB != 0 && roomA != roomB) {
//						if (m_MapList[roomA].nextMapID[DIR_RIGHT] == roomB) m_DoorRight[y][x] = true;
//					}
//				}
//				// 아래쪽 방과 연결되었는가?
//				if (y < 5) {
//					int roomB = m_Grid[y + 1][x];
//					if (roomB != 0 && roomA != roomB) {
//						if (m_MapList[roomA].nextMapID[DIR_DOWN] == roomB) m_DoorDown[y][x] = true;
//					}
//				}
//			}
//		}
//	} // end of while loop for boss placement
//}

void MapManager::CreateRandomMap()
{
	bool bossPlaced = false;
	int mapGenAttempts = 0;

	while (!bossPlaced && mapGenAttempts < 50)
	{
		mapGenAttempts++;

		for (int i = 1; i < 40; i++) {
			for (int j = 0; j < 5; j++) m_MapList[i].nextMapID[j] = 0;
			m_MapList[i].prefabID = 0;
		}
		for (int y = 0; y < 6; y++) for (int x = 0; x < 6; x++) m_Grid[y][x] = 0;

		m_MapList[1].id = 1;
		m_MapList[1].prefabID = 1; // 시작방은 1x1 1번 프리팹 사용
		m_MapList[1].width = m_Prefabs[1].width;
		m_MapList[1].height = m_Prefabs[1].height;
		m_MapList[1].layerCount = m_Prefabs[1].layerCount;
		m_Grid[3][0] = 1;

		for (int i = 0; i < m_Prefabs[1].layerCount; i++) m_MapList[1].bgLayer[i] = m_Prefabs[1].bgLayer[i];

		struct OpenDoor { int rID, dir; };
		std::vector<OpenDoor> q;
		q.push_back({ 1, DIR_RIGHT });

		int currentMapCount = 1;
		int failCount = 0;

		while (!q.empty() && failCount < 1000 && currentMapCount < 30)
		{
			failCount++;
			int qIdx = rand() % q.size();
			OpenDoor d = q[qIdx];
			q.erase(q.begin() + qIdx);

			if (m_MapList[d.rID].nextMapID[d.dir] != 0) continue;

			int rx = -1, ry = -1;
			for (int y = 0; y < 6; y++) {
				for (int x = 0; x < 6; x++) {
					if (m_Grid[y][x] == d.rID) { rx = x; ry = y; break; }
				}
				if (rx != -1) break;
			}
			if (rx == -1) continue;

			// =======================================================
			// 보스 대기실(6)일 경우 1번 프리팹(1x1)의 규격을 빌려 쓰도록 방어!
			// =======================================================

			int gw_old = m_Prefabs[m_MapList[d.rID].prefabID].gridW;
			int gh_old = m_Prefabs[m_MapList[d.rID].prefabID].gridH;

			std::vector<int> validPrefabs;
			bool forceBoss = false;

			int testTargetX = rx;
			if (d.dir == DIR_RIGHT) testTargetX = rx + gw_old;
			else if (d.dir == DIR_LEFT) testTargetX = rx - 1;

			// X좌표가 5 이상 다다르면, 보스 대기실 확정!
			if (!bossPlaced && testTargetX >= 5 && d.dir == DIR_RIGHT) {
				validPrefabs.push_back(6); // 보스 대기실 용도로 6번 방(1x1) 사용
				forceBoss = true;
			}
			else {
				// [핵심 변경] 비트마스크 검사 삭제! 무조건 남는 공간이 있으면 프리팹(1~4번) 투입!
				for (int newID = 1; newID <= 5; newID++) {
					int gw_new = m_Prefabs[newID].gridW;
					int gh_new = m_Prefabs[newID].gridH;

					int targetX = rx, targetY = ry;
					if (d.dir == DIR_UP) { targetY = ry - gh_new; targetX = rx; }
					if (d.dir == DIR_DOWN) { targetY = ry + gh_old; targetX = rx; }
					if (d.dir == DIR_LEFT) { targetX = rx - gw_new; targetY = ry; }
					if (d.dir == DIR_RIGHT) { targetX = rx + gw_old; targetY = ry; }

					if (targetX < 0 || targetX + gw_new > 6 || targetY < 0 || targetY + gh_new > 6) continue;

					bool spaceFree = true;
					for (int y = 0; y < gh_new; y++) {
						for (int x = 0; x < gw_new; x++) {
							if (m_Grid[targetY + y][targetX + x] != 0) spaceFree = false;
						}
					}
					if (spaceFree) validPrefabs.push_back(newID);
				}
			}

			if (validPrefabs.empty()) continue;

			int newPrefabID = validPrefabs[rand() % validPrefabs.size()];
			int gw_new = m_Prefabs[newPrefabID].gridW;
			int gh_new = m_Prefabs[newPrefabID].gridH;

			int targetX = rx, targetY = ry;
			if (d.dir == DIR_UP) { targetY = ry - gh_new; targetX = rx; }
			if (d.dir == DIR_DOWN) { targetY = ry + gh_old; targetX = rx; }
			if (d.dir == DIR_LEFT) { targetX = rx - gw_new; targetY = ry; }
			if (d.dir == DIR_RIGHT) { targetX = rx + gw_old; targetY = ry; }

			currentMapCount++;
			int newRoomID = currentMapCount;

			for (int y = 0; y < gh_new; y++) {
				for (int x = 0; x < gw_new; x++) {
					m_Grid[targetY + y][targetX + x] = newRoomID;
				}
			}

			m_MapList[newRoomID].id = newRoomID;
			m_MapList[newRoomID].prefabID = newPrefabID;
			m_MapList[newRoomID].width = m_Prefabs[newPrefabID].width;
			m_MapList[newRoomID].height = m_Prefabs[newPrefabID].height;
			m_MapList[newRoomID].layerCount = m_Prefabs[newPrefabID].layerCount;
			for (int i = 0; i < m_Prefabs[newPrefabID].layerCount; i++) {
				m_MapList[newRoomID].bgLayer[i] = m_Prefabs[newPrefabID].bgLayer[i];
			}

			// 무조건 사방에 뚫을 수 있는 문을 큐에 던져넣음
			int dirs[4] = { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT };
			for (int checkD : dirs) {
				int adjX = targetX, adjY = targetY;
				if (checkD == DIR_UP) adjY = targetY - 1;
				if (checkD == DIR_DOWN) adjY = targetY + gh_new;
				if (checkD == DIR_LEFT) adjX = targetX - 1;
				if (checkD == DIR_RIGHT) adjX = targetX + gw_new;

				if (adjX >= 0 && adjX < 6 && adjY >= 0 && adjY < 6 && m_Grid[adjY][adjX] != 0) {
					int adjRoom = m_Grid[adjY][adjX];
					int adjOppDir = 0;
					if (checkD == DIR_UP) adjOppDir = DIR_DOWN;
					if (checkD == DIR_DOWN) adjOppDir = DIR_UP;
					if (checkD == DIR_LEFT) adjOppDir = DIR_RIGHT;
					if (checkD == DIR_RIGHT) adjOppDir = DIR_LEFT;

					m_MapList[newRoomID].nextMapID[checkD] = adjRoom;
					m_MapList[adjRoom].nextMapID[adjOppDir] = newRoomID;
				}
				else {
					q.push_back({ newRoomID, checkD });
				}
			}

			if (forceBoss) { bossPlaced = true; break; }
		}

		m_MapList[39].id = 39;
		m_MapList[39].prefabID = ROOM_BOSS;
		m_MapList[39].width = m_Prefabs[ROOM_BOSS].width;
		m_MapList[39].height = m_Prefabs[ROOM_BOSS].height;
		m_MapList[39].layerCount = m_Prefabs[ROOM_BOSS].layerCount;
		for (int i = 0; i < m_Prefabs[ROOM_BOSS].layerCount; i++) {
			m_MapList[39].bgLayer[i] = m_Prefabs[ROOM_BOSS].bgLayer[i];
		}

		for (int y = 0; y < 6; y++) for (int x = 0; x < 6; x++) { m_DoorRight[y][x] = false; m_DoorDown[y][x] = false; }
		for (int y = 0; y < 6; y++) {
			for (int x = 0; x < 6; x++) {
				int roomA = m_Grid[y][x];
				if (roomA == 0) continue;
				if (x < 5 && m_Grid[y][x + 1] != 0 && roomA != m_Grid[y][x + 1]) {
					if (m_MapList[roomA].nextMapID[DIR_RIGHT] == m_Grid[y][x + 1]) m_DoorRight[y][x] = true;
				}
				if (y < 5 && m_Grid[y + 1][x] != 0 && roomA != m_Grid[y + 1][x]) {
					if (m_MapList[roomA].nextMapID[DIR_DOWN] == m_Grid[y + 1][x]) m_DoorDown[y][x] = true;
				}
			}
		}
	}
}

void MapManager::ChangeMap(int mapID)
{
	// 맵을 36개까지 쓰기로 했으니, 제한을 40으로
	if (mapID < 1 || mapID >= 40) return;

	m_pCurrentMapChunk = &m_MapList[mapID];

	CAM->SetMapSize(m_pCurrentMapChunk->width, m_pCurrentMapChunk->height);

	// [필수] 맵 바뀔 때 상태 리셋
	knight.isDashing = false;
	knight.gravity = 0;
	knight.isMove = false;
	coll.ClearWalls();

	int pID = m_pCurrentMapChunk->prefabID;

	RECT rc;

	// 혹시 에러로 프리팹이 배정 안 된 방이라면? (안전장치)
	if (pID == 0 || m_Prefabs[pID].walls.empty())
	{
		SetRect(&rc, 0, m_pCurrentMapChunk->height - 100, m_pCurrentMapChunk->width, m_pCurrentMapChunk->height + 50);
		coll.AddWall(rc);
		return;
	}

	// 1. 도면에 있는 수많은 벽과 발판들을 그냥 리스트에서 꺼내서 맵에 쫙! 뿌립니다.
	for (auto& prefabWall : m_Prefabs[pID].walls)
	{
		coll.AddWall(prefabWall);
	}

	//// =================================================================
	//// 2. 막힌 문 자동 캡핑 (진행 불가 버그 완벽 차단)
	//// =================================================================
	//int MW = m_pCurrentMapChunk->width;
	//int MH = m_pCurrentMapChunk->height;
	//int thick = 100;
	//// 프리팹 도면상으로는 문이 뚫려있는데, 실제 연결된 맵(nextMapID)이 0(없음)이라면?
	//// 묻지도 따지지도 않고 그곳에 보이지 않는 거대한 철벽을 세워버립니다!
	//if ((m_Prefabs[pID].typeID & DOOR_UP) && m_pCurrentMapChunk->nextMapID[DIR_UP] == 0) {
	//	SetRect(&rc, 0, -50, MW, thick); coll.AddWall(rc);
	//}
	//if ((m_Prefabs[pID].typeID & DOOR_DOWN) && m_pCurrentMapChunk->nextMapID[DIR_DOWN] == 0) {
	//	SetRect(&rc, 0, MH - thick, MW, MH + 50); coll.AddWall(rc);
	//}
	//if ((m_Prefabs[pID].typeID & DOOR_LEFT) && m_pCurrentMapChunk->nextMapID[DIR_LEFT] == 0) {
	//	SetRect(&rc, -50, 0, thick, MH); coll.AddWall(rc);
	//}
	//if ((m_Prefabs[pID].typeID & DOOR_RIGHT) && m_pCurrentMapChunk->nextMapID[DIR_RIGHT] == 0) {
	//	SetRect(&rc, MW - thick, 0, MW + 50, MH); coll.AddWall(rc);
	//}
	// =================================================================
	// 2. 도어 소켓(Door Socket) 기반 자동 캡핑 엔진!
	// =================================================================

	// =================================================================
	// 고급 도어 소켓 캡핑 엔진 (화면에 보이게 두께 30px 할당 & 구멍 뚫기)
	// =================================================================
	int currentRoomID = m_pCurrentMapChunk->id;
	int gw = m_Prefabs[pID].gridW;
	int gh = m_Prefabs[pID].gridH;
	int MW = SCREEN_WITH;
	int MH = SCREEN_HEIGHT;
	// 보스방(39번)은 그리드 바깥에 있으므로, 자동 캡핑을 무시하고 통짜 사방 벽을 세워줍니다!
	if (currentRoomID == 39)
	{
		SetRect(&rc, -50, -50, MW * 2 + 50, 30); coll.AddWall(rc);
		SetRect(&rc, -50, MH - 30, MW * 2 + 50, MH + 30); coll.AddWall(rc);
		SetRect(&rc, -30, -50, 30, MH + 50); coll.AddWall(rc);
		SetRect(&rc, MW * 2 - 30, -50, MW * 2 + 30, MH + 50); coll.AddWall(rc);
	}
	else if (currentRoomID == 38)
	{
		SetRect(&rc, -50, -50, gw * MW + 50, 30); coll.AddWall(rc); // 꽉 막힌 천장
		SetRect(&rc, -50, gh * MH - 30, gw * MW + 50, gh * MH + 30); coll.AddWall(rc); // 꽉 막힌 바닥
		SetRect(&rc, -30, -50, 30, gh * MH + 50); coll.AddWall(rc); // 꽉 막힌 좌측 벽
		SetRect(&rc, gw * MW - 30, -50, gw * MW + 30, gh * MH + 50); coll.AddWall(rc); // 꽉 막힌 우측 벽
	}
	else 
	{
		int rootX = GetRoomGridX(currentRoomID);
		int rootY = GetRoomGridY(currentRoomID);

		for (int y = 0; y < gh; y++)
		{
			for (int x = 0; x < gw; x++)
			{
				int cx = rootX + x;
				int cy = rootY + y;
				float px = x * MW;
				float py = y * MH;

				// ⬆️ 윗면 막기 (천장)
				if (cy == 0 || (m_Grid[cy - 1][cx] != currentRoomID && !m_DoorDown[cy - 1][cx])) {
					SetRect(&rc, px - 50, py, px + MW + 50, py + 30); coll.AddWall(rc); // 완전 막힘
				}
				else if (cy > 0 && m_Grid[cy - 1][cx] != currentRoomID && m_DoorDown[cy - 1][cx]) {
					SetRect(&rc, px - 50, py, px + 490, py + 30); coll.AddWall(rc);     // 뚫림 (좌측 천장)
					SetRect(&rc, px + 790, py, px + MW + 50, py + 30); coll.AddWall(rc);// 뚫림 (우측 천장)
				}

				// ⬇️ 아랫면 막기 (바닥)
				if (cy == 5 || (m_Grid[cy + 1][cx] != currentRoomID && !m_DoorDown[cy][cx])) {
					SetRect(&rc, px - 50, py + MH - 30, px + MW + 50, py + MH + 30); coll.AddWall(rc);
				}
				else if (cy < 5 && m_Grid[cy + 1][cx] != currentRoomID && m_DoorDown[cy][cx]) {
					SetRect(&rc, px - 50, py + MH - 30, px + 490, py + MH + 30); coll.AddWall(rc);
					SetRect(&rc, px + 790, py + MH - 30, px + MW + 50, py + MH + 30); coll.AddWall(rc);
				}

				// ⬅️ 좌측면 막기
				if (cx == 0 || (m_Grid[cy][cx - 1] != currentRoomID && !m_DoorRight[cy][cx - 1])) {
					SetRect(&rc, px - 30, py, px + 30, py + MH); coll.AddWall(rc);
				}
				else if (cx > 0 && m_Grid[cy][cx - 1] != currentRoomID && m_DoorRight[cy][cx - 1]) {
					SetRect(&rc, px - 30, py - 50, px + 30, py + 450); coll.AddWall(rc); // 뚫림 (문 위쪽 벽)
					SetRect(&rc, px - 30, py + 800, px + 30, py + MH + 50); coll.AddWall(rc); // 문 아래쪽 벽
				}

				// ➡️ 우측면 막기
				if (cx == 5 || (m_Grid[cy][cx + 1] != currentRoomID && !m_DoorRight[cy][cx])) {
					SetRect(&rc, px + MW - 30, py, px + MW + 30, py + MH); coll.AddWall(rc);
				}
				else if (cx < 5 && m_Grid[cy][cx + 1] != currentRoomID && m_DoorRight[cy][cx]) {
					SetRect(&rc, px + MW - 30, py - 50, px + MW + 30, py + 450); coll.AddWall(rc);
					SetRect(&rc, px + MW - 30, py + 800, px + MW + 30, py + MH + 50); coll.AddWall(rc);
				}
			}
		}
	}
	
	// 이전 맵 적들 메모리 정리
	for (auto e : m_Enemies) delete e;
	m_Enemies.clear();

	//// 8번 프리팹(시작방)일 때 테스트용 몹 스폰!
	//if (pID == 8) {
	//	// 지상 몹 2마리
	//	Enemy* g1 = new GroundEnemy(); g1->Init(400, m_pCurrentMapChunk->height - 200);
	//	Enemy* g2 = new GroundEnemy(); g2->Init(800, m_pCurrentMapChunk->height - 200);
	//	m_Enemies.push_back(g1);
	//	m_Enemies.push_back(g2);

	//	// 비행 몹 1마리 공중에
	//	Enemy* f1 = new FlyEnemy(); f1->Init(600, m_pCurrentMapChunk->height - 500);
	//	m_Enemies.push_back(f1);
	//}
	// =================================================================
	// 진짜 보스방(39번) 진입 시: 감시자의 기사 3형제 스폰 & 컷신 시작!
	// =================================================================
	if (currentRoomID == 39||(currentRoomID == 38)&&pID==ROOM_BOSS)
	{
		// 렌더링 순서(Z-Order)를 위해 배경 쪽에 있는 애들부터 먼저 스폰(Draw)합니다.
		knight.pos.x = 1000.0f;
		// 1. 왼쪽 보스 (2번, 잠듦)
		BossEnemy* b2 = new BossEnemy(2);
		b2->Init(800.0f, m_pCurrentMapChunk->height - 200.0f);
		b2->ChangeState(B_STATE_SLEEP);
		m_Enemies.push_back(b2);

		// 2. 오른쪽 보스 (3번, 잠듦)
		BossEnemy* b3 = new BossEnemy(3);
		b3->Init(2000.0f, m_pCurrentMapChunk->height - 200.0f);
		b3->ChangeState(B_STATE_SLEEP);
		m_Enemies.push_back(b3);

		// 3. 가운데 보스 (1번, 깨어남!) -> 가장 마지막에 그려서 화면 맨 앞에 오게 함
		BossEnemy* b1 = new BossEnemy(1);
		b1->Init(1400.0f, m_pCurrentMapChunk->height - 200.0f);
		b1->ChangeState(B_STATE_AWAKE_ROAR);
		m_Enemies.push_back(b1);

		// 기사 컷신 모드 돌입 (키보드 조작 불가)
		knight.isCutscene = true;
	}

	// 방 크기에 비례하여 다이나믹 몬스터 스폰!
	if (pID != ROOM_BOSS && currentRoomID != 1) // 보스방과 처음 시작방(1번방)은 몬스터 생성 X
	{
		int gw = m_Prefabs[pID].gridW;
		int gh = m_Prefabs[pID].gridH;

		// 1. 지상 몹 (가로 칸 수당 2마리씩)
		for (int i = 0; i < gw * 2; i++) {
			Enemy* g = new GroundEnemy();
			// 가로로 띄엄띄엄 배치, 높이는 제일 아래쪽 바닥 근처
			g->Init(400.0f + (i * 600.0f), (gh * SCREEN_HEIGHT) - 150.0f);
			m_Enemies.push_back(g);
		}

		// 2. 비행 몹 (세로 칸 수당 1마리씩)
		for (int i = 0; i < gh; i++) {
			Enemy* f = new FlyEnemy();
			// 세로로 띄엄띄엄 공중에 배치
			f->Init(600.0f, 200.0f + (i * 800.0f));
			m_Enemies.push_back(f);
		}
	}
}

void MapManager::Update(double frame)
{
	if (m_pCurrentMapChunk == nullptr) return;

	int MW = m_pCurrentMapChunk->width;
	int MH = m_pCurrentMapChunk->height;
	int rX = GetRoomGridX(m_pCurrentMapChunk->id);
	int rY = GetRoomGridY(m_pCurrentMapChunk->id);

	// 기사가 방의 어느 '칸(Cell)'에 있는지 정확히 계산
	float clampX = knight.pos.x; if (clampX < 0) clampX = 0; if (clampX >= MW) clampX = MW - 1;
	float clampY = knight.pos.y; if (clampY < 0) clampY = 0; if (clampY >= MH) clampY = MH - 1;

	int localX = (int)(clampX / SCREEN_WITH);
	int localY = (int)(clampY / SCREEN_HEIGHT);
	int gx = rX + localX; // 현재 있는 절대 그리드 X
	int gy = rY + localY; // 현재 있는 절대 그리드 Y

	// 해당 칸 안에서의 세부 픽셀 위치
	float offsetX = clampX - (localX * SCREEN_WITH);
	float offsetY = clampY - (localY * SCREEN_HEIGHT);

	// 1. 오른쪽으로 나갈 때
	if (knight.pos.x >= MW) {
		if (gx < 5 && m_DoorRight[gy][gx]) { // 오른쪽으로 뚫린 문이 있다면?
			int nextMap = m_Grid[gy][gx + 1];
			ChangeMap(nextMap);
			int new_rY = GetRoomGridY(nextMap);
			knight.pos.x = 50.0f;
			knight.pos.y = (gy - new_rY) * SCREEN_HEIGHT + offsetY; // 🌟 들어간 높이 그대로 튀어나옴!
			return;
		}
		else knight.pos.x = MW - 1; // 막혀있으면 튕겨냄
	}
	// 2. 왼쪽으로 나갈 때
	else if (knight.pos.x <= 0) {
		if (gx > 0 && m_DoorRight[gy][gx - 1]) {
			int nextMap = m_Grid[gy][gx - 1];
			ChangeMap(nextMap);
			int new_rY = GetRoomGridY(nextMap);
			knight.pos.x = m_pCurrentMapChunk->width - 50.0f;
			knight.pos.y = (gy - new_rY) * SCREEN_HEIGHT + offsetY;
			return;
		}
		else knight.pos.x = 1;
	}
	// 3. 위로 올라갈 때
	else if (knight.pos.y <= 0) {
		if (gy > 0 && m_DoorDown[gy - 1][gx]) {
			int nextMap = m_Grid[gy - 1][gx];
			ChangeMap(nextMap);
			int new_rX = GetRoomGridX(nextMap);
			knight.pos.x = (gx - new_rX) * SCREEN_WITH + offsetX; // 🌟 올라간 폭 그대로 튀어나옴!
			knight.pos.y = m_pCurrentMapChunk->height - 150.0f;
			knight.gravity = -12.0f;
			return;
		}
		else knight.pos.y = 1;
	}
	// 4. 아래로 떨어질 때
	else if (knight.pos.y >= MH) {
		if (gy < 5 && m_DoorDown[gy][gx]) {
			int nextMap = m_Grid[gy + 1][gx];
			ChangeMap(nextMap);
			int new_rX = GetRoomGridX(nextMap);
			knight.pos.x = (gx - new_rX) * SCREEN_WITH + offsetX;
			knight.pos.y = 50.0f;
			return;
		}
		else {
			knight.pos.y = MH - 100.0f; knight.grounded = true;
		}
	}

	// ==========================================================
	// 5. 보스 대기실 텔레포트 상호작용
	// ==========================================================
	if (m_pCurrentMapChunk->prefabID == 6) // 현재 방이 6번(보스 대기실)일 때만 작동!
	{
		// 기사가 맵의 중앙(MW / 2) 부근 제단 위에 서 있는지 확인 (좌우 100픽셀 여유)
		if (knight.pos.x >= MW / 2.0f - 100.0f && knight.pos.x <= MW / 2.0f + 100.0f)
		{
			// 기사가 땅에 서 있고(grounded), 윗방향키(isLookup)를 눌렀다면 텔레포트 발동!
			if (knight.grounded && knight.isLookup)
			{
				ChangeMap(39); // 미리 준비해둔 39번 맵(진짜 보스방)으로 전환!

				int bossRoomID = 16;

				// 진짜 보스방(16번)의 정중앙 공중에 스폰시켜서 멋지게 떨어지도록 연출
				knight.pos.x = 1000.0f;
				knight.pos.y = m_Prefabs[bossRoomID].height - 250.0f;

				return; // 맵이 바뀌었으니 이번 프레임 Update 즉시 종료
			}
		}
	}

	// 적 업데이트 및 전투(충돌) 처리
	for (auto it = m_Enemies.begin(); it != m_Enemies.end(); ) {
		Enemy* e = *it;
		e->Update();

		if (!e->isDead) {
			RECT temp;
			// 1. 기사가 적을 때림! (공격 히트박스 vs 적 몸체)
			if (knight.isAttacking && !knight.isAttackHit) {
				if (e->IsTargetable()&&IntersectRect(&temp, &knight.attackBox, &e->m_rc)) {
					knight.isAttackHit = true;
					e->TakeDamage(1, knight.dir == 1 ? -1 : 1); // 때린 방향으로 넉백

					if (knight.attackType == 2) { // 하단 찍기 포고 점프!
						knight.gravity = -13.0f;
						// 강제로 공중 판정을 주고, 바닥에서 5픽셀 강제로 뜯어냅니다! (끼임 방지)
						knight.grounded = false;
						knight.pos.y -= 5.0f;
					}
				}
			}

			// 2. 적이 기사를 때림! (몸통 박치기)
			if (!knight.isInvincible) {
				// 잠들어 있는 보스인지 확인하는 방어 코드
				if (e->CanDealDamage() && IntersectRect(&temp, &knight.m_rc, &e->m_rc)) {
					int pushDir = (knight.pos.x < e->pos.x) ? -1 : 1;
					knight.TakeDamage(1, pushDir);
				}
				/*if (IntersectRect(&temp, &knight.m_rc, &e->m_rc)) {
					int pushDir = (knight.pos.x < e->pos.x) ? -1 : 1;
					knight.TakeDamage(1, pushDir);
				}*/
			}
		}

		// (옵션) 시체가 된 지 3초 후 삭제
		/*if (e->isDead && (GetTickCount() - e->hitStartTime > 3000)) {
			delete e;
			it = m_Enemies.erase(it);
		}
		else {
			++it;
		}*/
		++it;
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
		// 카메라 좌표만큼 빼주기
		m_pCurrentMapChunk->bgLayer[i].Render(0 - CAM->GetX(), 0 - CAM->GetY(), 0, 1, 1);
	}

	for (auto e : m_Enemies) {
		e->Draw();
	}
	// =======================================================
	// 디버그용 텍스트 출력 모음
	// =======================================================
	if (coll.isDebugDraw)
	{
		char debugPrefab[256];
		sprintf_s(debugPrefab, "Current Prefab ID : %d", m_pCurrentMapChunk->prefabID);
		dv_font.DrawString(debugPrefab, 0, 0, D3DCOLOR_ARGB(255, 255, 255, 0));

		char debugPos[256];
		sprintf_s(debugPos, "Knight Pos: X(%.1f), Y(%.1f)", knight.pos.x, knight.pos.y);
		dv_font.DrawString(debugPos, 0, 100, D3DCOLOR_ARGB(255, 0, 255, 255));

		// =======================================================
		// 🚨 [추가] 보스 3형제 실시간 HP & 상태 스캐너!
		// =======================================================
		int textY = 400; // 화면 위쪽부터 아래로 출력
		for (auto e : m_Enemies)
		{
			if (e->type == 3) // 몬스터가 보스(type 3)일 경우
			{
				BossEnemy* b = (BossEnemy*)e;
				char bossInfo[256];

				// ID, 현재 체력, 현재 상태(Sleep, Roar 등)를 출력
				sprintf_s(bossInfo, "[Boss %d] HP : %d / State : %d", b->bossID, b->hp, b->state);

				// 1번(가운데)은 빨간색, 2번/3번(배경)은 회색빛으로 글씨 색깔도 센스있게!
				D3DCOLOR textColor = (b->bossID == 1) ? D3DCOLOR_ARGB(255, 255, 100, 100) : D3DCOLOR_ARGB(255, 150, 150, 150);

				dv_font.DrawString(bossInfo, 10, textY, textColor);
				textY += 30; // 다음 줄로 내림
			}
		}
	}
	// =======================================================
	// 디버그용: 현재 프리팹 번호 화면 출력
	// =======================================================
	//if (coll.isDebugDraw)
	//{
	//	char debugText[256];
	//	sprintf_s(debugText, "Current Prefab ID : %d", m_pCurrentMapChunk->prefabID);
	//	// 2. 좌측 하단: 현재 기사의 절대 좌표(X, Y) 출력 (새로 추가!)
	//	char debugPos[256];
	//	sprintf_s(debugPos, "Knight Pos: X(%.1f), Y(%.1f)", knight.pos.x, knight.pos.y);
	//	dv_font.DrawString(debugText, 0, 0);   //글자출력
	//	dv_font.DrawString(debugPos, 0, 100);
	//}
}

void MapManager::LoadDebugPrefab(int pID)
{
	// 1. 1~15번을 순환하도록 범위 제한
	if (pID < 1) pID = 16;
	if (pID > 16) pID = 1;

	// 혹시 아직 도면을 안 짠 프리팹이면 건너뛰기
	if (m_Prefabs[pID].typeID == 0) return;

	m_DebugPrefabID = pID;

	// =========================================================
	// 1번 방이 아닌, 38번 방(디버그 전용)을 사용합니다!
	// =========================================================
	int debugRoomID = 38;

	m_MapList[debugRoomID].id = debugRoomID;
	m_MapList[debugRoomID].prefabID = pID;
	m_MapList[debugRoomID].width = m_Prefabs[pID].width;
	m_MapList[debugRoomID].height = m_Prefabs[pID].height;
	m_MapList[debugRoomID].layerCount = m_Prefabs[pID].layerCount;

	for (int i = 0; i < m_Prefabs[pID].layerCount; i++)
	{
		m_MapList[debugRoomID].bgLayer[i] = m_Prefabs[pID].bgLayer[i];
	}

	// 3. 38번 방으로 즉시 이동!
	ChangeMap(debugRoomID);

	// 4. 캐릭터를 맵의 가로 중앙 & 맨 위(공중)로 안전하게 스폰
	knight.pos.x = m_Prefabs[pID].width / 2.0f;
	knight.pos.y = 50.0f;
	knight.gravity = 0;
	knight.isDashing = false;
	knight.grounded = false;
}