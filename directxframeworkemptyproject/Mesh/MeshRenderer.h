#pragma once
#include "../Component.h"
#include "../Framework.h"
#include "Mesh.h"

class Camera;

class MeshRenderer : virtual public Mesh
{
	DXManager* dxmanager;

public:
	Layout ComponentType(){return Layout::Render;};
	MeshRenderer(const MeshRenderer&);
	//HRESULT InitShader(std::string);

	HRESULT InitMesh();
	std::string ShaderName;

	SimpleVertex *Vertices;
	

	MeshRenderer(SimpleVertex*,WORD*, /*std::string,*/int indexes,int verteces);
	
	D3D11_INPUT_ELEMENT_DESC * layout();
	UINT NumberOfElements();
	~MeshRenderer();
	void process();
};


