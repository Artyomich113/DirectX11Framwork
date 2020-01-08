#include "Framework.h"
#include <iostream>
#include <thread>
#include <chrono>

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

Framework::Framework()
{
	curtime = 0;
	RunStartTime = 0;

	static bool raw_input_initialized = false;
	if (raw_input_initialized == false)
	{
		RAWINPUTDEVICE rid;

		rid.usUsagePage = 0x01;
		rid.usUsage = 0x02;
		rid.dwFlags = 0;
		rid.hwndTarget = NULL;

		if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
		{
			exit(-1);
		}
		raw_input_initialized = true;
	}
	IsRunning = true;

	ComponentsByLayout.insert( std::pair<Component::Layout,shared<std::list<Component*>>>(Component::Layout::GameLogic, shared<std::list<Component*>>(new std::list<Component*>)));
	ComponentsByLayout.insert( std::pair<Component::Layout, shared<std::list<Component*>>>(Component::Layout::Render, shared<std::list<Component*>>(new std::list<Component*>)));
}

void Framework::proceedAllGameobjects()
{
	for (auto iter = gameobjects.begin(); iter != gameobjects.end(); iter++)
	{
		(*iter)->procedure();
	}
}

bool Framework::Initialize(std::string name)
{
	dxmanager = new DXManager();
	
	//char *Name = new char[name.size() + 1];
	//strcpy_s(Name,sizeof(Name),name.c_str());
	if (!CreateDXWindow(name, WINDOW_POSX, WINDOW_POSY, SCREEN_HEIGHT, SCREEN_WIGHT)) 
	{
		return false;
	}
	if (FAILED(dxmanager->InitDevice(g_hWnd)))
	{
		return false;
	}
	return true;
	
}

void Framework::AddGameobject(Gameobject * object)
{
	gameobjects.push_back(object);
}

void Framework::AddComponent(Component *component)
{
	//std::cout << "\ncomponent type";
	Component::Layout type = component->ComponentType();


	//std::cout << "\npush back " << type;
	ComponentsByLayout[type]->push_back(component);
}

void Layout(shared<std::list<Component*>> components, std::chrono::milliseconds milliseconds)
{
	auto startTime = std::chrono::steady_clock::now();
	auto step = milliseconds;
	int i=0;
	while (Framework::instanse().IsRunning)
	{
		//std::cout << "\n" << ++i;
		startTime += step;
		for (auto it = components->begin(); it != components->end(); it++)
		{
			(*it)->process();
		}
		//std::cout << "clearRawInput";
		Framework::instanse().mouse.ClearRawInput();
		std::this_thread::sleep_until(startTime);


	}
}

MSG Framework::Run()
{

	//ShowCursor(false);
	std::cout << "\nRun";
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	auto ProggrammStartTime = std::chrono::steady_clock::now();
	auto startTime = std::chrono::steady_clock::now();
	auto step = std::chrono::milliseconds(1000l/ FRAME_RATE);
	MouseEvent me;

	shared<std::list<Component*>> RenderComponents = ComponentsByLayout[Component::Layout::Render];
	std::thread GameLogic(Layout,ComponentsByLayout[Component::Layout::GameLogic],std::chrono::milliseconds(20));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//std::cout << "\nlife sycle";
			
			startTime += step;
			auto curenttime = std::chrono::steady_clock::now();
			std::chrono::duration<float> deltatime = curenttime - ProggrammStartTime;
			auto timeinseconds =  std::chrono::duration_cast<std::chrono::milliseconds>(deltatime);
			//std::cout << (float)timeinseconds.count()/1000.0f << std::endl;
			curtime = (float)timeinseconds.count() / 1000.0f;
			cosTime = cos(curtime);
			//std::cout << cosTime << std::endl;

			float ClearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; // красный, зеленый, синий, альфа
			dxmanager->m_deviceContext->ClearRenderTargetView(dxmanager->m_renderTargetView, ClearColor);
			// Очистить буфер глубин до 1.0 (максимальное значение)
			dxmanager->m_deviceContext->ClearDepthStencilView(dxmanager->m_depthStencilview, D3D11_CLEAR_DEPTH, 1.0f, 0);
			//std::cout << "\nprocced gm";
			while (!mouse.EventBufferIsEmpty())
			{
				me = mouse.ReadEvent();
				//std::cout << "\nevent " << me.GetPosX() << " " << me.GetPosY();
			}
			//std::cout <<"\n" <<  mouse.RawInput.x << " " << mouse.RawInput.y;
			//proceedAllGameobjects();
			for (auto it = RenderComponents->begin(); it != RenderComponents->end(); it++)
			{
				(*it)->process();
			}
			/*std::cout << "clearRawInput";
			mouse.ClearRawInput();*/
			dxmanager->m_swapChain->Present(0, 0);

			if (GetKeyState('L') & 0x8000)
			{
				msg.message = WM_QUIT;
				IsRunning = false;
				GameLogic.join();
			}
			std::this_thread::sleep_until(startTime);
		}
	}

	return msg;
}

bool Framework::CreateDXWindow(std::string name, int x, int y, int height, int wight) {
	this->name = name;

	g_hIns = GetModuleHandle(NULL);

	WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = g_hIns;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = name.c_str();//"Urok4WindowClass";
    wc.hIconSm = wc.hIcon;
	if (!RegisterClassEx(&wc))
		return E_FAIL;

	int screenwight = GetSystemMetrics(SM_CXSCREEN);
	int screenheight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN)
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = screenwight;
		dmScreenSettings.dmPelsHeight = screenheight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
	}

	int nstyle = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX;

	g_hWnd = CreateWindowEx(WS_EX_APPWINDOW, name.c_str(), name.c_str(), nstyle, x, y, wight, height, NULL, NULL, g_hIns, NULL);

	if (!g_hWnd)
	{
		MessageBox(NULL, "CreateWindowEX() Failed", "Error", MB_OK);
		PostQuitMessage(0);
		return false;
	}
	ShowWindow(g_hWnd, SW_SHOW);
	SetForegroundWindow(g_hWnd);
	SetFocus(g_hWnd);

	return true;

	/*if (!g_hWnd)
		return E_FAIL;

	ShowWindow(g_hWnd, nCmdShow);

	return S_OK;*/
}

void cleanupLayout(std::shared_ptr<std::list<Component*>> listptr)
{
	for (auto it = listptr->begin(); it != listptr->end(); it++)
	{
		std::cout << "\ndeleting component " << *it;
		delete *it;
	}
}

Framework::~Framework()
{
    if (FULL_SCREEN)
    {
        ChangeDisplaySettings(NULL, 0);
    }
	UnregisterClass(name.c_str(),g_hIns);
	g_hIns = nullptr;
	for (auto it = gameobjects.begin(); it != gameobjects.end(); it++)
	{
		delete (*it);
	}
	//gameobjects.erase(gameobjects.begin(),gameobjects.end());
	delete dxmanager;
	cleanupLayout(ComponentsByLayout[Component::Layout::GameLogic]);
	cleanupLayout(ComponentsByLayout[Component::Layout::Render]);
	
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		DestroyWindow(hwnd);
		break;
	case WM_MOUSEMOVE :{
		//std::cout << "\nmouse move";
		//int x = LOWORD(lparam);
		//int y = HIWORD(lparam);
		Framework::instanse().mouse.OnMouseMove(LOWORD(lparam), HIWORD(lparam));
		break;}
	case WM_MOUSEWHEEL:
	{
		int x= LOWORD(lparam);
		int y = HIWORD(lparam);
		if (GET_WHEEL_DELTA_WPARAM(wparam) > 0)
		{
			Framework::instanse().mouse.OnWheelUp(x,y);
		}
		else if(GET_WHEEL_DELTA_WPARAM(wparam) < 0)
		{
			Framework::instanse().mouse.OnWheelDown(x,y);
		}
		break;
	}
	case WM_INPUT:
	{
		UINT dataSize;

		GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam),RID_INPUT,NULL,&dataSize, sizeof(RAWINPUTHEADER));
		if (dataSize > 0)
		{
			std::unique_ptr<BYTE[]> rawdata = std::make_unique<BYTE[]>(dataSize);
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, rawdata.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
			{
				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.get());
				if (raw->header.dwType == RIM_TYPEMOUSE)
				{
					Framework::instanse().mouse.OnMouseMoveRaw(raw->data.mouse.lLastX,raw->data.mouse.lLastY);
				}
			}
		}
		return DefWindowProc(hwnd, message, wparam, lparam);
		break;
	}
	case WM_LBUTTONDOWN :
		Framework::instanse().mouse.OnMouseMove(LOWORD(lparam), HIWORD(lparam));
		break;
	case WM_LBUTTONUP:
		Framework::instanse().mouse.OnLeftRelesed(LOWORD(lparam), HIWORD(lparam));
		break;
	case WM_RBUTTONDOWN:
		Framework::instanse().mouse.OnRightPressed(LOWORD(lparam), HIWORD(lparam));
		break;
	case WM_RBUTTONUP:
		Framework::instanse().mouse.OnRightRelesed(LOWORD(lparam), HIWORD(lparam));
		break;
	case WM_MBUTTONDOWN:
		Framework::instanse().mouse.OnMiddlePressed(LOWORD(lparam), HIWORD(lparam));
		break;
	case WM_MBUTTONUP:
		Framework::instanse().mouse.OnMiddleRelesed(LOWORD(lparam), HIWORD(lparam));
		break;
	default:
		return DefWindowProc(hwnd, message, wparam, lparam);
	}
	return 0;
}

