#pragma once

#include "System.hpp"

namespace Engine
{
	//----------------------------------------------------------------------------//
	// Definitions
	//----------------------------------------------------------------------------//

#define gInput Engine::Input::Get()

	//----------------------------------------------------------------------------//
	// Graphics
	//----------------------------------------------------------------------------//

	class ENGINE_API Input : public Subsystem, public Singleton<Input>
	{
	public:

	protected:

	};

	//----------------------------------------------------------------------------//
	// 
	//----------------------------------------------------------------------------//
}
