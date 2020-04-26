#include "DirectiinalLight.h"

DirectionalLight::DirectionalLight(XMVECTOR Direction, XMFLOAT4 Color)
{
	lightDirection = Direction;
	lightColor = Color;
}


DirectionalLight::~DirectionalLight()
{
}
