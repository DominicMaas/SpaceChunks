#include "Window.hpp"

namespace XyEngine
{
	Window* m_pWindow = nullptr;

	LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (m_pWindow) return m_pWindow->MsgProc(hwnd, msg, wParam, lParam);
		else return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	Window::Window(UINT width, UINT height, char* title)
	{
		SetWindowWidth(width);
		SetWindowHeight(height);
		SetWindowTitle(title);

		SetWindowInstance((HINSTANCE)GetModuleHandle(NULL));
		SetWindowHandle(NULL);
		SetDeviceContext(NULL);
		SetOpenGLRenderContext(NULL);
		SetWindowStyle(WS_OVERLAPPEDWINDOW);

		m_pWindow = this;
	}

	Window::~Window()
	{

	}

	bool Window::Create()
	{
		WNDCLASSEX wcex;
		ZeroMemory(&wcex, sizeof(WNDCLASSEX));

		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.hInstance = this->m_pWindowInstance;
		wcex.lpfnWndProc = MainWndProc;
		wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = L"XYENGINE_GAME";
		wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		if (!RegisterClassEx(&wcex))
		{
			std::cout << "[XyEngine] [Window] Failed to Create Window Class" << std::endl;
			return false;
		}

		RECT r = { 0, 0, (LONG)m_pWindowWidth, (LONG)m_pWindowHeight };
		AdjustWindowRect(&r, m_pWindowStyle, FALSE);
		UINT width = r.right - r.left;
		UINT height = r.bottom - r.top;

		UINT x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
		UINT y = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;

		this->m_pWindowHandle = CreateWindow(L"XYENGINE_GAME", (LPCWSTR)m_pWindowTitle, m_pWindowStyle, x, y, width, height, NULL, NULL, m_pWindowInstance, NULL);

		if (!m_pWindowHandle)
		{
			std::cout << "[XyEngine] [Window] Failed to Create Window" << std::endl;
			return false;
		}

		ShowWindow(m_pWindowHandle, SW_SHOW);

		m_pDeviceContext = GetDC(m_pWindowHandle);

		PIXELFORMATDESCRIPTOR pfd;
		ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;

		int format = ChoosePixelFormat(m_pDeviceContext, &pfd);

		if (!SetPixelFormat(m_pDeviceContext, format, &pfd))
		{
			std::cout << "[XyEngine] [Window] Unable to Set Pixel Format" << std::endl;
			return false;
		}

		m_pOpenGLRenderContext = wglCreateContext(m_pDeviceContext);

		if (!wglMakeCurrent(m_pDeviceContext, m_pOpenGLRenderContext))
		{
			std::cout << "[XyEngine] [Window] Unable to Create Render Context" << std::endl;
			return false;
		}

		GLenum err = glewInit();

		if (err != GLEW_OK)

		{
			std::cout << "[XyEngine] [Window] Unable to Initialize Glew" << std::endl;
			return false;
		}

	}

	bool Window::Destroy()
	{
		return true;
	}

	bool Window::PollEvent(Event event)
	{
		if (event.PollMsg() != false)
			return true;
		else
			return false;
	}

	LRESULT Window::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_SIZE:
			//RecalculateWindow(LOWORD(lParam), HIWORD(lParam));
			return DefWindowProc(hwnd, msg, wParam, lParam);

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	}
}
