#pragma once

#include "Mesh.h"
#include "../Component.h"
#include "../Framework.h"

class MeshRendererTextured : virtual public Mesh
{
	DXManager* dxmanager;

public:
	Layout ComponentType() { return Layout::Render; };
	MeshRendererTextured(const MeshRendererTextured&);
	//HRESULT InitShader(std::string);

	HRESULT InitMesh();
	std::string TextureName;

	TexturedVertex *Vertices;

	std::string modelpath;

	void LoadFromObj();

	ID3D11ShaderResourceView* g_pTextureRV = NULL;        // Объект текстуры
	ID3D11SamplerState*       g_pSamplerLinear = NULL;    // Параметры наложения текстуры

	MeshRendererTextured(std::string TextureName, TexturedVertex*vertices, WORD*indices, int indexes, int verteces);

	MeshRendererTextured(std::string ModelPath, std::string texturename);

	D3D11_INPUT_ELEMENT_DESC * layout();
	UINT NumberOfElements();
	~MeshRendererTextured();
	void process();
};



