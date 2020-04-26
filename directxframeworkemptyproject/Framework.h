#pragma once

#include"DXManager.h"
#include "Camera.h"
#include "Transform.h"
#include "Gameobject.h"
#include "resource.h"
#include "SystemDefs.h"
#include "DirectiinalLight.h"
#include "PointLight.h"
#include "Mouse/MouseClass.h"
#include <string>
#include <cstring>
#include <list>
#include <map>

//class DXManager;
class Gameobject;
class Camera;

template<typename T>
using shared = std::shared_ptr<T>;

class Framework
{
	Framework();
public:
	~Framework();
	
	static Framework& instanse()
	{
		static Framework framework;
		return framework;
	}
	bool IsRunning;
	std::map<Component::Layout,shared<std::list<Component*>>> ComponentsByLayout;

	MouseClass mouse;
	bool Initialize(std::string name);
	Camera* camera;
	DXManager* GetDXManager(){return dxmanager;}
	std::list<Gameobject*> gameobjects;
	void AddGameobject(Gameobject*);
	void AddComponent(Component*);
	MSG Run();
	void proceedAllGameobjects();
	//private:
	bool CreateDXWindow(std::string name, int x, int y, int height, int wight);
	std::string name;
	DXManager *dxmanager;
	HINSTANCE g_hIns;
	HWND g_hWnd;

	PointLight * pointLights[2];
	DirectionalLight * DirLight;
	float RunStartTime;
	float curtime;
	float cosTime;

};


