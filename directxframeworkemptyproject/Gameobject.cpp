#include "Gameobject.h"
#include "Framework.h"
#include "Transform.h"
#include <iostream>

void Gameobject::procedure()
{
//std::cout << "\ngameobject procedure";
	for (auto iter = components.begin(); iter != components.end(); iter++)
	{
		(*iter)->process();
	}
}

void Gameobject::AddComponent(Component * component)
{

	std::cout << "\npushback " << name;
	components.push_back(component);

	component->gameobject = this;

	std::cout << "\naddcomponent " << name;
	Framework::instanse().AddComponent(component);
}

Gameobject::Gameobject()
{
	name = "GameObject";
	transform = new Transform();
}

Gameobject::Gameobject(std::string name)
{
	this->name = name;
	transform = new Transform();
}

Gameobject::~Gameobject()
{
	std::cout << "\ndelete gm";
	delete transform;
}