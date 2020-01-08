#pragma once
#include "Component.h"
#include "SystemDefs.h"
#include "Framework.h"
#include <iostream>

class Framework;
class Gameobject;
class DXManager;

class Camera: virtual public Component
{
	public:	
	Layout ComponentType(){return Layout::Render;};
	XMMATRIX g_view;
	XMMATRIX g_Projection;
	Camera();
	~Camera();
	void *operator new(size_t size)
	{
		//std::cout << "\ncamera is aligned by " << 16;
		void* p = _aligned_malloc(size, 16);
		if(!p)
		throw std::bad_alloc();

		return p;
	}

	void operator delete(void * p)
	{
		std::cout << "\ndelete camera overload ";
		_aligned_free(p);
	}

	void process();
	
};


