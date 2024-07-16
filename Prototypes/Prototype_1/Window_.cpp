#include "Window.hpp"
#include "Internal.hpp"
#include "Engine.hpp"


//---------------------------------------------------------------------------//
// Window 
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
bool Window::UserRequestsExit(void)
{
	return glfwWindowShouldClose(m_window);
}
//---------------------------------------------------------------------------//
EventResult Window::OnEvent(int _id, const Variant& _args)
{
	switch (_id)
	{
	case FrameEvent::Begin:
		glfwPollEvents();
		break;
	};

	return EventResult::Pass;
}
//---------------------------------------------------------------------------//
bool Window::_Startup(void)
{
	LOG("%s", __FUNCTION__);

	m_window = glfwCreateWindow(800, 600, "Game", nullptr, nullptr);

	if (!m_window)
	{
		LOG("ERROR: Unable to create GLFW window");
		return false;
	}

	glfwMakeContextCurrent(m_window);
	glfwSetWindowSizeCallback(m_window, _SizeCallback);

	return true;
}
//---------------------------------------------------------------------------//
void Window::_Shutdown(void)
{
	LOG("%s", __FUNCTION__);

	if (m_window)
	{
		glfwDestroyWindow(m_window);
		m_window = nullptr;
	}
}
//---------------------------------------------------------------------------//
void Window::_SizeCallback(GLFWwindow* _window, int _width, int _height)
{
	gWindow->m_size = { _width, _height };
	//gEngine->SendEvent(WindowEvent::Resize, gWindow->m_size);
}
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
//
//---------------------------------------------------------------------------//
