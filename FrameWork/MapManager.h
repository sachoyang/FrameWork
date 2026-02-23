#pragma once
#include "Include.h"
#include <list> // list 사용을 위해 추가
#include <vector>

// =======================================================
// 프리팹 방 방향 비트 플래그
// =======================================================
#define DOOR_UP    1   // 0001
#define DOOR_DOWN  2   // 0010
#define DOOR_LEFT  4   // 0100
#define DOOR_RIGHT 8   // 1000
// 보스방 고유 ID
#define ROOM_BOSS  16

// =======================================================
// 프리팹(도면) 구조체
// =======================================================
struct RoomPrefab
{
	int typeID;           // 1~15 사이의 값 (문이 뚫린 방향의 합)
	int width;            // 이 방의 가로 크기
	int height;           // 이 방의 세로 크기
	
	int gridW;				// 이 방이 차지하는 그리드 칸 수 (가로)
	int gridH;				// 이 방이 차지하는 그리드 칸 수 (세로)

	Sprite bgLayer[2];    // 배경 이미지 (일단 2개 쓴다고 가정)
	int layerCount;
	// 이 방에 무조건 고정으로 들어갈 벽과 발판들
	std::list<RECT> walls;

	// 특정방향으로 들어왔을 때 플레이어가 스폰될 위치 (문 위치에서 약간 안쪽으로)
	// 인덱스: 1(UP), 2(DOWN), 3(LEFT), 4(RIGHT)
	float spawnX[5];
	float spawnY[5];
};

enum DIR_TYPE { DIR_UP = 1, DIR_DOWN, DIR_LEFT, DIR_RIGHT, DIR_NUM };

struct MapChunk
{
	int id;
	Sprite bgLayer[5];
	int layerCount;

	int nextMapID[DIR_NUM]; // 다음 맵 ID (오른쪽, 왼쪽, 위, 아래)
	// 문 위치나 몬스터 위치 등등

	// 맵 크기 정보
	int width;
	int height;

	int prefabID;
};

class MapManager
{

public :
	MapManager();
	~MapManager();

	MapChunk* m_pCurrentMapChunk;

	// std::map<int, MapChunk*> m_MapList;
	MapChunk m_MapList[13];

	Sprite m_MapImg1_1[6];
	Sprite m_MapImg1_2[45];

	DWORD m_MapImg1_1_ani1;

	int m_MapImg1_1_ani1Count;

	int m_Stage;

	double posX, posY;

	RoomPrefab m_Prefabs[17]; // 1~16번 프리팹 (0번은 빈칸)

	void Init();
	void Update(double frame);
	void Draw();

	void InitPrefabs();
	void CreateRandomMap();
	void ChangeMap(int mapID);

	//디버그용
	int m_DebugPrefabID; // 현재 보고 있는 프리팹 번호
	void LoadDebugPrefab(int pID); // 특정 프리팹을 강제로 로드하는 함수
};

extern MapManager mapMng;