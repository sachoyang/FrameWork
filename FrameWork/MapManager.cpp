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

void MapManager::InitPrefabs()
{
	RECT rc;
	int thickness = 100;
	int pH = 30; // 발판 두께

	// ====================================================================
	// [프리팹 1번] 위(UP) 
	// ====================================================================
	{
		int id = DOOR_UP;
		m_Prefabs[id].typeID = id;
		m_Prefabs[id].gridW = 1; m_Prefabs[id].gridH = 2;
		m_Prefabs[id].width = SCREEN_WITH; m_Prefabs[id].height = SCREEN_HEIGHT * 2;
		m_Prefabs[id].layerCount = 1;

		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;

		SetRect(&rc, 0, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc); // 바닥
		SetRect(&rc, -50, 0, thickness, MH); m_Prefabs[id].walls.push_back(rc);  // 좌벽
		SetRect(&rc, MW - thickness, 0, MW + 50, MH); m_Prefabs[id].walls.push_back(rc); // 우벽
		SetRect(&rc, 0, -50, MW / 2 - 150, thickness); m_Prefabs[id].walls.push_back(rc); // 천장(좌)
		SetRect(&rc, MW / 2 + 150, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc); // 천장(우)

		// [발판 추가] 촘촘한 지그재그 & 중앙 보조 발판 (150px 간격)
		SetRect(&rc, 150, 1350, 400, 1350 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 850, 1200, 1100, 1200 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 450, 1050, 750, 1050 + pH); m_Prefabs[id].walls.push_back(rc); // 중앙
		SetRect(&rc, 100, 900, 350, 900 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 850, 750, 1150, 750 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 450, 600, 700, 600 + pH); m_Prefabs[id].walls.push_back(rc); // 중앙
		SetRect(&rc, 200, 450, 450, 450 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW / 2 - 150, 300, MW / 2 + 150, 300 + pH); m_Prefabs[id].walls.push_back(rc); // 출구 밑
	}

	// ====================================================================
	// [프리팹 2번] 아래(DOWN) 
	// ====================================================================
	{
		int id = DOOR_DOWN;
		m_Prefabs[id].typeID = id;
		m_Prefabs[id].gridW = 1; m_Prefabs[id].gridH = 1;
		m_Prefabs[id].width = SCREEN_WITH; m_Prefabs[id].height = SCREEN_HEIGHT;
		m_Prefabs[id].layerCount = 1;

		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;

		SetRect(&rc, 0, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc); // 천장
		SetRect(&rc, -50, 0, thickness, MH); m_Prefabs[id].walls.push_back(rc); // 좌
		SetRect(&rc, MW - thickness, 0, MW + 50, MH); m_Prefabs[id].walls.push_back(rc); // 우
		SetRect(&rc, 0, floorY, MW / 2 - 150, MH + 50); m_Prefabs[id].walls.push_back(rc); // 바닥(좌)
		SetRect(&rc, MW / 2 + 150, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc); // 바닥(우)

		SetRect(&rc, 300, 500, 500, 500 + pH); m_Prefabs[id].walls.push_back(rc); // 보조 발판
		SetRect(&rc, 750, 500, 950, 500 + pH); m_Prefabs[id].walls.push_back(rc);
	}

	// ====================================================================
	// [프리팹 3번] 위, 아래(UP, DOWN) 
	// ====================================================================
	{
		int id = DOOR_UP | DOOR_DOWN;
		m_Prefabs[id].typeID = id;
		m_Prefabs[id].gridW = 2; m_Prefabs[id].gridH = 2;
		m_Prefabs[id].width = SCREEN_WITH * 2; m_Prefabs[id].height = SCREEN_HEIGHT * 2;
		m_Prefabs[id].layerCount = 1;

		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;

		SetRect(&rc, -50, 0, thickness, MH); m_Prefabs[id].walls.push_back(rc); // 좌
		SetRect(&rc, MW - thickness, 0, MW + 50, MH); m_Prefabs[id].walls.push_back(rc); // 우
		SetRect(&rc, 0, -50, MW / 2 - 150, thickness); m_Prefabs[id].walls.push_back(rc); // 천장
		SetRect(&rc, MW / 2 + 150, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 0, floorY, MW / 2 - 200, MH + 50); m_Prefabs[id].walls.push_back(rc); // 바닥
		SetRect(&rc, MW / 2 + 200, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);

		// [발판 추가] 웅덩이 구조 + 촘촘한 공중 섬
		SetRect(&rc, 0, 1150, 600, 1200); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW - 600, 1000, MW, 1050); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 0, 850, 500, 900); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW - 500, 650, MW, 700); m_Prefabs[id].walls.push_back(rc);

		SetRect(&rc, 800, 1350, 1000, 1350 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 1300, 1200, 1500, 1200 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 900, 1050, 1100, 1050 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 1400, 850, 1600, 850 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 1000, 650, 1200, 650 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 1200, 450, 1450, 450 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW / 2 - 150, 300, MW / 2 + 150, 300 + pH); m_Prefabs[id].walls.push_back(rc); // 출구 밑
	}

	// ====================================================================
	// [프리팹 4번] 왼쪽(LEFT)
	// ====================================================================
	{
		int id = DOOR_LEFT;
		m_Prefabs[id].typeID = id;
		m_Prefabs[id].gridW = 2; m_Prefabs[id].gridH = 2;
		m_Prefabs[id].width = SCREEN_WITH * 2; m_Prefabs[id].height = SCREEN_HEIGHT * 2;
		m_Prefabs[id].layerCount = 1;

		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;

		SetRect(&rc, 0, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc); // 천장
		SetRect(&rc, 0, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc); // 바닥
		SetRect(&rc, MW - thickness, 0, MW + 50, MH); m_Prefabs[id].walls.push_back(rc); // 우

		SetRect(&rc, -50, 0, thickness, SCREEN_HEIGHT); m_Prefabs[id].walls.push_back(rc); // 좌(상) 막음
		SetRect(&rc, -50, SCREEN_HEIGHT, thickness, floorY - 200); m_Prefabs[id].walls.push_back(rc); // 문

		// [발판 추가] 완만한 피라미드
		SetRect(&rc, 500, 1350, 2000, floorY); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 700, 1200, 1800, 1350); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 900, 1050, 1600, 1200); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 1100, 900, 1400, 1050);  m_Prefabs[id].walls.push_back(rc);

		SetRect(&rc, 200, 1150, 400, 1150 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW - 400, 1000, MW - 200, 1000 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW / 2 - 100, 700, MW / 2 + 100, 700 + pH); m_Prefabs[id].walls.push_back(rc); // 꼭대기 장식
	}

	// ====================================================================
	// [프리팹 5번] 위, 왼쪽 (UP, LEFT)
	// ====================================================================
	{
		int id = DOOR_UP | DOOR_LEFT;
		m_Prefabs[id].typeID = id;
		m_Prefabs[id].gridW = 2; m_Prefabs[id].gridH = 2;
		m_Prefabs[id].width = SCREEN_WITH * 2; m_Prefabs[id].height = SCREEN_HEIGHT * 2;
		m_Prefabs[id].layerCount = 1;

		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;

		SetRect(&rc, 0, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW - thickness, 0, MW + 50, MH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 0, -50, MW / 2 - 150, thickness); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW / 2 + 150, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, -50, 0, thickness, SCREEN_HEIGHT); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, -50, SCREEN_HEIGHT, thickness, floorY - 200); m_Prefabs[id].walls.push_back(rc);

		// [발판 추가] 촘촘한 나선형
		SetRect(&rc, 300, 1350, 600, 1350 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 800, 1200, 1100, 1200 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 1300, 1050, 1600, 1050 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 1800, 900, 2200, 900 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 1400, 750, 1650, 750 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 1000, 600, 1250, 600 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 800, 450, 1050, 450 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW / 2 - 150, 300, MW / 2 + 150, 300 + pH); m_Prefabs[id].walls.push_back(rc);
	}

	// ====================================================================
	// [프리팹 6번] 아래, 왼쪽 (DOWN, LEFT) - 🌟 왼쪽 문이 위(TOP)에 있음
	// ====================================================================
	{
		int id = DOOR_DOWN | DOOR_LEFT;
		m_Prefabs[id].typeID = id;
		m_Prefabs[id].gridW = 1; m_Prefabs[id].gridH = 2;
		m_Prefabs[id].width = SCREEN_WITH; m_Prefabs[id].height = SCREEN_HEIGHT * 2;
		m_Prefabs[id].layerCount = 1;

		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;

		SetRect(&rc, 0, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW - thickness, 0, MW + 50, MH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 0, floorY, MW / 2 - 150, MH + 50); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW / 2 + 150, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, -50, 0, thickness, 600); m_Prefabs[id].walls.push_back(rc); // 상단 문 (뚫림)
		SetRect(&rc, -50, 800, thickness, MH); m_Prefabs[id].walls.push_back(rc); // 하단 막음

		// [발판 추가] 떨어지면서 밟기 (촘촘)
		SetRect(&rc, 100, 750, 350, 750 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 500, 900, 800, 900 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 250, 1050, 500, 1050 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 650, 1200, 950, 1200 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 300, 1350, 550, 1350 + pH); m_Prefabs[id].walls.push_back(rc);
	}

	// ====================================================================
	// [프리팹 7번] 위, 아래, 왼쪽 (UP, DOWN, LEFT) 
	// ====================================================================
	{
		int id = DOOR_UP | DOOR_DOWN | DOOR_LEFT;
		m_Prefabs[id].typeID = id;
		m_Prefabs[id].gridW = 2; m_Prefabs[id].gridH = 2;
		m_Prefabs[id].width = SCREEN_WITH * 2; m_Prefabs[id].height = SCREEN_HEIGHT * 2;
		m_Prefabs[id].layerCount = 1;

		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;

		SetRect(&rc, MW - thickness, 0, MW + 50, MH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 0, -50, MW / 2 - 150, thickness); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW / 2 + 150, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 0, floorY, MW / 2 - 150, MH + 50); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW / 2 + 150, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, -50, 0, thickness, SCREEN_HEIGHT); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, -50, SCREEN_HEIGHT, thickness, floorY - 200); m_Prefabs[id].walls.push_back(rc);

		// [발판 추가] 촘촘한 구름 다리
		SetRect(&rc, 300, 1350, 600, 1350 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 750, 1200, 1050, 1200 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW / 2 + 100, 1050, MW / 2 + 400, 1050 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW / 2 - 200, 900, MW / 2 + 50, 900 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW / 2 + 150, 750, MW / 2 + 450, 750 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW / 2 - 300, 600, MW / 2 - 50, 600 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW / 2 + 50, 450, MW / 2 + 350, 450 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW / 2 - 150, 300, MW / 2 + 150, 300 + pH); m_Prefabs[id].walls.push_back(rc);
	}

	// ====================================================================
	// [프리팹 8번] 오른쪽 (RIGHT) - 시작 방
	// ====================================================================
	{
		int id = DOOR_RIGHT;
		m_Prefabs[id].typeID = id;
		m_Prefabs[id].gridW = 1; m_Prefabs[id].gridH = 1;
		m_Prefabs[id].width = SCREEN_WITH; m_Prefabs[id].height = SCREEN_HEIGHT;
		m_Prefabs[id].layerCount = 1;

		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;

		SetRect(&rc, 0, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 0, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, -50, 0, thickness, MH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW - thickness, 0, MW + 50, floorY - 200); m_Prefabs[id].walls.push_back(rc); // 문
	}

	// ====================================================================
	// [프리팹 9번] 위, 오른쪽 (UP, RIGHT)
	// ====================================================================
	{
		int id = DOOR_UP | DOOR_RIGHT;
		m_Prefabs[id].typeID = id;
		m_Prefabs[id].gridW = 1; m_Prefabs[id].gridH = 2;
		m_Prefabs[id].width = SCREEN_WITH; m_Prefabs[id].height = SCREEN_HEIGHT * 2;
		m_Prefabs[id].layerCount = 1;

		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;

		SetRect(&rc, 0, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, -50, 0, thickness, MH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 0, -50, MW / 2 - 150, thickness); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW / 2 + 150, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW - thickness, 0, MW + 50, SCREEN_HEIGHT); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW - thickness, SCREEN_HEIGHT, MW + 50, floorY - 200); m_Prefabs[id].walls.push_back(rc);

		// [발판 추가] 촘촘한 좌우 이동
		SetRect(&rc, MW - 400, 1350, MW - 100, 1350 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 200, 1200, 500, 1200 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 600, 1050, 900, 1050 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 150, 900, 450, 900 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW / 2, 750, MW / 2 + 300, 750 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 150, 600, 450, 600 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 500, 450, 800, 450 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW / 2 - 150, 300, MW / 2 + 150, 300 + pH); m_Prefabs[id].walls.push_back(rc);
	}

	// ====================================================================
	// [프리팹 10번] 아래, 오른쪽 (DOWN, RIGHT) 
	// ====================================================================
	{
		int id = DOOR_DOWN | DOOR_RIGHT;
		m_Prefabs[id].typeID = id;
		m_Prefabs[id].gridW = 2; m_Prefabs[id].gridH = 1;
		m_Prefabs[id].width = SCREEN_WITH * 2; m_Prefabs[id].height = SCREEN_HEIGHT;
		m_Prefabs[id].layerCount = 1;

		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;

		SetRect(&rc, 0, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, -50, 0, thickness, MH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 0, floorY, MW / 2 - 200, MH + 50); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW / 2 + 200, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW - thickness, 0, MW + 50, floorY - 200); m_Prefabs[id].walls.push_back(rc);

		// [발판 추가] 촘촘한 징검다리
		SetRect(&rc, 600, 600, 800, 600 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 1000, 500, 1200, 500 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 1400, 600, 1600, 600 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 1800, 500, 2000, 500 + pH); m_Prefabs[id].walls.push_back(rc);
	}

	// ====================================================================
	// [프리팹 11번] 위, 아래, 오른쪽 (UP, DOWN, RIGHT) 
	// ====================================================================
	{
		int id = DOOR_UP | DOOR_DOWN | DOOR_RIGHT;
		m_Prefabs[id].typeID = id;
		m_Prefabs[id].gridW = 1; m_Prefabs[id].gridH = 2;
		m_Prefabs[id].width = SCREEN_WITH; m_Prefabs[id].height = SCREEN_HEIGHT * 2;
		m_Prefabs[id].layerCount = 1;

		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;

		SetRect(&rc, -50, 0, thickness, MH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 0, -50, MW / 2 - 150, thickness); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW / 2 + 150, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 0, floorY, MW / 2 - 150, MH + 50); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW / 2 + 150, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW - thickness, 0, MW + 50, SCREEN_HEIGHT); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW - thickness, SCREEN_HEIGHT, MW + 50, floorY - 200); m_Prefabs[id].walls.push_back(rc);

		// [발판 추가] 수직 엘리베이터 (150 간격)
		SetRect(&rc, 600, 1350, 900, 1350 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 150, 1200, 450, 1200 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 700, 1050, 1000, 1050 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 200, 900, 500, 900 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 650, 750, 950, 750 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 150, 600, 450, 600 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 600, 450, 900, 450 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW / 2 - 150, 300, MW / 2 + 150, 300 + pH); m_Prefabs[id].walls.push_back(rc);
	}

	// ====================================================================
	// [프리팹 12번] 왼쪽, 오른쪽 (LEFT, RIGHT) 
	// ====================================================================
	{
		int id = DOOR_LEFT | DOOR_RIGHT;
		m_Prefabs[id].typeID = id;
		m_Prefabs[id].gridW = 2; m_Prefabs[id].gridH = 1;
		m_Prefabs[id].width = SCREEN_WITH * 2; m_Prefabs[id].height = SCREEN_HEIGHT;
		m_Prefabs[id].layerCount = 1;

		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;

		SetRect(&rc, 0, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 0, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, -50, 0, thickness, floorY - 200); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW - thickness, 0, MW + 50, floorY - 200); m_Prefabs[id].walls.push_back(rc);

		// [발판 추가] 바리케이드 완화
		SetRect(&rc, 1000, 500, 1560, floorY); m_Prefabs[id].walls.push_back(rc); // 중앙 장애물 (낮춤)
		SetRect(&rc, 600, 600, 900, 630); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 1660, 600, 1960, 630); m_Prefabs[id].walls.push_back(rc);
	}

	// ====================================================================
	// [프리팹 13번] 위, 왼쪽, 오른쪽 (UP, LEFT, RIGHT) 
	// ====================================================================
	{
		int id = DOOR_UP | DOOR_LEFT | DOOR_RIGHT;
		m_Prefabs[id].typeID = id;
		m_Prefabs[id].gridW = 2; m_Prefabs[id].gridH = 2;
		m_Prefabs[id].width = SCREEN_WITH * 2; m_Prefabs[id].height = SCREEN_HEIGHT * 2;
		m_Prefabs[id].layerCount = 1;

		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;

		SetRect(&rc, 0, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 0, -50, MW / 2 - 150, thickness); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW / 2 + 150, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, -50, 0, thickness, SCREEN_HEIGHT); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, -50, SCREEN_HEIGHT, thickness, floorY - 200); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW - thickness, 0, MW + 50, SCREEN_HEIGHT); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW - thickness, SCREEN_HEIGHT, MW + 50, floorY - 200); m_Prefabs[id].walls.push_back(rc);

		// [발판 추가] 산맥 등반로 완화 (150 간격)
		SetRect(&rc, 900, 1350, 1660, floorY); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 500, 1200, 800, 1200 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 1760, 1200, 2060, 1200 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 850, 1050, 1150, 1050 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 1410, 1050, 1710, 1050 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 1000, 900, 1300, 900 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 1260, 750, 1560, 750 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 1000, 600, 1300, 600 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 1150, 450, 1450, 450 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW / 2 - 150, 300, MW / 2 + 150, 300 + pH); m_Prefabs[id].walls.push_back(rc);
	}

	// ====================================================================
	// [프리팹 14번] 아래, 왼쪽, 오른쪽 (DOWN, LEFT, RIGHT) 
	// ====================================================================
	{
		int id = DOOR_DOWN | DOOR_LEFT | DOOR_RIGHT;
		m_Prefabs[id].typeID = id;
		m_Prefabs[id].gridW = 2; m_Prefabs[id].gridH = 1;
		m_Prefabs[id].width = SCREEN_WITH * 2; m_Prefabs[id].height = SCREEN_HEIGHT;
		m_Prefabs[id].layerCount = 1;

		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;

		SetRect(&rc, 0, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 0, floorY, MW / 2 - 300, MH + 50); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW / 2 + 300, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, -50, 0, thickness, floorY - 200); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW - thickness, 0, MW + 50, floorY - 200); m_Prefabs[id].walls.push_back(rc);

		// [발판 추가] 촘촘한 흔들다리
		SetRect(&rc, 700, 650, 900, 650 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 1050, 550, 1250, 550 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 1400, 550, 1600, 550 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 1750, 650, 1950, 650 + pH); m_Prefabs[id].walls.push_back(rc);
	}

	// ====================================================================
	// [프리팹 15번] 상, 하, 좌, 우 모두 뚫림 
	// ====================================================================
	{
		int id = DOOR_UP | DOOR_DOWN | DOOR_LEFT | DOOR_RIGHT;
		m_Prefabs[id].typeID = id;
		m_Prefabs[id].gridW = 2; m_Prefabs[id].gridH = 2;
		m_Prefabs[id].width = SCREEN_WITH * 2; m_Prefabs[id].height = SCREEN_HEIGHT * 2;
		m_Prefabs[id].layerCount = 1;

		int MW = m_Prefabs[id].width; int MH = m_Prefabs[id].height; int floorY = MH - thickness;

		SetRect(&rc, 0, -50, MW / 2 - 200, thickness); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW / 2 + 200, -50, MW, thickness); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 0, floorY, MW / 2 - 200, MH + 50); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW / 2 + 200, floorY, MW, MH + 50); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, -50, 0, thickness, SCREEN_HEIGHT); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, -50, SCREEN_HEIGHT, thickness, floorY - 200); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW - thickness, 0, MW + 50, SCREEN_HEIGHT); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW - thickness, SCREEN_HEIGHT, MW + 50, floorY - 200); m_Prefabs[id].walls.push_back(rc);

		// [발판 추가] 거대하고 촘촘한 정거장
		SetRect(&rc, 1000, 1200, 1560, 1200 + pH); m_Prefabs[id].walls.push_back(rc); // 중앙 베이스
		SetRect(&rc, 1150, 1350, 1410, 1350 + pH); m_Prefabs[id].walls.push_back(rc);

		SetRect(&rc, 300, 1300, 550, 1300 + pH); m_Prefabs[id].walls.push_back(rc); // 좌
		SetRect(&rc, 700, 1250, 950, 1250 + pH); m_Prefabs[id].walls.push_back(rc);

		SetRect(&rc, 2000, 1300, 2250, 1300 + pH); m_Prefabs[id].walls.push_back(rc); // 우
		SetRect(&rc, 1600, 1250, 1850, 1250 + pH); m_Prefabs[id].walls.push_back(rc);

		SetRect(&rc, 1000, 1050, 1250, 1050 + pH); m_Prefabs[id].walls.push_back(rc); // 위로 가는 길
		SetRect(&rc, 1300, 900, 1550, 900 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 1000, 750, 1250, 750 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 1300, 600, 1550, 600 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, 1000, 450, 1250, 450 + pH); m_Prefabs[id].walls.push_back(rc);
		SetRect(&rc, MW / 2 - 150, 300, MW / 2 + 150, 300 + pH); m_Prefabs[id].walls.push_back(rc);
	}


	// ====================================================================
	// 🌟 [자동 스폰 위치 계산기] (모든 1~15번 방 일괄 적용)
	// ====================================================================
	for (int i = 1; i <= 15; i++)
	{
		if (m_Prefabs[i].typeID == 0) continue;

		float pWidth = (float)m_Prefabs[i].width;
		float pHeight = (float)m_Prefabs[i].height;

		// 1. 위에서 떨어질 때 (중앙 상단)
		m_Prefabs[i].spawnX[DIR_UP] = pWidth / 2.0f;
		m_Prefabs[i].spawnY[DIR_UP] = 50.0f;

		// 2. 아래에서 솟아오를 때 (중앙 하단)
		m_Prefabs[i].spawnX[DIR_DOWN] = pWidth / 2.0f;
		m_Prefabs[i].spawnY[DIR_DOWN] = pHeight - 150.0f;

		// 3. 왼쪽 문으로 들어올 때 (대부분 좌측 하단 문)
		m_Prefabs[i].spawnX[DIR_LEFT] = 50.0f;
		m_Prefabs[i].spawnY[DIR_LEFT] = pHeight - 150.0f;

		// 4. 오른쪽 문으로 들어올 때 (대부분 우측 하단 문)
		m_Prefabs[i].spawnX[DIR_RIGHT] = pWidth - 100.0f;
		m_Prefabs[i].spawnY[DIR_RIGHT] = pHeight - 150.0f;
	}

	// [예외 처리] 왼쪽 문이 '상단'에 뚫려있는 6번 프리팹만 Y좌표 수정!
	m_Prefabs[6].spawnY[DIR_LEFT] = SCREEN_HEIGHT - 150.0f; // y = 650 (위쪽 칸)
}

// MapManager.cpp

void MapManager::CreateRandomMap()
{
	// [1] 맵 초기화
	for (int i = 1; i <= 10; i++)
	{
		for (int j = 0; j < 5; j++) m_MapList[i].nextMapID[j] = 0;
		m_MapList[i].prefabID = 0;
	}
	int grid[15][15] = { 0, };

	// [2] 1번 방(시작 방) 배치: 무조건 '오른쪽'이 뚫린 8번 프리팹 사용
	m_MapList[1].id = 1;
	m_MapList[1].prefabID = DOOR_RIGHT; // 프리팹 8번
	m_MapList[1].width = m_Prefabs[DOOR_RIGHT].width;
	m_MapList[1].height = m_Prefabs[DOOR_RIGHT].height;
	grid[7][7] = 1; // 15x15 그리드의 정중앙(7,7)에 배치

	int currentMapCount = 1;
	int maxMapCount = 10;
	int failCount = 0; // 무한 루프 방지용

	// [3] 레고 블록 조립 (절차적 생성 시작!)
	while (currentMapCount < maxMapCount && failCount < 1000)
	{
		failCount++;

		// 3-1. 이미 배치된 방들 중 무작위로 하나 선택
		int randRoomID = (rand() % currentMapCount) + 1;
		int pID = m_MapList[randRoomID].prefabID;

		// 3-2. 그 방의 문 4방향 중 하나 랜덤 선택
		int dirs[4] = { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT };
		int checkDir = dirs[rand() % 4];

		// 선택한 방향이 프리팹 도면상 진짜 뚫려있는 문인지 비트마스크로 확인
		int bitFlag = 0;
		if (checkDir == DIR_UP) bitFlag = DOOR_UP;
		else if (checkDir == DIR_DOWN) bitFlag = DOOR_DOWN;
		else if (checkDir == DIR_LEFT) bitFlag = DOOR_LEFT;
		else if (checkDir == DIR_RIGHT) bitFlag = DOOR_RIGHT;

		if ((pID & bitFlag) == 0) continue; // 꽉 막힌 벽이면 취소
		if (m_MapList[randRoomID].nextMapID[checkDir] != 0) continue; // 이미 다른 방과 연결된 문이어도 취소

		// 3-3. 이 방의 그리드 좌표 찾기 (방이 여러 칸을 차지하면 가장 좌측 상단 기준)
		int rx = -1, ry = -1;
		for (int y = 0; y < 15; y++) {
			for (int x = 0; x < 15; x++) {
				if (grid[y][x] == randRoomID) {
					rx = x; ry = y; break;
				}
			}
			if (rx != -1) break;
		}

		// 3-4. 새 방이 들어갈 타겟 좌표 계산
		int targetX = rx, targetY = ry;
		if (checkDir == DIR_UP) targetY--;
		else if (checkDir == DIR_DOWN) targetY += m_Prefabs[pID].gridH; // 현재 방의 세로 칸 수만큼 밑으로
		else if (checkDir == DIR_LEFT) targetX--;
		else if (checkDir == DIR_RIGHT) targetX += m_Prefabs[pID].gridW; // 현재 방의 가로 칸 수만큼 우측으로

		if (targetX < 0 || targetX >= 15 || targetY < 0 || targetY >= 15) continue; // 맵 밖으로 나가면 취소

		// 3-5. 연결될 수 있는 "반대쪽 문"을 가진 프리팹들만 모으기
		int oppositeBit = 0;
		if (checkDir == DIR_UP) oppositeBit = DOOR_DOWN;
		else if (checkDir == DIR_DOWN) oppositeBit = DOOR_UP;
		else if (checkDir == DIR_LEFT) oppositeBit = DOOR_RIGHT;
		else if (checkDir == DIR_RIGHT) oppositeBit = DOOR_LEFT;

		std::vector<int> validPrefabs;
		for (int i = 1; i <= 15; i++) {
			if (m_Prefabs[i].typeID == 0) continue; // 아직 안 만든 도면 패스
			if ((m_Prefabs[i].typeID & oppositeBit) != 0) { // 반대쪽 문이 있다면 후보에 추가!
				validPrefabs.push_back(i);
			}
		}
		if (validPrefabs.empty()) continue;

		// 3-6. 후보 중 랜덤으로 하나 뽑아서 "그리드에 들어갈 공간(1x2, 2x2 등)이 있는지" 검사
		int newPrefabID = validPrefabs[rand() % validPrefabs.size()];
		int gw = m_Prefabs[newPrefabID].gridW;
		int gh = m_Prefabs[newPrefabID].gridH;

		bool isSpaceFree = true;
		if (targetX + gw > 15 || targetY + gh > 15) isSpaceFree = false; // 배열 넘어가면 컷
		else {
			for (int y = 0; y < gh; y++) {
				for (int x = 0; x < gw; x++) {
					if (grid[targetY + y][targetX + x] != 0) {
						isSpaceFree = false; // 다른 방이 이미 자리를 차지하고 있으면 컷
						break;
					}
				}
				if (!isSpaceFree) break;
			}
		}

		if (!isSpaceFree) continue; // 공간 없으면 배치 포기하고 다시 처음부터

		// ==========================================================
		// 모든 조건 통과! 새 방을 생성하고 연결합니다.
		// ==========================================================
		currentMapCount++;
		int newRoomID = currentMapCount;

		// 그리드 영역 차지 (예: 2x2 방이면 4칸에 모두 newRoomID 기록)
		for (int y = 0; y < gh; y++) {
			for (int x = 0; x < gw; x++) {
				grid[targetY + y][targetX + x] = newRoomID;
			}
		}

		// 새 방의 정보 저장
		m_MapList[newRoomID].id = newRoomID;
		m_MapList[newRoomID].prefabID = newPrefabID;
		m_MapList[newRoomID].width = m_Prefabs[newPrefabID].width;
		m_MapList[newRoomID].height = m_Prefabs[newPrefabID].height;
		m_MapList[newRoomID].layerCount = m_Prefabs[newPrefabID].layerCount;

		// 배경 이미지도 프리팹에서 가져와서 연결
		for (int layer = 0; layer < m_MapList[newRoomID].layerCount; layer++) {
			m_MapList[newRoomID].bgLayer[layer] = m_Prefabs[newPrefabID].bgLayer[layer];
		}

		// 기존 방과 새 방의 문(포탈) 서로 연결!
		m_MapList[randRoomID].nextMapID[checkDir] = newRoomID;

		int oppositeDir = 0;
		if (checkDir == DIR_UP) oppositeDir = DIR_DOWN;
		else if (checkDir == DIR_DOWN) oppositeDir = DIR_UP;
		else if (checkDir == DIR_LEFT) oppositeDir = DIR_RIGHT;
		else if (checkDir == DIR_RIGHT) oppositeDir = DIR_LEFT;

		m_MapList[newRoomID].nextMapID[oppositeDir] = randRoomID;
	}
}


void MapManager::ChangeMap(int mapID)
{
	if (mapID < 1 || mapID > 10) return;

	m_pCurrentMapChunk = &m_MapList[mapID];

	CAM->SetMapSize(m_pCurrentMapChunk->width, m_pCurrentMapChunk->height);
	// [필수] 맵 바뀔 때 상태 리셋
	knight.isDashing = false;
	knight.gravity = 0;
	knight.isMove = false;
	// 1. Clear existing walls
	coll.ClearWalls();

	// =================================================================
	// [4단계 핵심 로직] 프리팹 도면 복사 붙여넣기!
	// =================================================================
	int pID = m_pCurrentMapChunk->prefabID; // 이 방이 무슨 도면을 썼는지 가져옴

	// 혹시 에러로 프리팹이 배정 안 된 방이라면? (안전장치: 안 떨어지게 바닥만 생성)
	if (pID == 0 || m_Prefabs[pID].walls.empty())
	{
		RECT rc;
		SetRect(&rc, 0, m_pCurrentMapChunk->height - 100, m_pCurrentMapChunk->width, m_pCurrentMapChunk->height + 50);
		coll.AddWall(rc);
		return;
	}

	// 도면에 있는 수많은 벽과 발판들을 그냥 리스트에서 꺼내서 맵에 쫙! 뿌립니다.
	for (auto& prefabWall : m_Prefabs[pID].walls)
	{
		coll.AddWall(prefabWall);
	}

	//RECT rc;
	//int thickness = 100; // Wall thickness
	////int floorY = SCREEN_HEIGHT - thickness; // The top Y of the floor (e.g., 500 if height is 600)
	//int MW = m_pCurrentMapChunk->width;
	//int MH = m_pCurrentMapChunk->height;
	//int floorY = MH - thickness; // 바닥 위치도 맵 높이 기준!
	//// =============================================================
	//// 1. FLOOR (Bottom) Handling
	//// =============================================================
	//if (m_pCurrentMapChunk->nextMapID[DIR_DOWN] == 0)
	//{
	//	// [No Path] Solid floor
	//	// Top: 500, Bottom: 600 (or slightly more to catch falling)
	//	SetRect(&rc, 0, floorY, MW, MH + 50);
	//	coll.AddWall(rc);
	//}
	//else
	//{
	//	// [Path Exists] Hole in the middle
	//	int holeSize = 200;
	//	int midX = MW / 2;
	//	// Left Floor Piece (0 to 300)
	//	SetRect(&rc, 0, floorY, midX - (holeSize / 2), MH + 50);
	//	coll.AddWall(rc);
	//	// Right Floor Piece (500 to 800)
	//	SetRect(&rc, midX + (holeSize / 2), floorY, MW, MH + 50);
	//	coll.AddWall(rc);
	//}
	//// =============================================================
	//// 2. CEILING (Top) Handling
	//// =============================================================
	//if (m_pCurrentMapChunk->nextMapID[DIR_UP] == 0)
	//{
	//	// [No Path] Solid ceiling
	//	SetRect(&rc, 0, -50, MW, thickness); // -50 to 100
	//	coll.AddWall(rc);
	//}
	//else
	//{	// 위가 뚫려있으면 중간에 밟고 올라갈 수 있는 발판 추가
	//	int pW = 200; // 발판 넓이
	//	int pH = 30;  // 발판 두께
	//	// 맵 정중앙, 바닥에서 적당히 높은 곳에 배치 (여기선 바닥 위 400px 지점 예시)
	//	int platformY = floorY - 350;
	//	RECT platform;
	//	SetRect(&platform, (MW / 2) - (pW / 2), platformY, (MW / 2) + (pW / 2), platformY + pH);
	//	coll.AddWall(platform);
	//}
	//// Else: No ceiling wall (allow jumping up)
	//// =============================================================
	//// 3. LEFT WALL Handling
	//// =============================================================
	//if (m_pCurrentMapChunk->nextMapID[DIR_LEFT] == 0)
	//{
	//	// [No Path] Solid left wall
	//	SetRect(&rc, -50, 0, thickness, MH);
	//	coll.AddWall(rc);
	//}
	//else
	//{
	//	// [Path Exists] Doorway (Wall only on top part)
	//	// Adjust 300 to control door height
	//	SetRect(&rc, -50, 0, thickness, floorY - 200);
	//	coll.AddWall(rc);
	//}
	//// =============================================================
	//// 4. RIGHT WALL Handling
	//// =============================================================
	//if (m_pCurrentMapChunk->nextMapID[DIR_RIGHT] == 0)
	//{
	//	// [No Path] Solid right wall
	//	SetRect(&rc, MW - thickness, 0, MW + 50, MH);
	//	coll.AddWall(rc);
	//}
	//else
	//{
	//	// [Path Exists] Doorway
	//	SetRect(&rc, MW - thickness, 0, MW + 50, floorY - 200);
	//	coll.AddWall(rc);
	//}
}


void MapManager::Update(double frame)
{
	if (m_pCurrentMapChunk == nullptr) return;

	int MW = m_pCurrentMapChunk->width;
	int MH = m_pCurrentMapChunk->height;

	// 1. 오른쪽으로 나갈 때 (-> 다음 방의 '왼쪽 문'으로 스폰됨)
	if (knight.pos.x > MW)
	{
		int nextMap = m_pCurrentMapChunk->nextMapID[DIR_RIGHT];
		if (nextMap != 0)
		{
			ChangeMap(nextMap);
			int pID = m_pCurrentMapChunk->prefabID;
			// 🌟 다음 방(pID)의 "왼쪽 문" 스폰 좌표로 딱 맞춰서 이동!
			knight.pos.x = m_Prefabs[pID].spawnX[DIR_LEFT];
			knight.pos.y = m_Prefabs[pID].spawnY[DIR_LEFT];

			return;
		}
		else knight.pos.x = MW;
	}

	// 2. 왼쪽으로 나갈 때 (-> 다음 방의 '오른쪽 문'으로 스폰됨)
	else if (knight.pos.x < 0)
	{
		int nextMap = m_pCurrentMapChunk->nextMapID[DIR_LEFT];
		if (nextMap != 0)
		{
			ChangeMap(nextMap);
			int pID = m_pCurrentMapChunk->prefabID;
			// 🌟 다음 방(pID)의 "오른쪽 문" 스폰 좌표로 딱 맞춰서 이동!
			knight.pos.x = m_Prefabs[pID].spawnX[DIR_RIGHT];
			knight.pos.y = m_Prefabs[pID].spawnY[DIR_RIGHT];

			return;
		}
		else knight.pos.x = 0;
	}

	// 3. 위로 올라갈 때 (-> 다음 방의 '아랫 문'으로 스폰됨)
	if (knight.pos.y < 0)
	{
		int nextMap = m_pCurrentMapChunk->nextMapID[DIR_UP];
		if (nextMap != 0)
		{
			ChangeMap(nextMap);
			int pID = m_pCurrentMapChunk->prefabID;
			knight.pos.x = m_Prefabs[pID].spawnX[DIR_DOWN];
			knight.pos.y = m_Prefabs[pID].spawnY[DIR_DOWN];

			// 솟아오르는 관성 유지 (기존처럼)
			knight.gravity = -12.0f;

			return;
		}
		else knight.pos.y = 0;
	}

	// 4. 아래로 떨어질 때 (-> 다음 방의 '윗 문'으로 스폰됨)
	else if (knight.pos.y > MH)
	{
		int nextMap = m_pCurrentMapChunk->nextMapID[DIR_DOWN];
		if (nextMap != 0)
		{
			ChangeMap(nextMap);
			int pID = m_pCurrentMapChunk->prefabID;
			knight.pos.x = m_Prefabs[pID].spawnX[DIR_UP];
			knight.pos.y = m_Prefabs[pID].spawnY[DIR_UP];

			return;
		}
		else
		{
			knight.pos.y = MH - 100.0f;
			knight.grounded = true;
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
		// 카메라 좌표만큼 빼주기
		m_pCurrentMapChunk->bgLayer[i].Render(0 - CAM->GetX(), 0 - CAM->GetY(), 0, 1, 1);
	}

	// =======================================================
	// 디버그용: 현재 프리팹 번호 화면 출력
	// =======================================================
	char debugText[256];
	sprintf_s(debugText, "Current Prefab ID : %d", m_pCurrentMapChunk->prefabID);

	dv_font.DrawString(debugText, 0, 0);   //글자출력
}

void MapManager::LoadDebugPrefab(int pID)
{
	// 1. 1~15번을 순환하도록 범위 제한
	if (pID < 1) pID = 15;
	if (pID > 15) pID = 1;

	// 혹시 아직 도면을 안 짠 프리팹이면 건너뛰기
	if (m_Prefabs[pID].typeID == 0) return;

	m_DebugPrefabID = pID;

	// 2. 1번 맵을 강제로 현재 선택한 프리팹 도면으로 덮어씌움
	m_MapList[1].id = 1;
	m_MapList[1].prefabID = pID;
	m_MapList[1].width = m_Prefabs[pID].width;
	m_MapList[1].height = m_Prefabs[pID].height;
	m_MapList[1].layerCount = m_Prefabs[pID].layerCount;

	for (int i = 0; i < m_Prefabs[pID].layerCount; i++)
	{
		m_MapList[1].bgLayer[i] = m_Prefabs[pID].bgLayer[i];
	}

	// 3. 1번 방으로 즉시 이동! (콜라이더 생성 및 카메라 세팅)
	ChangeMap(1);

	// 4. 캐릭터를 맵의 가로 중앙 & 맨 위(공중)로 안전하게 스폰
	knight.pos.x = m_Prefabs[pID].width / 2.0f;
	knight.pos.y = 50.0f;
	knight.gravity = 0;
	knight.isDashing = false;
	knight.grounded = false;
}