#pragma once

//---------------------------------------------------------------------------//
// Includes
//---------------------------------------------------------------------------//

#include "Common.hpp"
#include "Container.hpp"
#include "Debug.hpp"
#include "String.hpp"
#include "Serialization.hpp"
#include "System.hpp"
#include "File.hpp"


#include "Math.hpp"
//---------------------------------------------------------------------------//
// Definitions
//---------------------------------------------------------------------------//

//!
#define gEngine Engine::Get()

//---------------------------------------------------------------------------//
// Engine
//---------------------------------------------------------------------------//

//!
/*class Engine : public System, public Singleton<Engine>
{
public:


	//!
	Engine(void);
	//!
	~Engine(void);

	//!
	void BeginFrame(void);
	//!
	void EndFrame(void);

protected:

	//!
	bool _Startup(void);
	//!
	void _Shutdown(void);

	//!
	std::list<SharedPtr<System>> m_systems;
};*/

//---------------------------------------------------------------------------//
// 
//---------------------------------------------------------------------------//
