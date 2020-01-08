#pragma once
#include "Mouse/MouseClass.h"
#include <iostream>

class Gameobject;

class Component
{

public:
	enum Layout
	{
		Render,
		GameLogic,
	};
	Gameobject *gameobject;
	virtual void process() = 0;
	virtual Layout ComponentType() = 0;

	Component()
	{
		//std::cout << "\nPtr " << this;
	}
	virtual ~Component()
	{
		//std::cout << "\n~comp" << this;
	};
};



