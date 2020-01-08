#include "DXManager.h"
#include "SystemDefs.h"
#include <iostream>

HRESULT CompileShaderFromFile(LPCSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
	dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pErrorBlob) pErrorBlob->Release();
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}

DXManager::DXManager()
{
	m_swapChain = 0;
	m_device = 0;
	m_deviceContext = 0;
	m_renderTargetView = 0;
	m_depthStencilBuffer = 0;
	m_depthStencilState = 0;
	m_depthStencilview = 0;
	m_rasterState = 0;
	m_alphaEnableBledingState = 0;
	m_alphaDisabledBlendingState = 0;
	m_depthDisabledStencilState = 0;
}

HRESULT DXManager::InitMatrixes(HWND hwnd)
{

	std::cout << "\nInitMatrixes";
	RECT rc;
	GetClientRect(hwnd, &rc);
	UINT width = rc.right - rc.left;	// �������� ������
	UINT height = rc.bottom - rc.top;	// � ������ ����

	// ������������� ������� ����
	SetWorld(XMMatrixIdentity());

	// ������������� ������� ����
	//XMVECTOR Eye = XMVectorSet(0.0f, 2.0f, -8.0f, 0.0f);	// ������ �������
	//XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);	// ���� �������
	//XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);	// ����������� �����
	//g_View = XMMatrixLookAtLH(Eye, At, Up);

	// ������������� ������� ��������
	// ���������: 1) ������ ���� ��������� 2) "������������" �������
	// 3) ����� ������� ������� ���������� 4) ����� ������� ������� ����������
	//g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, width / (FLOAT)height, 0.01f, 100.0f);
	std::cout << "\nfinished";
	return S_OK;
}


DXManager::~DXManager()
{
	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}

	if (m_device)
	{
		m_device->Release();
		m_device = 0;
	}
	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = 0;
	}
	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}
	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}
	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = 0;
	}
	if (m_depthStencilview)
	{
		m_depthStencilview->Release();
		m_depthStencilview = 0;
	}
	if (m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = 0;
	}
	if (m_alphaEnableBledingState)
	{
		m_alphaEnableBledingState->Release();
		m_alphaEnableBledingState = 0;
	}
	if (m_alphaDisabledBlendingState)
	{
		m_alphaDisabledBlendingState->Release();
		m_alphaDisabledBlendingState = 0;
	}
	if (m_depthDisabledStencilState)
	{
		m_depthDisabledStencilState->Release();
		m_depthDisabledStencilState = 0;
	}

}

/*bool DXManager::Initialize(int screenWight, int screenHeight, bool vsync, HWND hwnd, bool fullscreen)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes,i,numerator,denominator,stringLength;
	DXGI_MODE_DESC * displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	ID3D11Texture2D* backBufferPtr;
	m_vsync_enabled = vsync;

	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)&factory);

	if (FAILED(result))
	{
		return false;
	}

	//use factory to create an adapter for the primary graphics interface
	result = factory->EnumAdapters(0,&adapter);
	if (FAILED(result))
	{
		return false;
	}
	result = adapter->EnumOutputs(0,&adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	//Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM

	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B5G5R5A1_UNORM,DXGI_ENUM_MODES_INTERLACED,&numModes,NULL);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}*/

HRESULT DXManager::InitDevice(HWND g_hWnd)
{
	std::cout << "\ninitDevice";
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(g_hWnd, &rc);
	UINT width = rc.right - rc.left;           // �������� ������
	UINT height = rc.bottom - rc.top;   // � ������ ����

	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	// ��� �� ������� ������ �������������� ������ DirectX

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	// ������ �� �������� ���������� DirectX. ��� ������ �������� ���������,
	// ������� ��������� �������� ��������� ������ � ����������� ��� � ������ ����.
	DXGI_SWAP_CHAIN_DESC sd;            // ���������, ����������� ���� ����� (Swap Chain)
	ZeroMemory(&sd, sizeof(sd));    // ������� ��
	sd.BufferCount = 1;                               // � ��� ���� ������ �����
	sd.BufferDesc.Width = width;                     // ������ ������
	sd.BufferDesc.Height = height;                          // ������ ������
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;      // ������ ������� � ������
	sd.BufferDesc.RefreshRate.Numerator = FRAME_RATE;         // ������� ���������� ������
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // ���������� ������ - ������ �����
	sd.OutputWindow = g_hWnd;                               // ����������� � ������ ����
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;                               // �� ������������� �����

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags,
			featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &sd, &m_swapChain,
			&m_device, &g_featureLevel, &m_deviceContext);
		if (SUCCEEDED(hr)) // ���� ���������� ������� �������, �� ������� �� �����
			break;
	}
	if (FAILED(hr)) return hr;

	// ������ ������� ������ �����. �������� ��������, � SDK
	// RenderTargetOutput - ��� �������� �����, � RenderTargetView - ������.
	// ��������� �������� ������� ������
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr)) return hr;

	// �� ����������� �������� ������� ����������� ���������
	hr = m_device->CreateRenderTargetView(pBackBuffer, NULL, &m_renderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr)) return hr;

	// ��������� � �������� ������ ������
	// ������� ��������-�������� ������ ������
	D3D11_TEXTURE2D_DESC descDepth;	// ��������� � �����������
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;		// ������ �
	descDepth.Height = height;		// ������ ��������
	descDepth.MipLevels = 1;		// ������� ������������
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	// ������ (������ �������)
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;		// ��� - ����� ������
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	// ��� ������ ����������� ���������-�������� ������� ������ ��������
	hr = m_device->CreateTexture2D(&descDepth, NULL, &m_depthStencilBuffer);
	if (FAILED(hr)) return hr;

	// ������ ���� ������� ��� ������ ������ ������
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;	// ��������� � �����������
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;		// ������ ��� � ��������
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	// ��� ������ ����������� ���������-�������� � �������� ������� ������ ������ ������
	hr = m_device->CreateDepthStencilView(m_depthStencilBuffer, &descDSV, &m_depthStencilview);
	if (FAILED(hr)) return hr;

	// ���������� ������ ������� ������ � ��������� ����������
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilview);

	// ��������� ��������

	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	// ���������� ������� � ��������� ����������
	m_deviceContext->RSSetViewports(1, &vp);

	return S_OK;
}

void DXManager::BeginScene(float r, float g, float b, float a)
{
}

void DXManager::EndScene()
{
}

void DXManager::EnableAlphaBlending(bool)
{
}

void DXManager::EnableZbuffer(bool)
{
}

HRESULT DXManager::InitShader(std::string filename)
{
	std::cout << "\ninit shader " << filename;
	HRESULT hr;


	// ���������� ���������� ������� �� �����
	ID3DBlob* pVSBlob = NULL; // ��������������� ������ - ������ ����� � ����������� ������



	std::cout << "\ncompile vertex shader";
	hr = CompileShaderFromFile(filename.c_str(), "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL, "���������� �������������� ���� FX. ����������, ��������� ������ ��������� �� �����, ���������� ���� FX.", "������", MB_OK);
		return hr;
	}

	// �������� ���������� �������
	std::cout << "\ncreate vertex shader";
	hr = m_device->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &m_VertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	};

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// �������� ������� ������
	std::cout << "\ncreate input layout";
	hr = m_device->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &g_pVertexLayout);

	pVSBlob->Release();
	if (FAILED(hr)) return hr;

	// ����������� ������� ������
	m_deviceContext->IASetInputLayout(g_pVertexLayout);

	// ���������� ����������� ������� �� �����
	std::cout << "\ncompile pixel shader";
	ID3DBlob* pPSBlob = NULL;
	hr = CompileShaderFromFile(filename.c_str(), "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL, "���������� �������������� ���� FX. ����������, ��������� ������ ��������� �� �����, ���������� ���� FX.", "������", MB_OK);
		return hr;
	}

	std::cout << "\n create pixel shader";
	hr = m_device->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_PixelShader);
	pPSBlob->Release();
	if (FAILED(hr)) return hr;
	std::cout << "\nshader initialized";
	return hr;
}

ID3D11Device * DXManager::GetDevice()
{
	return m_device;
}

ID3D11DeviceContext * DXManager::GetDeviceContext()
{
	return m_deviceContext;
}

IDXGISwapChain * DXManager::GetSwapChain()
{
	return m_swapChain;
}

ID3D11RenderTargetView * DXManager::GetRenderTargetView()
{
	return m_renderTargetView;
}

ID3D11VertexShader * DXManager::GetVertexShader()
{
	return m_VertexShader;
}

bool DXManager::InitializeZbuffer()
{
	return false;
}



bool DXManager::InitializeSwapChain(HWND hwnd, bool fullscreen, int screenWight, int screenHeight, unsigned int numerator, unsigned int denominator)
{
	return false;
}

bool DXManager::InitializeDepthBuffer(int screenWight, int screenHeight)
{
	return false;
}

bool DXManager::InitializeDepthStencilBuffer()
{
	return false;
}

bool DXManager::InitializeDepthStencilView()
{
	return false;
}

bool DXManager::InitializeRasterilView()
{
	return false;
}

void DXManager::InitializeViewport(int screenWight, int screenHeight)
{
}

bool DXManager::InitializeAlphaBlending()
{
	return false;
}
