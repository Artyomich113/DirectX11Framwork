#pragma once

#include "Component.h"
#include "DXManager.h"


class PointLight : virtual public Component  
{
public:
	XMFLOAT4 lightColor;
	XMVECTOR lightPos;
	float lightPower;
	int index;
	PointLight(XMFLOAT4 lightColor, float lightPower, int index);
	PointLight();
	~PointLight();
	void process();
	Layout ComponentType(){return Layout::Render;}
};

