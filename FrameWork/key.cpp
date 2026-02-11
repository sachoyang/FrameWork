#include "Include.h"

Key key;

Key::Key(void)
{

	
}

Key::~Key(void)
{
}

// 동시 키를 위하여 KeyTime, KeyTime1,KeyTime2,KeyTime3 따로 처리

void Key::Update()
{
	knight.isMove = false;
	knight.isLookup = false;
	knight.isLookdown = false;
	// 시스템에 따라...GetTickCount/GetTickCount64
	if(KeyDown('1'))
	{
		if(GetTickCount64() - KeyTime > 200)
		{
			KeyTime = GetTickCount64();
		}
	}

	if(KeyDown('2'))
	{
		if(GetTickCount64() - KeyTime > 200)
		{
			KeyTime = GetTickCount64();
		}
		
	}

	if(KeyDown('3'))
	{
		if(GetTickCount64() - KeyTime > 200)
		{
			Gmanager.m_Collision = true;
			KeyTime = GetTickCount64();
		}
		
	}
	
	if(KeyDown('4'))
	{
		if(GetTickCount64() - KeyTime > 200)
		{
			KeyTime = GetTickCount64();
		}
		
	}

	if(KeyDown('5'))
	{
		if(GetTickCount64() - KeyTime > 200)
		{
			Gmanager.Respawn = true;
			KeyTime = GetTickCount64();
		}
	
	}
	
	if(KeyDown('6'))
	{
		if(GetTickCount64() - KeyTime > 200)
		{
			Gmanager.myList.clear();
			KeyTime = GetTickCount64();
		}
	}



	if(KeyDown('7'))
	{
		if(GetTickCount64() - KeyTime > 200)
		{
			KeyTime = GetTickCount64();
		}
		
	}



	
	

	if(KeyDown('8'))
	{
		if(GetTickCount64() - KeyTime > 200)
		{

		}
		
	}

	if(KeyDown('9'))
	{
		if(GetTickCount64() - KeyTime > 200)
		{

			KeyTime = GetTickCount64();
		}
		
	}


	if(KeyDown('0'))
	{
		if(GetTickCount64() - KeyTime > 200)
		{

			KeyTime = GetTickCount64();
		}
		
	}






	if(KeyDown(VK_F1))
	{
		if(GetTickCount64() - KeyTime1 > 10)
		{	
			Camera::GetInstance()->SetXCam(1);
			KeyTime1 = GetTickCount64();
		}
	
	}

	
	if(KeyDown(VK_F2))
	{
		if(GetTickCount64() - KeyTime1 > 10)
		{	
			Camera::GetInstance()->SetXCam(-1);
			KeyTime1 = GetTickCount64();
		}
	
	}

	
	if(KeyDown(VK_F3))
	{
		if(GetTickCount64() - KeyTime > 200)
		{	
			KeyTime = GetTickCount64();
		}
	
	}

	
	if(KeyDown(VK_F4))
	{
		if(GetTickCount64() - KeyTime > 200)
		{	
			KeyTime = GetTickCount64();
		}
	
	}

	
	if(KeyDown(VK_F5))
	{
		if(GetTickCount64() - KeyTime > 200)
		{
			KeyTime = GetTickCount64();
		}
	
	}

	
	if(KeyDown(VK_F6))
	{
		if(GetTickCount64() - KeyTime > 200)
		{	
			KeyTime = GetTickCount64();
		}
	
	}

	
	if(KeyDown(VK_F7))
	{
		if(GetTickCount64() - KeyTime > 200)
		{	
		
			KeyTime = GetTickCount64();
		}
	
	}

	
	if(KeyDown(VK_F8))
	{
		if(GetTickCount64() - KeyTime > 200)
		{		
			KeyTime = GetTickCount64();
		}
	
	}

	
	if(KeyDown(VK_F9))
	{
		if(GetTickCount64() - KeyTime > 200)
		{	
			KeyTime = GetTickCount64();
		}
	
	}

	
	if(KeyDown(VK_F10))
	{
		if(GetTickCount64() - KeyTime > 200)
		{
			KeyTime = GetTickCount64();
		}
	
	}

	
	if(KeyDown(VK_F11))
	{
		if(GetTickCount64() - KeyTime > 200)
		{
			KeyTime = GetTickCount64();
		}
	
	}

	
	if(KeyDown(VK_F12))
	{
		if(GetTickCount64() - KeyTime > 200)
		{	
			KeyTime = GetTickCount64();
		}
	
	}
	


	if(KeyDown(VK_SPACE) ) 
	{
		
		if(GetTickCount64() - KeyTime2 > 200)
		{
			KeyTime2 = GetTickCount64();
		}
		
	}



	if(KeyDown(VK_LEFT))
	{
		if(GetTickCount64() - KeyTime1 > 10)
		{
			knight.isMove = TRUE;
			knight.pos.x -= 2.0f;
			knight.dir = 1;

			KeyTime1 = GetTickCount64();
		}
	}

	if(KeyDown(VK_RIGHT))
	{

		if(GetTickCount64() - KeyTime1 > 10)
		{
			knight.isMove = TRUE;
			knight.pos.x += 2.0f;
			knight.dir = -1;

			KeyTime1 = GetTickCount64();
		}
		
	}
	
	if(KeyDown(VK_UP))
	{
		if(GetTickCount64() - KeyTime3 > 10)
		{	
			knight.isLookup = TRUE;
			KeyTime3 = GetTickCount64();
		}
		
	}

	if(KeyDown(VK_DOWN))
	{

		if(GetTickCount64() - KeyTime3 > 10)
		{
			knight.isLookdown = TRUE;
			KeyTime3 = GetTickCount64();
		}
		
	}


	if(KeyDown('C') )
	{
		if(GetTickCount64() - KeyTime1 > 200)
		{	
			KeyTime1 = GetTickCount64();
		}
	}




	
	if(KeyDown('N'))
	{
		
		if(GetTickCount64() - KeyTime > 200)
		{			
			KeyTime = GetTickCount64();
		}
	}

	
	if(KeyDown('A'))
	{
		if(GetTickCount64() - KeyTime > 200)
		{
			KeyTime = GetTickCount64();
		}
	}




	if(KeyDown('W') )
	{
		if(GetTickCount64() - KeyTime > 200)
		{
			
			KeyTime = GetTickCount64();
		}

	}


	if(KeyDown('E')  )
	{
		if(GetTickCount64() - KeyTime > 200)
		{	
			KeyTime = GetTickCount64();
		}
		
	}


	if(KeyDown('R') )
	{
		if(GetTickCount64() - KeyTime > 200)
		{

			KeyTime = GetTickCount64();
		}
	}


	if(KeyDown('S') )
	{
		if(GetTickCount64() - KeyTime > 200)
		{
	
			KeyTime = GetTickCount64();
		}
	}


	if(KeyDown('D') )
	{
		if(GetTickCount64() - KeyTime > 200)
		{
			
			KeyTime = GetTickCount64();
		}
	}


	if(KeyDown(VK_RETURN) ) 
	{
		
		if(GetTickCount64() - KeyTime1 > 200)
		{

			KeyTime1 = GetTickCount64();
		}
		
	}
}