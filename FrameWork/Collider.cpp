#include "Include.h"


Collider coll;


Collider::Collider()
{

}

Collider::~Collider()
{
}

void Collider::Init()
{
	/*m_rc.left = 0;
	m_rc.top = 600;
	m_rc.right = 1250;
	m_rc.bottom = 720;*/
	m_Walls.clear();

}

void Collider::Update()
{
}

// 바닥
void Collider::Draw()
{
	if (Gmanager.m_GameStart == true)
	{
		/*dv_font.DrawString("┌ ", m_rc.left, m_rc.top, D3DCOLOR_ARGB(255, 0, 255, 0));
		dv_font.DrawString("┐ ", m_rc.right, m_rc.top, D3DCOLOR_ARGB(255, 0, 255, 0));
		dv_font.DrawString("└", m_rc.left, m_rc.bottom, D3DCOLOR_ARGB(255, 0, 255, 0));
		dv_font.DrawString(" ┘", m_rc.right, m_rc.bottom, D3DCOLOR_ARGB(255, 0, 255, 0));*/
		// 리스트에 있는 모든 벽 그리기
		for (auto& wall : m_Walls)
		{
			// 벽을 초록색 박스로 그림
			BoxSow(wall, 0, 0, D3DCOLOR_ARGB(255, 0, 255, 0));
		}
	}
}

// 디버그 콜라이더 (x, y는 오프셋)
void Collider::BoxSow(RECT m_rc, long x, long y, D3DCOLOR color)
{
	if (Gmanager.m_GameStart == true /* && 디버그 일때 처리 */)
	{
		/*dv_font.DrawString("┌ ", m_rc.left-x, m_rc.top  - y, color);
		dv_font.DrawString("┐ ", m_rc.right+x, m_rc.top - y, color);
		dv_font.DrawString("└", m_rc.left-x, m_rc.bottom + y, color);
		dv_font.DrawString(" ┘", m_rc.right+x, m_rc.bottom + y, color);*/
		// Draw slightly inside the rect so it's visible
		// Top-Left
		dv_font.DrawString("┌", m_rc.left + 5, m_rc.top + 5, color);

		// Top-Right (Draw slightly left of the right edge)
		dv_font.DrawString("┐", m_rc.right - 15, m_rc.top + 5, color);

		// Bottom-Left (Draw slightly above the bottom edge)
		dv_font.DrawString("└", m_rc.left + 5, m_rc.bottom - 15, color);

		// Bottom-Right
		dv_font.DrawString("┘", m_rc.right - 15, m_rc.bottom - 15, color);
	}

}

