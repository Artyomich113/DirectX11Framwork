#pragma once
#include <string>
//#include <Windows.h>
#include <map>
#include <D3Dcompiler.h>
#include <D3D11.h>
#include <D3DX11.h>
#include <xnamath.h>
#include "resource.h"

class Framework;



struct ShaderPointers
{
	ID3D11VertexShader*     m_VertexShader = NULL;		// Вершинный шейдер
	ID3D11PixelShader*      m_PixelShader = NULL;		// Пиксельный шейдер
	ID3D11InputLayout*      g_pVertexLayout = NULL;		// Описание формата вершин
};


class DXManager
{
public:
	DXManager();
	~DXManager();

	std::map<std::string,ShaderPointers> Shaders;
	//bool Initialize(int screenWight,int screenHeight, bool vsync, HWND hwnd, bool fullscreen);
	HRESULT InitDevice(HWND g_hWnd);
	void BeginScene(float, float, float, float);
	void EndScene();

	void EnableAlphaBlending(bool enable);
	void EnableZbuffer(bool enable);
	HRESULT InitShader(std::string);


	ID3D11Device *GetDevice();
	ID3D11DeviceContext *GetDeviceContext();
	IDXGISwapChain* GetSwapChain();
	ID3D11RenderTargetView *GetRenderTargetView();
	ID3D11VertexShader *GetVertexShader();
	
	XMFLOAT4X4                g_World;					// Матрица мира
	void SetWorld(CXMMATRIX matrix)
	{
		XMStoreFloat4x4(&g_World, matrix);
	}
	XMMATRIX GetWorld()
	{
		return XMLoadFloat4x4(&g_World);
	}
	//XMMATRIX                g_View;						// Матрица вида
	//XMMATRIX                g_Projection;				// Матрица проекции

	ID3D11VertexShader*     m_VertexShader = NULL;		// Вершинный шейдер
	ID3D11PixelShader*      m_PixelShader = NULL;		// Пиксельный шейдер
	ID3D11InputLayout*      g_pVertexLayout = NULL;		// Описание формата вершин
	//ID3D11Buffer*           g_pVertexBuffer = NULL;		// Буфер вершин
	//ID3D11Buffer*           g_pIndexBuffer = NULL;		// Буфер индексов вершин
	//ID3D11Buffer*           g_pConstantBuffer = NULL;	// Константный буфер
	
	IDXGISwapChain *m_swapChain;
	ID3D11Device *m_device;
	ID3D11DeviceContext *m_deviceContext;
	ID3D11RenderTargetView *m_renderTargetView;
	ID3D11Texture2D *m_depthStencilBuffer;
	ID3D11DepthStencilState *m_depthStencilState;
	ID3D11DepthStencilView *m_depthStencilview;
	ID3D11RasterizerState *m_rasterState;
	ID3D11BlendState *m_alphaEnableBledingState;
	ID3D11BlendState *m_alphaDisabledBlendingState;
	ID3D11DepthStencilState *m_depthDisabledStencilState;
	HRESULT InitMatrixes(HWND);
private:
	bool InitializeSwapChain(HWND hwnd, bool fullscreen,int screenWight, int screenHeight, unsigned int numerator, unsigned int denominator);
	bool InitializeDepthBuffer(int screenWight, int screenHeight);
	bool InitializeDepthStencilBuffer();
	bool InitializeDepthStencilView();
	bool InitializeRasterilView();
	void InitializeViewport(int screenWight, int screenHeight);
	bool InitializeAlphaBlending();
	bool InitializeZbuffer();

	bool m_vsync_enabled;
	int m_videoCardMemory;
	char *m_videoCardDescription[128];
	

	

	D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
};
