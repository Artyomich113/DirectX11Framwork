#include "MeshRendererTextured.h"
#include <mmsystem.h>
#include <fstream>

MeshRendererTextured::MeshRendererTextured(const MeshRendererTextured &ob)
{
	NumOfIndexes = ob.NumOfIndexes;
	NumOfVerteces = ob.NumOfVerteces;

	g_pVertexBuffer = ob.g_pVertexBuffer;
	g_pIndexBuffer = ob.g_pIndexBuffer;
	g_pConstantBuffer = ob.g_pConstantBuffer;

	g_pTextureRV = ob.g_pTextureRV;
	g_pSamplerLinear = ob.g_pSamplerLinear;

	shaderPointers = ob.shaderPointers;
	dxmanager = Framework::instanse().GetDXManager();
	copy = true;
}

HRESULT MeshRendererTextured::InitMesh()
{
	std::cout << "init mesh\n";
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;	// ���������, ����������� ����������� �����
	//std::cout << "\nzero memory";
	ZeroMemory(&bd, sizeof(bd));				// ������� ��
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(TexturedVertex) * NumOfVerteces;	// ������ ������
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// ��� ������ - ����� ������
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;	// ���������, ���������� ������ ������
	//std::cout << "\nzero memory";
	ZeroMemory(&InitData, sizeof(InitData));	// ������� ��
	InitData.pSysMem = Vertices;
	// ����� ������ g_pd3dDevice ������� ������ ������ ������
	/*for (int i = 0; i < NumOfVerteces + 5; i++)
	{
		std::cout << i << ")" << Vertices[i].TexPos.x << " " << Vertices[i].TexPos.y << "\n";
	}*/
	std::cout << "create vertex buffer ind " << NumOfVerteces << "\n"
		<< " byte width " << bd.ByteWidth << "\n"
		<< "sizeof " << sizeof(TexturedVertex) << "\n"
		<< "mem " << InitData.pSysMem << "\n";
	hr = dxmanager->m_device->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
	std::cout << "buffer created\n";
	if (FAILED(hr))
	{
		std::cout << "failed create vertices buffer " << NumOfVerteces << " verteces\n";
		return hr;
	}
	ZeroMemory(&bd, sizeof(bd)); //
	bd.Usage = D3D11_USAGE_DEFAULT;		// ���������, ����������� ����������� �����
	bd.ByteWidth = sizeof(WORD) * NumOfIndexes;	// ��� 6 ������������� ���������� 18 ������
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER; // ��� - ����� ��������
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = Indices;				// ��������� �� ��� ������ ��������
	// ����� ������ g_pd3dDevice ������� ������ ������ ��������
	std::cout << "creating indeces buffer \n ";
	hr = dxmanager->m_device->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
	std::cout << "buffer created\n";

	if (FAILED(hr))
	{
		std::cout << "\nfailed create indices buffer";
		return hr;
	}


	dxmanager->m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// �������� ������������ ������
	bd.Usage = D3D11_USAGE_DEFAULT;

	bd.ByteWidth = sizeof(ConstantBuffer);		// ������ ������ = ������� ���������
	//std::cout << "text ConstantBuffer width " << sizeof(ConstantBuffer) << std::endl;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// ��� - ����������� �����
	bd.CPUAccessFlags = 0;

	hr = dxmanager->m_device->CreateBuffer(&bd, NULL, &g_pConstantBuffer);
	if (FAILED(hr))
	{
		std::cout << "\nfailed create costant buffer";
		return hr;
	}

	hr = D3DX11CreateShaderResourceViewFromFile(dxmanager->m_device, TextureName.c_str(), NULL, NULL, &g_pTextureRV, NULL);
	if (FAILED(hr))
	{
		std::cout << "\n failed createShaderResourceView";
		return hr;
	}

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// ��� ����������
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;//D3D11_TEXTURE_ADDRESS_WRAP;		
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;//D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = 4;
	// ������� ��������� ������ ���������������
	hr = dxmanager->m_device->CreateSamplerState(&sampDesc, &g_pSamplerLinear);
	if (FAILED(hr))
	{
		std::cout << "\nfailed create sample state";
		return hr;
	}
	std::cout << "\ninit mesh/";
	return hr;
}

void MeshRendererTextured::LoadFromObj()
{
	std::cout << "Loading texture from file " << modelpath << std::endl;

	std::string FileName = modelpath;
	//---------------------------------------------------------------------------
	// ������ ������������ ���������� ������ � ��������. ��� ��������
	// ����� ��������� ���� ��������� ��������� ������� ����       
	//---------------------------------------------------------------------------
	const int buffermax = 16384;
	//---------------------------------------------------------------------------

	//VERTEX * bufferVertices = new VERTEX[buffermax];		// ������� ��� ���������� �������� ������ � ��������
	TexturedVertex * bufferVertices = new TexturedVertex[buffermax];
	WORD * bufferIndices = new WORD[buffermax];
	int verticesI = 0;													// ������� � ���� ��������
	int indicesI = 0;

	//�������� ���������� ��������� �������� ������, ��� ������ ������ �����
	//��������� � ���������� ������ ��������� ��� ������ � ��������� � ��������� ������
	XMFLOAT3 * Positions = new XMFLOAT3[buffermax];
	XMFLOAT2 * TexCoords = new XMFLOAT2[buffermax];
	XMFLOAT3 * Normals = new XMFLOAT3[buffermax];
	// ������� ��� ��������
	int PositionsI = 0;
	int TexCoordsI = 0;
	int NormalsI = 0;

	// ���� ������ �� �����
	//char fname[256];
	//strcpy(fname,Edit1->Text.c_str());
	WCHAR strCommand[256] = { 0 };
	std::wifstream InFile(FileName.c_str());

	if (!InFile)
		return;

	for (; ; )
	{
		InFile >> strCommand;
		//std::cout << strCommand << std::endl;
		if (!InFile)
			break;

		if (0 == wcscmp(strCommand, L"#"))
		{
			// �����������
		}
		else if (0 == wcscmp(strCommand, L"v"))
		{
			// ����������
			float x, y, z;
			InFile >> x >> y >> z;
			//float c=0.05f;
			//std::cout << PositionsI << " v" << x << " " << y << " " << z << "\n";
			Positions[PositionsI++] = XMFLOAT3(x, y, z);
		}
		else if (0 == wcscmp(strCommand, L"vt"))
		{

			// ���������� ����������
			float u, v;
			InFile >> u >> v;
			TexCoords[TexCoordsI++] = XMFLOAT2(u, v);
			//std::cout << "vt" << u << " " << v << "\n";
		}
		else if (0 == wcscmp(strCommand, L"vn"))
		{
			//std::cout << "vn\n";

			// �������
			float x, y, z;
			InFile >> x >> y >> z;
			Normals[NormalsI++] = XMFLOAT3(x, y, z);
		}
		else if (0 == wcscmp(strCommand, L"f"))
		{
			//std::cout << "f\n";

			// Face
			UINT iPosition, iTexCoord, iNormal;
			TexturedVertex vertex;

			for (UINT iFace = 0; iFace < 3; iFace++)
			{
				ZeroMemory(&vertex, sizeof(TexturedVertex));

				// OBJ ������ ���������� ������� � ��������� �� 1
				InFile >> iPosition;
				vertex.Pos = Positions[iPosition - 1];

				if ('/' == InFile.peek())
				{
					InFile.ignore();

					if ('/' != InFile.peek())
					{
						// �������� ���������� ����������
						InFile >> iTexCoord;
						vertex.TexPos = XMFLOAT2(TexCoords[iTexCoord - 1]);//(TexCoords[iTexCoord - 1].x, TexCoords[iTexCoord - 1].y);
					}

					if ('/' == InFile.peek())
					{
						InFile.ignore();

						// �������� �������
						InFile >> iNormal;
						vertex.Norm = Normals[iNormal - 1];
					}
				}

				//��������� ������� � ������
				int index = -1;
				//����� ������������ �������
				for (int i = 0; i < verticesI; i++)
					if (memcmp(&bufferVertices[i], &vertex, sizeof(TexturedVertex)) == 0)
					{
						index = i;
						//break;
					}
				//����������
				if (index < 0)
				{
					bufferVertices[verticesI++] = vertex;
					index = verticesI - 1;
				}


				bufferIndices[indicesI++] = index;
			}
		}
	}
	InFile.close();

	std::cout << "Total vertices " << verticesI << std::endl;
	std::cout << "Total indices " << indicesI << std::endl;

	NumOfVerteces = verticesI;
	Vertices = new TexturedVertex[verticesI];
	memcpy(Vertices, bufferVertices, verticesI * sizeof(TexturedVertex));

	NumOfIndexes = indicesI;
	Indices = new WORD[indicesI];
	memcpy(Indices, bufferIndices, indicesI * sizeof(WORD));


	//SetVerticesIndices(bufferVertices, verticesI, bufferIndices, indicesI, 0);

	//������� ��������� ��������
	delete[] Positions;
	delete[] TexCoords;
	delete[] Normals;
	delete[] bufferVertices;
	delete[] bufferIndices;

}

MeshRendererTextured::MeshRendererTextured(std::string TextureName, TexturedVertex *vertices, WORD* indices, int NumberOfIndexes, int NumberOfVerteces)
{
	//std::cout << " new meh Renderer Textured";
	Vertices = vertices;
	Indices = indices;
	this->NumOfIndexes = NumberOfIndexes;
	this->NumOfVerteces = NumberOfVerteces;
	dxmanager = Framework::instanse().GetDXManager();
	this->TextureName = TextureName;
}

MeshRendererTextured::MeshRendererTextured(std::string ModelPath, std::string texturename)
{
	this->TextureName = texturename;
	modelpath = ModelPath;
	dxmanager = Framework::instanse().GetDXManager();
}



D3D11_INPUT_ELEMENT_DESC * MeshRendererTextured::layout()
{
	D3D11_INPUT_ELEMENT_DESC * layout = new D3D11_INPUT_ELEMENT_DESC[3];

	layout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	layout[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	layout[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	return layout;
}

UINT MeshRendererTextured::NumberOfElements()
{
	return (UINT)3;
}

MeshRendererTextured::~MeshRendererTextured()
{
	std::cout << "\n~MeshRendererTextured";
	if (g_pSamplerLinear)g_pSamplerLinear->Release();
	if (g_pTextureRV)g_pTextureRV->Release();
}


void MeshRendererTextured::process()
{
	XMMATRIX mRotation = XMMatrixRotationQuaternion(gameobject->transform->Rotation);
	XMMATRIX mTranslate = XMMatrixTranslationFromVector(gameobject->transform->Position);
	XMMATRIX mscale = XMMatrixScalingFromVector(gameobject->transform->LocalScale);
	XMMATRIX g_World = mscale * mRotation * mTranslate;

	ConstantBuffer cb;

	cb.mWorld = XMMatrixTranspose(g_World);
	cb.mView = XMMatrixTranspose(Framework::instanse().camera->g_view);
	cb.mProjection = XMMatrixTranspose(Framework::instanse().camera->g_Projection);
	cb.CameraPos = Framework::instanse().camera->gameobject->transform->Position.m128_f32;
	cb.LightColor = Framework::instanse().DirLight->lightColor;
	cb.LightDir = Framework::instanse().DirLight->lightDirection.m128_f32;

	cb.pointLightColors[0] = Framework::instanse().pointLights[0]->lightColor;
	cb.pointLightColors[1] = Framework::instanse().pointLights[1]->lightColor;

	cb.pointLightPos[0] = Framework::instanse().pointLights[0]->lightPos.m128_f32;
	cb.pointLightPos[1] = Framework::instanse().pointLights[1]->lightPos.m128_f32;
	
	cb.pointLightPower[0] = Framework::instanse().pointLights[0]->lightPower;
	cb.pointLightPower[1] = Framework::instanse().pointLights[1]->lightPower;

	cb.costime = Framework::instanse().cosTime;
	cb.curtime = Framework::instanse().curtime;
	//std::cout << cb.costime << std::endl;

	//std::cout << "\nsubresource";
	UINT stride = sizeof(TexturedVertex);
	UINT offset = 0;


	ID3D11DeviceContext *id3d11devicecontext = dxmanager->m_deviceContext;

	id3d11devicecontext->IASetInputLayout(shaderPointers.g_pVertexLayout);

	id3d11devicecontext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);
	id3d11devicecontext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	id3d11devicecontext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);


	//std::cout << "\nContext";
	//id3d11devicecontext->VSSetShader(dxmanager->m_VertexShader, NULL, 0);
	id3d11devicecontext->VSSetShader(shaderPointers.m_VertexShader, NULL, 0);
	id3d11devicecontext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	id3d11devicecontext->PSSetShader(shaderPointers.m_PixelShader, NULL, 0);

	id3d11devicecontext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);

	id3d11devicecontext->PSSetShaderResources(0, 1, &g_pTextureRV);
	id3d11devicecontext->PSSetSamplers(0, 1, &g_pSamplerLinear);

	id3d11devicecontext->DrawIndexed(NumOfIndexes, 0, 0);
	//std::cout << "\nTextMR/";
}
