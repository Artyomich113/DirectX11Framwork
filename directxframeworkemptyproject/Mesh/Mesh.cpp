#include "Mesh.h"
#include <iostream>

HRESULT CompileShaderFromFile(LPCSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

HRESULT Mesh::InitShader(std::string ShaderName, std::string vertexShaderFunctionName , std::string pixelShaderFunctionName)
{
	HRESULT hr;

	DXManager* dxmanager = Framework::instanse().dxmanager;

	auto it = dxmanager->Shaders.find(vertexShaderFunctionName + pixelShaderFunctionName);
	
	if (it != dxmanager->Shaders.end())
	{
		std::cout << "\nFound Shader Pointer " << vertexShaderFunctionName + pixelShaderFunctionName << std::endl;
		shaderPointers = dxmanager->Shaders[vertexShaderFunctionName + pixelShaderFunctionName];
		return S_OK;
	}

	//std::cout << "\nCreating Shader Pointer " << ShaderName;

	// Компиляция вершинного шейдера из файла
	ID3DBlob* pVSBlob = NULL; // Вспомогательный объект - просто место в оперативной памяти

	//std::cout << "\ncompile vertex shader";
	hr = CompileShaderFromFile(ShaderName.c_str(), vertexShaderFunctionName.c_str(), "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Невозможно скомпилировать файл FX. Пожалуйста, запустите данную программу из папки, содержащей файл FX.", "Ошибка", MB_OK);
		return hr;
	}

	// Создание вершинного шейдера
	//std::cout << "\ncreate vertex shader";
	hr = dxmanager->m_device->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &shaderPointers.m_VertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	};

	
	D3D11_INPUT_ELEMENT_DESC * lay = layout();
	hr = dxmanager->m_device->CreateInputLayout(lay, NumberOfElements(), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &shaderPointers.g_pVertexLayout);
	delete lay;

	pVSBlob->Release();
	if (FAILED(hr)) return hr;

	// Подключение шаблона вершин
	//dxmanager->m_deviceContext->IASetInputLayout(shaderPointers.g_pVertexLayout);

	// Компиляция пиксельного шейдера из файла
	//std::cout << "\ncompile pixel shader";
	ID3DBlob* pPSBlob = NULL;
	hr = CompileShaderFromFile(ShaderName.c_str(), pixelShaderFunctionName.c_str(), "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Невозможно скомпилировать файл FX. Пожалуйста, запустите данную программу из папки, содержащей файл FX.", "Ошибка", MB_OK);
		return hr;
	}

	//std::cout << "\n create pixel shader";
	hr = dxmanager->m_device->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &shaderPointers.m_PixelShader);
	pPSBlob->Release();
	if (FAILED(hr)) return hr;

	dxmanager->Shaders.insert(std::pair<std::string, ShaderPointers>(vertexShaderFunctionName + pixelShaderFunctionName, shaderPointers));

	//std::cout << "\nshader initialized";
	return hr;

}

Mesh::~Mesh()
{
	//std::cout << "\n~mesh";
	//std::cout << "c" << g_pConstantBuffer;
	if (copy == false)
	{
		if (g_pConstantBuffer)
			g_pConstantBuffer->Release();
		if (g_pIndexBuffer)
			g_pIndexBuffer->Release();
		if (g_pVertexBuffer)
			g_pVertexBuffer->Release();
	}
}
