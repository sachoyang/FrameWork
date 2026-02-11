#include "Include.h"

MapManager mapMng;


MapManager::MapManager()
{
	m_Stage = 1;
}

MapManager::~MapManager()
{
}

void MapManager::Init()
{
	int i,j;
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
		
	}

}

void MapManager::Update(double frame)
{
	if(GetTickCount64() - m_MapImg1_1_ani1 > frame)
	{
		if(m_Stage==1)
		{
			m_MapImg1_1_ani1Count++;
			if(m_MapImg1_1_ani1Count > 44) m_MapImg1_1_ani1Count = 0;
		}

		m_MapImg1_1_ani1 = GetTickCount64();
	}
}

void MapManager::Draw()
{
	if(m_Stage==1)
	{
		m_MapImg1_1[0].Render(posX, posY, 0, 1, 1);
		m_MapImg1_1[1].Render(posX, posY, 0, 1, 1);
		m_MapImg1_1[3].Render(posX+270, posY+70, 0, 1, 1);
		m_MapImg1_2[m_MapImg1_1_ani1Count].Render(posX, posY, 0, 1.5, 1.5);
		m_MapImg1_1[2].Render(posX+190, posY, 0, 1, 1);
		m_MapImg1_1[4].Render(posX+900, posY, 0, 1, 1);
		m_MapImg1_1[5].Render(posX, posY, 0, 1, 1);

	}
}