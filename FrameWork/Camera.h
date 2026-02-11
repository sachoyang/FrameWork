#pragma once

struct CameraPos
{
	double posX;
	double posY;

};

class Camera
{
private:
	//	유일한 객체
	// 먼저 데이타 영역에 올라감
	static Camera* m_pInstance;
	CameraPos camPos;

public:
	Camera(void);
	~Camera(void);

	//	객체 얻기
	static Camera*	GetInstance();

	void SetXCam(double);
	void SetYCam(double);

	void Update();
};
