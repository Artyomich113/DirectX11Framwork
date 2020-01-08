#include "Transform.h"
#include <iostream>

Transform::Transform()
{
	LocalScale = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);//vector3
	Position = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);  //vector3
	
	Rotation = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);  //quaternion
	Rotation = XMQuaternionNormalize(Rotation);

}

Transform::~Transform()
{

}

void Transform::Translate(XMVECTOR direction)
{
	//std::cout << "\ntranslate " << Position.m128_f32[0] << Position.m128_f32[1] << Position.m128_f32[2] << Position.m128_f32[3];
	Position += direction;
	//std::cout << "\nafter translate " << Position.m128_f32[0] << Position.m128_f32[1] << Position.m128_f32[2] << Position.m128_f32[3];
}
/*
rotQuad * Rotation
*/
void Transform::RotateLC(XMVECTOR axis, float angle)
{
	//std::cout << "W" << angle;
	XMVECTOR rotQuat = XMQuaternionRotationNormal(axis, angle);
	//Rotation = XMQuaternionMultiply( rotQuat, Rotation );
	Rotation = XMQuaternionMultiply(rotQuat, Rotation);	
	Rotation = XMQuaternionNormalize(Rotation);
}
/*
Rotation * rotQuad
*/
void Transform::RotateWR(XMVECTOR axis, float angle)
{
//std::cout << "L" << angle;
	XMVECTOR rotQuat = XMQuaternionRotationNormal(axis, angle);
	Rotation = XMQuaternionMultiply(Rotation, rotQuat);
	Rotation = XMQuaternionNormalize(Rotation);
}

void Transform::Rotate(float x, float y, float z)
{
	
	Rotation = XMQuaternionMultiply(Rotation,XMQuaternionRotationRollPitchYawFromVector(XMVectorSet(x,y,z,0.0f)));
	//Rotation += XMVectorSet(x,y,z,0);
	//std::cout << "\n" << Rotation.m128_f32[0] << " " << Rotation.m128_f32[1] << " " << Rotation.m128_f32[2] << " " << Rotation.m128_f32[3];
}

/*void Transform::Rotate(DirectX::SimpleMath::Vector3 vector3, float angle)
{
	Rotation += DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(vector3, angle);
	//DirectX::SimpleMath::Matrix matrix = DirectX::SimpleMath::Matrix::CreateFromAxisAngle(vector3,angle);
	//Rotation += DirectX::SimpleMath::Quaternion::CreateFromRotationMatrix(matrix);
}*/

void Transform::RotateAround(XMVECTOR point, XMVECTOR axis, float angle)
{
	XMVECTOR rotationvector = Position - point;
	Position = point + XMVector3Rotate(rotationvector, XMQuaternionRotationAxis(axis, angle));
}

XMVECTOR Transform::Up()
{
	//XMMATRIX rot = XMMatrixRotationQuaternion(Rotation);
	//return XMVectorSet(rot._21, rot._22, rot._23, 0.0f);
	return XMVector3Rotate(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), Rotation);
}

XMVECTOR Transform::Down()
{
	//XMMATRIX rot = XMMatrixRotationQuaternion(Rotation);
	//return XMVectorSet(-rot._21, -rot._22, -rot._23, 0.0f);
	return XMVector3Rotate(XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f), Rotation);
}
XMVECTOR Transform::Right()
{
	//XMMATRIX rot = XMMatrixRotationQuaternion(Rotation);
	//return XMVectorSet(rot._11, rot._12, rot._13,0.0f);
	return XMVector3Rotate(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), Rotation);
}

XMVECTOR Transform::Left()
{
	//XMMATRIX rot = XMMatrixRotationQuaternion(Rotation);
	//return XMVectorSet(-rot._11, -rot._12, -rot._13, 0.0f);
	return XMVector3Rotate(XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f), Rotation);
}

XMVECTOR Transform::Forward()
{
	//XMMATRIX rot = XMMatrixRotationQuaternion(Rotation);
	//XMVectorSet(rot._31, rot._32, rot._33, 0.0f);
	return XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), Rotation);
}

XMVECTOR Transform::Back()
{
	//XMMATRIX rot = XMMatrixRotationQuaternion(Rotation);
	//return XMVectorSet(-rot._31, -rot._32, -rot._33, 0.0f);
	return XMVector3Rotate(XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f), Rotation);
}

