#pragma once

#include "System.hpp"
#include "Math.hpp"

struct GLFWwindow;

namespace Tiny
{
	//---------------------------------------------------------------------------//
	// WindowEvent
	//---------------------------------------------------------------------------//

	//!
	struct WindowEvent
	{
		enum ID
		{
			//! попытка пользователя закрыть окно (arg=0)
			Close = ConstHash("WindowEvent::Close"),
			//!	изменение размера окна (arg=0)
			Resize = ConstHash("WindowEvent::Resize"),
		};
	};

	//---------------------------------------------------------------------------//
	// Window
	//---------------------------------------------------------------------------//

	//!
#	define gWindow Tiny::Window::Get()

	//!
	class Window : public System, public Singleton<Window>
	{
	public:
		//!
		RTTI(Window);

		//!
		bool Startup(void) override;
		//!
		void Shutdown(void) override;
		//!
		bool OnEvent(int _id, void* _arg = nullptr) override;

		//!
		bool ShouldClose(void) { return m_shouldClose; }
		//!
		void ShouldClose(bool _state, bool _sendEvent = true);

		//!
		typedef GLFWwindow* Handle;
		//!
		Handle _GetHandle(void) { return m_handle; }

	protected:

		//!
		bool m_shouldClose = false;
		//!
		Handle m_handle = nullptr;

		//!
		static void _OnExit(Handle);
	};

	//---------------------------------------------------------------------------//
	// 
	//---------------------------------------------------------------------------//
}
