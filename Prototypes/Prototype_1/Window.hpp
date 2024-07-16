#pragma once

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

#include "System.hpp"
#include "Math.hpp"

//---------------------------------------------------------------------------//
// Types 
//---------------------------------------------------------------------------//

struct GLFWwindow;

//---------------------------------------------------------------------------//
// Window 
//---------------------------------------------------------------------------//

//!
#define gWindow Window::Get()

//!
class Window : public System, public Singleton<Window>
{
public:
	RTTI(Window);

	//!
	GLFWwindow* Handle(void) { return m_window; }


	const IntVector2& GetSize(void) { return m_size; }

	//!
	bool UserRequestsExit(void);

	//!
	EventResult OnEvent(int _id, const Variant& _args = Variant::Null) override;


protected:
	//!
	bool _Startup(void)	override;
	//!
	void _Shutdown(void) override;

	static void _SizeCallback(GLFWwindow* _window, int _width, int _height);

	//!
	GLFWwindow* m_window = nullptr;

	IntVector2 m_size = { 800, 600 };
};

//---------------------------------------------------------------------------//
//  
//---------------------------------------------------------------------------//
