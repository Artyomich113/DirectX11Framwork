#include "Camera.h"
#include <iostream>
#include <math.h>
/*
операции для матрицы проекции

x' = x/(z * tan(a/2))
y' = y/(z * tan(b/2))

z' = ((Z-Zmin /) Z)(Zmax - (Zmax - Zmin))

Zmin >> 10^(-7)*Z^2 / delZ // delZ - расстояние между объектами

{
ctan(a/2),0,0,0
0,ctag(b/2),0,0
0,0,......
0,0,......


perspective proj
(
w,0,0,0
0,k,0,0
0,0,q,1
o,0,-q*z,0
)
x' = x/w
y' = y/w
z' = z/w


}
*/

template <typename T> int sgn(T val)
{
	return (T(0) < val) - (val < T(0));
}
/*
XMMATRIX Camera::GetView()
{
	return XMLoadFloat4x4(&g_view);
}

void Camera::SetView(CXMMATRIX view)
{
	XMStoreFloat4x4(&g_view, view);
}

XMMATRIX Camera::GetProjection()
{
	return XMLoadFloat4x4(&g_Projection);
}

void Camera::SetProjection(CXMMATRIX proj)
{
	XMStoreFloat4x4(&g_Projection, proj);
}
*/
Camera::Camera()
{
	std::cout << "new camera";
	//SetProjection(XMMatrixPerspectiveFovLH(XM_PIDIV4, SCREEN_WIGHT / (FLOAT)SCREEN_HEIGHT, 0.01f, 100.0f));
	g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, SCREEN_WIGHT / (FLOAT)SCREEN_HEIGHT, 0.01f, 100.0f);

	
	//g_view = XMMatrixRotationQuaternion(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f));
	XMVECTOR ZEROPOS = XMVectorSet(0.0f,0.0f,-1.0f,0.0f);
	//std::cout << "\ng_view";
	//SetView(XMMatrixLookAtLH(ZEROPOS, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
	g_view = XMMatrixLookAtLH(ZEROPOS, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

	//std::cout << "\ninit Camera/";
}

Camera::~Camera()
{

}

void Camera::process()
{
	//std::cout << "\ncamera";
	float val = sgn(gameobject->transform->Up().m128_f32[1]);

	//SetView(XMMatrixLookAtLH(gameobject->transform->Position,gameobject->transform->Position + gameobject->transform->Forward(),XMVectorSet(0.0f, val/*sgn(gameobject->transform->Up().m128_f32[1])*/,0.0f,0.0f)));
	g_view = XMMatrixLookToLH(gameobject->transform->Position, gameobject->transform->Forward(), XMVectorSet(0.0f, val/*sgn(gameobject->transform->Up().m128_f32[1])*/, 0.0f, 0.0f));


	//SetView(XMMatrixLookToLH(gameobject->transform->Position,gameobject->transform->Rotation,XMVectorSet(0.0f, val/*sgn(gameobject->transform->Up().m128_f32[1])*/, 0.0f, 0.0f)));

	//std::cout.width(15);
	//std::cout << "\n" << gameobject->transform->Up().m128_f32[1] <<	" " << val;
	
	//XMMatrixRotationQuaternion(gameobject->transform->Rotation);
}