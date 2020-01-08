#pragma once
#include "Component.h"
#include "Framework.h"
#include <iostream>

class Gameobject;
class RotateQ : virtual public Component
{
	XMVECTOR axis;
	float rotationRad;
public:
	Layout ComponentType() { return Layout::GameLogic; };
	RotateQ(const RotateQ& ob)
	{
		axis = ob.axis;
		rotationRad = ob.rotationRad;
	}
	RotateQ(float val, XMVECTOR axis);
	~RotateQ(){};
	void process();
};

class RotateV : virtual public Component
{
	float x;
	float y;
	float z;
	public:
		Layout ComponentType() { return Layout::GameLogic; };
	RotateV(const RotateV& ob)
	{
		x = ob.x;
		y = ob.y;
		z = ob.z;
	}
	RotateV(float x,float y ,float z);
	RotateV(XMVECTOR);
	~RotateV(){}
	void process();
};

class Translator : virtual public Component
{
	XMVECTOR direction;
	public:
		Layout ComponentType() { return Layout::GameLogic; };
	Translator(const Translator& ob)
	{
		direction = ob.direction;
	}
	Translator(XMVECTOR dir)
	{
		direction = dir;
	}
	void process()
	{
		gameobject->transform->Translate(direction);
	}
};

class RotateAround : virtual public Component
{
	XMVECTOR point;
	XMVECTOR axis;
	float angle;
	public:
		Layout ComponentType() { return Layout::GameLogic; };
	RotateAround(const RotateAround& ob);
	RotateAround(XMVECTOR point,XMVECTOR axis,float angle)
	{
		this->point = point;
		this->axis = axis;
		this->angle = angle * XM_PI / 180;
	}
	void process();
};

class Controller : virtual public Component
{
	float speed;
	float angle;
public:
	Layout ComponentType() { return Layout::GameLogic; };
	Controller(float speed, float angle)
	{
		std::cout << " new controller";
		this->angle = angle * XM_PI / 180.0f;
		this->speed = speed;
	}
	~Controller();
	void process();
};
