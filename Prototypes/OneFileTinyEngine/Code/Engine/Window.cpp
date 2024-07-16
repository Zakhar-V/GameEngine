#include "Window.hpp"
#include "Engine.hpp"
#include "Frame.hpp"
#include "GLFW/glfw3.h"

namespace Tiny
{
	//---------------------------------------------------------------------------//
	// Window
	//---------------------------------------------------------------------------//

	//---------------------------------------------------------------------------//
	bool Window::Startup(void)
	{
		LOG("Startup: Window");

		m_handle = glfwCreateWindow(640, 480, "TinyEngine", nullptr, nullptr);

		if (!m_handle)
		{
			LOG("Error: Failed to create the window");
			return false;
		}

		glfwSetWindowCloseCallback(m_handle, _OnExit);

		return true;
	}
	//---------------------------------------------------------------------------//
	void Window::Shutdown(void)
	{
		LOG("Shutdown: Window");

		if (m_handle)
		{
			glfwDestroyWindow(m_handle);
			m_handle = nullptr;
		}
	}
	//---------------------------------------------------------------------------//
	bool Window::OnEvent(int _id, void* _arg)
	{
		if (_id == FrameEvent::Begin)
		{
			glfwPollEvents();
		}

		return false;
	}
	//---------------------------------------------------------------------------//
	void Window::ShouldClose(bool _state, bool _sendEvent)
	{
		m_shouldClose = _state;
		if (_state && _sendEvent)
			gEngine->SendEvent(WindowEvent::Close);
	}
	//---------------------------------------------------------------------------//
	void Window::_OnExit(Handle)
	{
		gWindow->ShouldClose(true, true);
	}
	//---------------------------------------------------------------------------//

	//---------------------------------------------------------------------------//
	// 
	//---------------------------------------------------------------------------//
}
