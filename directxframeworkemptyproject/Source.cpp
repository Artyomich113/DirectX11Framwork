#include <iostream>

#include "Camera.h"
#include "Framework.h"
#include "Mesh/MeshRenderer.h"
#include "Mesh/MeshRendererUI.h"
#include "Mesh/MeshRendererTextured.h"
#include "Mesh/Mesh.h"
#include "Behaviour.h"

SimpleVertex Pyramidvertices[] =
{	/* координаты X, Y, Z				цвет R, G, B, A				*/
	{ XMFLOAT3(0.0f,  1.5f,  0.0f), XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(-1.0f,  0.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
	{ XMFLOAT3(1.0f,  0.0f, -1.0f), XMFLOAT4(0.0f, 0.9f, 0.9f, 1.0f) },
	{ XMFLOAT3(-1.0f,  0.0f,  1.0f), XMFLOAT4(0.0f, 0.8f, 0.8f, 1.0f) },
	{ XMFLOAT3(1.0f,  0.0f,  1.0f), XMFLOAT4(0.0f, 0.7f, 0.7f, 1.0f) }
};

WORD Pyramidindices[] =
{	// индексы массива vertices[], по которым строятся треугольники
	0,2,1,	/* Треугольник 1 = vertices[0], vertices[2], vertices[3] */
	0,3,4,	/* Треугольник 2 = vertices[0], vertices[3], vertices[4] */
	0,1,3,	/* и т. д. */
	0,4,2,
	1,2,3,
	2,4,3,
};

TexturedVertex CubeverticesTextured[] =
{
	{ XMFLOAT3(1.0f,  2.0f,  -1.0f), XMFLOAT2(1.0f, 1.0f) , XMFLOAT3(1.0f,1.0f,1.0f) },
	{ XMFLOAT3(-1.0f,  2.0f,  1.0f), XMFLOAT2(0.0f, 0.0f) , XMFLOAT3(1.0f,1.0f,1.0f)},
	{ XMFLOAT3(1.0f,  2.0f,  1.0f),  XMFLOAT2(1.0f, 0.0f) , XMFLOAT3(1.0f,1.0f,1.0f)},
	{ XMFLOAT3(-1.0f,  2.0f,  -1.0f),XMFLOAT2(0.0f, 1.0f) , XMFLOAT3(1.0f,1.0f,1.0f)},
	{ XMFLOAT3(-1.0f,  0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) , XMFLOAT3(1.0f,1.0f,1.0f)},
	{ XMFLOAT3(1.0f,  0.0f, -1.0f),  XMFLOAT2(1.0f, 0.0f) , XMFLOAT3(1.0f,1.0f,1.0f)},
	{ XMFLOAT3(-1.0f,  0.0f,  1.0f), XMFLOAT2(1.0f, 1.0f) , XMFLOAT3(1.0f,1.0f,1.0f)},
	{ XMFLOAT3(1.0f,  0.0f,  1.0f),  XMFLOAT2(0.0f, 0.0f) , XMFLOAT3(1.0f,1.0f,1.0f)}
};

TexturedVertex PlaneVertex[] =
{
	{ XMFLOAT3(0.0f,  0.0f,  0.0f), XMFLOAT2(0.0f, 10.0f) ,XMFLOAT3(0.0f,1.0f,0.0f)},
	{ XMFLOAT3(0.0f,  0.0f,  1.0f), XMFLOAT2(0.0f, 0.0f)   ,XMFLOAT3(0.0f,1.0f,0.0f)},
	{ XMFLOAT3(1.0f,  0.0f,  1.0f), XMFLOAT2(10.0f, 0.0f)  ,XMFLOAT3(0.0f,1.0f,0.0f)},
	{ XMFLOAT3(1.0f,  0.0f,  0.0f), XMFLOAT2(10.0f, 10.0f) ,XMFLOAT3(0.0f,1.0f,0.0f)},
};

WORD PlaneIndices[] =
{
	0,1,3,
	3,1,2,
};

SimpleVertex Cubevertices[] =
{	/* координаты X, Y, Z				цвет R, G, B, A					 */
	{ XMFLOAT3(1.0f,  2.0f,  -1.0f), XMFLOAT4(0.0f, 0.0f, 2.0f, 1.0f) },
	{ XMFLOAT3(-1.0f,  2.0f,  1.0f), XMFLOAT4(0.0f, 0.0f, 3.0f, 1.0f) },
	{ XMFLOAT3(1.0f,  2.0f,  1.0f), XMFLOAT4(0.0f, 0.0f, 4.0f, 1.0f) },
	{ XMFLOAT3(-1.0f,  2.0f,  -1.0f), XMFLOAT4(0.0f, 0.0f, 5.0f, 1.0f) },
	{ XMFLOAT3(-1.0f,  0.0f, -1.0f), XMFLOAT4(2.0f, 0.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(1.0f,  0.0f, -1.0f), XMFLOAT4(3.0f, 0.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(-1.0f,  0.0f,  1.0f), XMFLOAT4(4.0f, 0.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(1.0f,  0.0f,  1.0f), XMFLOAT4(5.0f, 0.0f, 0.0f, 1.0f) }
};

WORD Cubeindices[] =
{	// индексы массива vertices[], по которым строятся треугольники
	0,1,2,
	1,0,3,
	3,5,4,
	5,3,0,
	2,5,0,
	5,2,7,
	2,6,7,
	6,2,1,
	1,3,6,
	3,4,6,
	4,5,6,
	6,5,7,
};

//int main(int argc, char *argv[])
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	try
	{
		Framework::instanse().Initialize("WindowXD");

		auto *PyrMR = new MeshRendererUI(Pyramidvertices, Pyramidindices, 18, 5);
		if (FAILED(PyrMR->InitShader("Shaders.fx", "VSUI", "PSUI")))
		{
			std::cout << "\nfailed init shader uishader";
			throw 1;
		}

		auto *CubeMR = new MeshRenderer(Cubevertices, Cubeindices, 36, 8);
		if (FAILED(CubeMR->InitShader("Shaders.fx", "VS", "PS")))
		{
			std::cout << "\nfailed init shader shaders vs ps";
			throw 1;
		}

		auto *CubeTex = new MeshRendererTextured("aquafish01.png", CubeverticesTextured, Cubeindices, 36, 8);
		if (FAILED(CubeTex->InitShader("Shaders.fx", "VSTEX", "PSTEX")))
		{
			std::cout << "\nfailed init shader shaders vstex pstex1";
			throw 1;
		}

		auto *PlaneTexture = new MeshRendererTextured("josuka.dds", PlaneVertex, PlaneIndices, 6, 4);

		if (FAILED(PlaneTexture->InitShader("Shaders.fx", "VSTEX", "PSTEX")))
		{
			std::cout << "\nfailed init shader shaders vstex pstex2";
			throw 1;
		}
		
		auto Fish = new MeshRendererTextured("aquafish01.obj", "aquafish01.png");
		Fish->LoadFromObj();
		if (FAILED(Fish->InitShader("Shaders.fx", "VSTEX", "PSTEX")))
		{
			std::cout << "\nfailed init shader shaders vstex pstex3";
			throw 1;
		}
		
		if (FAILED(Fish->InitMesh()))
		{
			std::cout << "\nfailed init mesh";
			throw 1;
		}
		

		if (FAILED(PyrMR->InitMesh()))
		{
			std::cout << "\nfailed init mesh";
			throw 1;
		}
		if (FAILED(CubeMR->InitMesh()))
		{
			std::cout << "\nfailed init mesh";
			throw 1;
		}
		if (FAILED(CubeTex->InitMesh()))
		{
			std::cout << "\nfailed init meshTex";
			throw 1;
		}
		if (FAILED(PlaneTexture->InitMesh()))
		{
			std::cout << "\nfailed init meshTex";
			throw 1;
		}

		
		if (FAILED(Framework::instanse().dxmanager->InitMatrixes(Framework::instanse().g_hWnd)))
		{
			std::cout << "failed init matrixes";
			throw 1;
		}
		
		
		Gameobject* P1 = new Gameobject("P1");
		Gameobject* Plane = new Gameobject("Plane");
		Gameobject* Player = new Gameobject("Player");
		Gameobject* C1 = new Gameobject("C1");
		Gameobject* C2 = new Gameobject("C2");
		Gameobject* C3 = new Gameobject("C3");

		P1->transform->Position = XMVectorSet(-0.5f, -0.0f, 0.5f, 0.0f);
		P1->transform->LocalScale = XMVectorSet(0.5f, 0.5f, 0.5f, 0.0f) / 3;

		Plane->transform->Position = XMVectorSet(0.0f, -5.0f, 0.0f, 0.0f);
		Plane->transform->LocalScale = XMVectorSet(10.0f, 1.f, 10.0f, 0.0f);


		C1->transform->Position = XMVectorSet(-2.0f, 0.0f, 0.0f, 0.0f);
		C1->transform->LocalScale = XMVectorSet(0.5f, 0.5f, 0.5f, 0.0f);

		C2->transform->Position = XMVectorSet(0.0f, 2.1f, 0.0f, 0.0f);
		C2->transform->LocalScale = XMVectorSet(0.1f, 0.1f, 0.1f, 0.0f);

		C3->transform->Position = XMVectorSet(-2.2f, 0.0f, 0.0f, 0.0f);
		C3->transform->LocalScale = XMVectorSet(0.5f, 0.5f, 0.5f, 0.0f);


		Camera* cam = new Camera();


		Framework::instanse().camera = cam;
		Controller* cont = new Controller(2.0f, 5.0f);
		//RotateQ *rotateQ = new RotateQ(30.0f/FRAME_RATE, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
		RotateQ *RotQ111 = new RotateQ(30.0f / FRAME_RATE, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

		//Translator *trans = new Translator(XMVectorSet(-1.0f / FRAME_RATE, 0, 0.0f, 0));
		//Translator *trans2 = new Translator(XMVectorSet(1.0f / FRAME_RATE, 0, 0.0f, 0));
		RotateAround *rotatearoundY = new RotateAround(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), 30.0f / FRAME_RATE);
		//RotateAround *rotatearoundX = new RotateAround(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), 30.0f / FRAME_RATE);
		RotateAround *rotatearoundZ = new RotateAround(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), 30.0f / FRAME_RATE);

		//RotateAround *rotatearound2 = new RotateAround(*rotatearound);

		//Pyramid->AddComponent(rotateQ);

		//gameobject->AddComponent(trans);

		Player->AddComponent(cam);
		Player->AddComponent(cont);
		Player->transform->Position = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		//C1->AddComponent(rotatearound);
		C1->AddComponent(rotatearoundY);
		C1->AddComponent(CubeMR);
		//C1->AddComponent(RotQ);
		C2->AddComponent(Fish);
		//C2->AddComponent(rotatearoundX);

		C3->AddComponent(CubeTex);
		C3->AddComponent(rotatearoundZ);

		//Pyramid->AddComponent(rotatearound2);
		//P1->AddComponent(rotateQ);
		//P1->AddComponent(rotateQ);
		//P1->AddComponent(RotQ111);
		P1->AddComponent(PyrMR);

		Plane->AddComponent(PlaneTexture);



		Framework::instanse().AddGameobject(Player);
		Framework::instanse().AddGameobject(P1);
		Framework::instanse().AddGameobject(C1);
		Framework::instanse().AddGameobject(C2);
		Framework::instanse().AddGameobject(C3);
		Framework::instanse().AddGameobject(Plane);
		//Framework::instanse().AddGameobject(P1);
		//delelop
		Framework::instanse().Run();
	}
	catch (std::exception e)
	{
		MessageBox(nullptr, e.what(), "Error", MB_OK | MB_ICONERROR);
	}
	catch (...)
	{
		MessageBox(nullptr, "wat", "Error", MB_OK | MB_ICONERROR);
	}
	//Framework::instanse().~Framework();
	//system("pause");
	return 0;
}