#pragma once
//#include 
#include "../DXManager.h"
#include "../Framework.h"



class Mesh : virtual public Component
{
protected:
	bool copy = false;
public:
	ShaderPointers shaderPointers;

	ID3D11Buffer*           g_pVertexBuffer = NULL;		// ����� ������
	ID3D11Buffer*           g_pIndexBuffer = NULL;		// ����� �������� ������
	ID3D11Buffer*           g_pConstantBuffer = NULL;	// ����������� ����� // �������� ������� �������� ��� ������� ���������, ������ ��� ����� ���� ��������� �������
	WORD * Indices;
	int NumOfIndexes;
	int NumOfVerteces;

	virtual D3D11_INPUT_ELEMENT_DESC* layout() = 0;
	virtual UINT NumberOfElements() = 0;


	HRESULT InitShader(std::string ShaderName, std::string vertexShaderFunctionName, std::string pixelShaderFunctionName);
	virtual HRESULT InitMesh() = 0;

	~Mesh();
	virtual void process() = 0;
};

struct SimpleVertex
{
	XMFLOAT3 Pos;	// ���������� ����� � ������������
	XMFLOAT4 Color;	// ������ ������ ������� ����� ��������� ���������� � �����
};

struct TexturedVertex
{
	XMFLOAT3 Pos;
	XMFLOAT2 TexPos;
	XMFLOAT3 Norm;
};

struct ConstantBuffer
{
	XMMATRIX mWorld;		// ������� ����
	XMMATRIX mView;			// ������� ����
	XMMATRIX mProjection;	// ������� ��������
	XMFLOAT4 CameraPos;
	float costime;
	float curtime;
};
//
//struct ConstantBufferUI
//{
//	XMMATRIX mWorld;		// ������� ����
//	XMMATRIX mProjection;	// ������� ��������
//};