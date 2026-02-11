#pragma once
#include "Include.h"

class Collider
{


public :
	Collider();
	~Collider();

	RECT m_rc;

	void Init();
	void Update();
	void Draw();

	void BoxSow(RECT m_rc, long x, long y, D3DCOLOR color = D3DCOLOR_ARGB(255, 0, 255, 0));

};

extern Collider coll;