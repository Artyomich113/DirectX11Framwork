#pragma once
#include <d3d11.h>
#include <xnamath.h>
//#include "SimpleMath.inl" 
//#include "SimpleMath.h"

/*���������� 
������� ��������
1)�����������, ��� ��� ����������� (������� ������ �������)
2)�������� ���� directx (������� ������ ������)
3)���� windows
	�������� ������� �������� x=[-1,1] y=[-1,1] z=[0,1]

	�����������
	vector3 finale = (vector3)start + (vector3)translate;
	������� ������ ������������ ��� �� ����
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


