#pragma once
#include "../Component.h"
#include "../Framework.h"
#include "Mesh.h"

class Camera;

class MeshRendererUI : virtual public Mesh
{
	DXManager* dxmanager;

public:
	Layout ComponentType() { return Layout::Render; };
	MeshRendererUI(const MeshRendererUI&);
	//HRESULT InitShader(std::string);

	HRESULT InitMesh();
	std::string ShaderName;

	SimpleVertex *Vertices;
	
	MeshRendererUI(SimpleVertex*, WORD*, /*std::string,*/int indexes, int verteces);

	D3D11_INPUT_ELEMENT_DESC * layout();
	UINT NumberOfElements();
	~MeshRendererUI();
	void process();
};

