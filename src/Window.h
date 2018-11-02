#pragma once
#include "Header.h"
#include "Event.h"

namespace XyEngine
{
	class Window
	{
	private:
		HWND			m_pWindowHandle;
		HINSTANCE		m_pWindowInstance;
		HDC				m_pDeviceContext;
		HGLRC			m_pOpenGLRenderContext;

		UINT			m_pWindowWidth;
		UINT			m_pWindowHeight;

		DWORD			m_pWindowStyle;

		char*			m_pWindowTitle;

		bool			m_pFullScreen;
	public:
		Window(UINT width, UINT height, char* title);
		~Window();

		bool Create();
		bool Destroy();

		bool PollEvent(Event event);

		LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		// Window Getters
		HWND			GetWindowHandle() { return this->m_pWindowHandle; }
		HINSTANCE		GetWindowInstance() { return this->m_pWindowInstance; }
		HDC				GetDeviceContext() { return this->m_pDeviceContext; }
		HGLRC			GetOpenGLRenderContext() { return this->m_pOpenGLRenderContext; }

		UINT			GetWindowHeight() { return this->m_pWindowHeight; }

		DWORD			GetWindowStyle() { return this->m_pWindowStyle; }

		char*			GetWindowTitle() { return this->m_pWindowTitle; }

		bool			GetWindowFullscreen() { return this->m_pFullScreen; }

		// Window Setters

		void			SetWindowHandle(HWND hwnd) { this->m_pWindowHandle = hwnd; }
		void			SetWindowInstance(HINSTANCE hinstance) { this->m_pWindowInstance = hinstance; }
		void			SetDeviceContext(HDC hdc) { this->m_pDeviceContext = hdc; }
		void			SetOpenGLRenderContext(HGLRC hglrc) { this->m_pOpenGLRenderContext = hglrc; }

		void			SetWindowWidth(UINT width) { this->m_pWindowWidth = width; }
		void			SetWindowHeight(UINT height) { this->m_pWindowHeight = height; }

		void			SetWindowStyle(DWORD style) { this->m_pWindowStyle = style; }

		void			SetWindowTitle(char* title) { this->m_pWindowTitle = title; }

		void			SetWindowFullscreen(bool value) { this->m_pFullScreen = value; }

	};
}

