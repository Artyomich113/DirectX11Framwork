#include "MeshRenderer.h"
#include <iostream>

MeshRenderer::MeshRenderer(const MeshRenderer &ob)
{
	NumOfIndexes = ob.NumOfIndexes;
	NumOfVerteces = ob.NumOfVerteces;

	g_pVertexBuffer = ob.g_pVertexBuffer;
	g_pIndexBuffer = ob.g_pIndexBuffer;
	g_pConstantBuffer = ob.g_pConstantBuffer;


	shaderPointers = ob.shaderPointers;
	dxmanager = Framework::instanse().GetDXManager();
	copy = true;
}
/*
HRESULT MeshRenderer::InitShader(std::string name)
{
	return dxmanager->InitShader(name);
}*/

HRESULT MeshRenderer::InitMesh()
{
	std::cout << "\ninit mesh";
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;	// Структура, описывающая создаваемый буфер
	//std::cout << "\nzero memory";
	ZeroMemory(&bd, sizeof(bd));				// очищаем ее
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * NumOfVerteces;	// размер буфера
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// тип буфера - буфер вершин
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;	// Структура, содержащая данные буфера
	//std::cout << "\nzero memory";
	ZeroMemory(&InitData, sizeof(InitData));	// очищаем ее
	InitData.pSysMem = Vertices;				// указатель на наши 8 вершин
	// Вызов метода g_pd3dDevice создаст объект буфера вершин
	std::cout << "\n   createbuffer";
	hr = dxmanager->m_device->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
	std::cout << "\n   buffer created";
	if (FAILED(hr))
	{
	std::cout << "\n  failed create vertices buffer " << NumOfVerteces << " vecteces";
	return hr;
	}

	bd.Usage = D3D11_USAGE_DEFAULT;		// Структура, описывающая создаваемый буфер
	bd.ByteWidth = sizeof(WORD) * NumOfIndexes;	// для 6 треугольников необходимо 18 вершин
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER; // тип - буфер индексов
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = Indices;				// указатель на наш массив индексов
	// Вызов метода g_pd3dDevice создаст объект буфера индексов
	std::cout << "\n    creating indeces buffer";
	hr = dxmanager->m_device->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
	if (FAILED(hr))
	{
	std::cout << "\n   failed create indices buffer";
	return hr;
	}

	
	dxmanager->m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Создание константного буфера
	bd.Usage = D3D11_USAGE_DEFAULT;
	//std::cout << "bytewidth" << sizeof(ConstantBuffer);
	bd.ByteWidth = sizeof(ConstantBuffer);		// размер буфера = размеру структуры
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// тип - константный буфер
	bd.CPUAccessFlags = 0;
	std::cout << "\n creating constant buffer";
	hr = dxmanager->m_device->CreateBuffer(&bd, NULL, &g_pConstantBuffer);
	if (FAILED(hr)) 
	{
		std::cout << "\nfailed create costant buffer";
		return hr;
	}
	return hr;
}

MeshRenderer::MeshRenderer(SimpleVertex *vertices, WORD* indices, int NumberOfIndexes, int NumberOfVerteces)
{
	//std::cout << " new meh Renderer";
	Vertices = vertices;
	Indices = indices;
	this->NumOfIndexes = NumberOfIndexes;
	this->NumOfVerteces = NumberOfVerteces;
	dxmanager = Framework::instanse().GetDXManager();
	/*if (FAILED(InitShader(ShaderName)))
	{
		std::cout << "\nfailed init shader";
	}*/
}



MeshRenderer::~MeshRenderer()
{
	std::cout << "\n~MeshRenderer";
}

D3D11_INPUT_ELEMENT_DESC * MeshRenderer::layout()
{
	
	D3D11_INPUT_ELEMENT_DESC *layout = new D3D11_INPUT_ELEMENT_DESC[2];
	layout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	layout[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	
	std::cout << "\nReturning Layout " << layout;
	return layout;
}

UINT MeshRenderer::NumberOfElements()
{
std::cout << "\nreturning number of elements " << 2;
	return (UINT)2;
}

inline void MeshRenderer::process()
{
	//std::cout <<"\nMR";
	XMMATRIX mRotation = XMMatrixRotationQuaternion(gameobject->transform->Rotation);
	//XMMATRIX mRotation = XMMatrixRotationRollPitchYawFromVector(gameobject->transform->Rotation);

	XMMATRIX mTranslate = XMMatrixTranslationFromVector(gameobject->transform->Position);

	XMMATRIX mscale = XMMatrixScalingFromVector(gameobject->transform->LocalScale);

	
	XMMATRIX g_World = mscale * mRotation * mTranslate;
	
	ConstantBuffer cb;
	//std::cout << "\ng_World";
	cb.mWorld = XMMatrixTranspose(g_World);
	//std::cout << "\ng_View";
	//cb.mView = XMMatrixTranspose(Framework::instanse().camera->GetView());
	cb.mView = XMMatrixTranspose(Framework::instanse().camera->g_view);
	//std::cout << "\ng_Projection";
	//cb.mProjection = XMMatrixTranspose(Framework::instanse().camera->GetProjection());
	cb.mProjection = XMMatrixTranspose(Framework::instanse().camera->g_Projection);
	cb.CameraPos = Framework::instanse().camera->gameobject->transform->Position.m128_f32;
	cb.LightColor = Framework::instanse().DirLight->lightColor;
	cb.LightDir = Framework::instanse().DirLight->lightDirection.m128_f32;
	cb.costime = Framework::instanse().cosTime;
	cb.curtime = Framework::instanse().curtime;

	//std::cout << "\nsubresource";
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;

	ID3D11DeviceContext *id3d11devicecontext = dxmanager->m_deviceContext;

	id3d11devicecontext->IASetInputLayout(shaderPointers.g_pVertexLayout);

	
	id3d11devicecontext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);

	id3d11devicecontext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	id3d11devicecontext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);


	//std::cout << "\nContext";
	
	id3d11devicecontext->VSSetShader(shaderPointers.m_VertexShader, NULL, 0);
	id3d11devicecontext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	id3d11devicecontext->PSSetShader(shaderPointers.m_PixelShader, NULL, 0);
	
	id3d11devicecontext->DrawIndexed(NumOfIndexes, 0, 0);
}
