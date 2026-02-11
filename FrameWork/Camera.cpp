#include "Include.h"


Camera cam;

//	유일 객체 (데이타 영역에 올라간 변수에 초기값 을 nullptr 로 셋팅)
//  이 로직은 프로그램 시작전에(main 호출) 실행된다.
Camera * Camera::m_pInstance = nullptr;

Camera* Camera::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		// 힙 메모리에 객체 할당 and 데이타 영역의 참조 변수에 주소 할당
		m_pInstance = new Camera();
	}

	return m_pInstance;
}


Camera::Camera()
{
	camPos.posX = 0;
	camPos.posY = 0;
}

Camera::~Camera()
{
}

void Camera::Update() // 원래는 모든 클래스를 다형성으로 배열로 참조하여 값을 전부 변경(리스트?)
{
	// 일단 맵 처리...
	mapMng.posX = camPos.posX;
	mapMng.posY = camPos.posY;
}

void Camera::SetXCam(double x)
{
	camPos.posX += x;
}

void Camera::SetYCam(double y)
{
	camPos.posY += y;
}

// Singleton 디자인 패턴으로 extern 선언 안해도 됨 (공부용)
