#pragma once
#include "Include.h"

enum DIR_TYPE { DIR_UP = 1, DIR_DOWN, DIR_LEFT, DIR_RIGHT, DIR_NUM };

struct MapChunk
{
	int id;
	Sprite bgLayer[5];
	int layerCount;

	int nextMapID[DIR_NUM]; // 다음 맵 ID (오른쪽, 왼쪽, 위, 아래)
	// 문 위치나 몬스터 위치 등등
};

class MapManager
{

public :
	MapManager();
	~MapManager();

	MapChunk* m_pCurrentMapChunk;

	// std::map<int, MapChunk*> m_MapList;
	MapChunk m_MapList[10];

	/*Sprite test;
	bool tests;*/
	Sprite m_MapImg1_1[6];
	Sprite m_MapImg1_2[45];

	DWORD m_MapImg1_1_ani1;

	int m_MapImg1_1_ani1Count;

	int m_Stage;

	double posX, posY;

	void Init();
	void Update(double frame);
	void Draw();

	void ChangeMap(int mapID);
};

extern MapManager mapMng;