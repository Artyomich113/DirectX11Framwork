#pragma once
#include <list>
//#include "Transform.h"
#include "Component.h"
class Transform;

class Gameobject
{
public:
	std::string name;
	std::list<Component*> components;
	Transform *transform;
	void procedure();
	void AddComponent(Component*);
	Gameobject();
	Gameobject(std::string);
	~Gameobject();
};