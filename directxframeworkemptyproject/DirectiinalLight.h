#pragma once

#include "DXManager.h"

class DirectionalLight
{
public:
	XMFLOAT4 lightColor;
	XMVECTOR lightDirection;

	DirectionalLight(XMVECTOR Direction, XMFLOAT4 Color);
	~DirectionalLight();
};

