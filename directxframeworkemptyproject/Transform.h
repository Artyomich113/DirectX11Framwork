#pragma once
#include <d3d11.h>
#include <xnamath.h>
//#include "SimpleMath.inl" 
//#include "SimpleMath.h"

/*математика 
системы кординам
1)виртуальная, где все описывается (переход дальше сложный)
2)условное окно directx (переход дальше легкий)
3)окно windows
	экранная система кординат x=[-1,1] y=[-1,1] z=[0,1]

	перемещение
	vector3 finale = (vector3)start + (vector3)translate;
	поворот вокруг произвольной оси на угол
*/

class Transform
{
public:
	XMVECTOR Position; //vector3
	XMVECTOR Rotation; //quaternion
	XMVECTOR LocalScale;//vector3
	Transform();
	~Transform();
	void Translate(XMVECTOR direction);
	void RotateLC(XMVECTOR axis,float angle);
	void RotateWR(XMVECTOR axis, float angle);
	void Rotate(float x,float y,float z);
	void RotateAround(XMVECTOR point, XMVECTOR axis,float angle);
	/*(1,0,0)*/
	XMVECTOR Right();
	/*(-1,0,0)*/
	XMVECTOR Left();
	/*(0,0,1)*/
	XMVECTOR Forward();
	/*(0,0,-1)*/
	XMVECTOR Back();
	/*(0,1,0)*/
	XMVECTOR Up();
	/*(0,-1,0)*/
	XMVECTOR Down();
}; 


