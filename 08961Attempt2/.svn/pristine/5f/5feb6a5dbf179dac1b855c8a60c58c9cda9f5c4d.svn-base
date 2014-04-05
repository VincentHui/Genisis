////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "systemclass.h"

D3DClass * sysDevice = new D3DClass;
SystemClass::SystemClass(): PtrRotation(0)
{
	m_Input = 0;
	m_Position = 0;
	m_Graphics = 0;

	// Create the timer object.
	m_Timer = new TimerClass;

	// Initialize the timer object.
	bool result = m_Timer->Initialize();

	// Create the position object.
	m_Position = new PositionClass;
	m_Position->SetPosition(0.0f, 0.0f, 0.0f);
	m_Position->SetRotation(0.0f, 0.0f, 0.0f);
	m_width = 0;
	m_height = 0;
}


SystemClass::SystemClass(const SystemClass& other)
{
}


SystemClass::~SystemClass()
{
}


bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	m_Input = new InputClass;
	if(!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	result = m_Input->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	m_d3d = sysDevice;
	result = m_d3d->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, m_hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		Logger::AppendErrorToLog("Direct3D failed to initilise in GraphicsClass::Initialize");
		MessageBox(m_hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
	}

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_Graphics = new GraphicsClass(screenWidth, screenHeight, m_hwnd, m_d3d, SCREEN_NEAR, SCREEN_DEPTH);
	if(!m_Graphics)
	{
		return false;
	}
	// Initialize the graphics object.
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if(!result)
	{
		return false;
	}
	//m_Position = new PositionClass;
	//TwInit(TW_DIRECT3D11, m_Graphics->GetD3DDevice());
	//m_bar = TwNewBar("system");
	//m_Graphics->GetQuat(PtrRotation);
	//TwAddVarRW(m_bar, "World Rotation", TW_TYPE_QUAT4F, &m_Graphics->Quaternion, "opened=true axisz=-z group=Graphics");
	return true;
}


void SystemClass::Shutdown()
{
	// Release the graphics object.
	if(m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// Release the input object.
	if(m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	delete m_Position;
	m_Position = 0;

	PtrRotation = 0;
	delete PtrRotation;
	// Shutdown the window.
	ShutdownWindows();


	return;
}

bool SystemClass::HandleInput(float frameTime)
{
	bool keyDown;

	// Set the frame time for calculating the updated position.
	m_Position->SetFrameTime(frameTime);

	keyDown = m_Input->IsKeyHeld(DIK_LEFT);
	m_Position->TurnLeft(keyDown);

	keyDown = m_Input->IsKeyHeld(DIK_RIGHT);
	m_Position->TurnRight(keyDown);

	keyDown = m_Input->IsKeyHeld(DIK_UP);
	m_Position->MoveForward(keyDown);

	keyDown = m_Input->IsKeyHeld(DIK_DOWN);
	m_Position->MoveBackward(keyDown);

	keyDown = m_Input->IsKeyHeld(DIK_A);
	m_Position->MoveUpward(keyDown);

	keyDown = m_Input->IsKeyHeld(DIK_Z);
	m_Position->MoveDownward(keyDown);

	keyDown = m_Input->IsKeyHeld(DIK_PGUP);
	m_Position->LookUpward(keyDown);

	keyDown = m_Input->IsKeyHeld(DIK_PGDN);
	m_Position->LookDownward(keyDown);

	return true;
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while(!done)
	{
		// Handle the windows messages.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if(msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			result = Frame();
			if(!result)
			{
				MessageBox(m_hwnd, L"Frame Processing Failed", L"Error", MB_OK);
				done = true;
			}
		}

		// Check if the user pressed escape and wants to quit.
		if(m_Input->IsEscapePressed() == true)
		{
			done = true;
		}

		//TwAddVarRW(ABar, "zoom", TW_TYPE_FLOAT, )
	}

	return;
}


bool SystemClass::Frame()
{

	bool result;
	int mouseX, mouseY;

	// Do the input frame processing.
	result = m_Input->Frame();
	if(!result)
	{
		return false;
	}

	// Get the location of the mouse from the input object,
	m_Input->GetMouseLocation(mouseX, mouseY);

	m_Timer->Frame();
	HandleInput(m_Timer->GetTime());

	XMFLOAT3 pos, rot;
	m_Position->GetPosition(pos.x, pos.y, pos.z);
	m_Position->GetRotation(rot.x, rot.y, rot.z);
	float width = m_d3d->g_DepthStencilDesc.Width;
	float height = m_d3d->g_DepthStencilDesc.Height;

	// Do the frame processing for the graphics object.
	result = m_Graphics->Frame(mouseX, mouseY, pos, rot,  width, height, m_Timer->GetTime());
	if(!result)
	{
		return false;
	}

	// Finally render the graphics to the screen.
	//result = m_Graphics->Render(0.0f, width, height, m_Timer->GetTime());
	if(!result)
	{
		return false;
	}
	m_d3d->EndScene();
	return true;
}


LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{

	return DefWindowProc(hwnd, umsg, wparam, lparam);
}


void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	//RECT rc = { 0, 0, 640, 480 };
	//   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Engine";

	// Setup the windows class with default settings.
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = m_hinstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm       = wc.hIcon;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize        = sizeof(WNDCLASSEX);
	
	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if(FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		
		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;

		m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, 
						WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
						posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);
	}
	else
	{
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX), CS_HREDRAW|CS_VREDRAW, WndProc,
			0L, 0L, m_hinstance, NULL, NULL, NULL, NULL, L"TwDX11", NULL };
		RegisterClassEx(&wcex);
		// Create the window with the screen settings and get the handle to it.
		RECT rc = { 0, 0, 800, 600 };
		//screenWidth = 640;
		//screenHeight = 480;
		screenWidth = 800;
		screenHeight = 600;
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
		m_hwnd = CreateWindow(L"TwDX11", L"AntTweakBar simple example using DirectX11", 
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
			rc.right-rc.left, rc.bottom-rc.top, NULL, NULL, m_hinstance, NULL);
	}




	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(true);

	return;
}


void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if(FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	/*m_d3d->GetDevice();*/
	return;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	D3DClass *  g_D3DDev = sysDevice;
	if(TwEventWin(hwnd, umessage, wparam, lparam))
	{
		return 0;
	}
	switch(umessage)
	{

	case WM_SIZE: // Window size has been changed
		{
			g_D3DDev->Resize( hwnd,  umessage,  wparam,  lparam);
			return 0;
		}
		// Check if the window is being destroyed.
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
	case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}

		// All other messages pass to the message handler in the system class.
	default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}

ID3D11Device* SystemClass::GetGraphicsDevice()
{
	return m_Graphics->GetD3DDevice();
}