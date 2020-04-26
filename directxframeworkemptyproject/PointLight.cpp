#include "PointLight.h"
#include "Framework.h"

PointLight::PointLight(XMFLOAT4 lightColor, float lightPower, int index)
{
	this->lightColor = lightColor;
	this->lightPower = lightPower;
	lightPos = XMVectorSet(0.0f,0.0f,0.0f,0.0f);
	this->index = index;

	Framework::instanse().pointLights[index] = this;
}

PointLight::PointLight()
{

}


PointLight::~PointLight()
{

}

void PointLight::process()
{
	lightPos = gameobject->transform->Position;
}
