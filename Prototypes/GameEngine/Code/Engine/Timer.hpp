#pragma once

#include "System.hpp"

namespace Engine
{
	//----------------------------------------------------------------------------//
	// Definitions
	//----------------------------------------------------------------------------//

#define gTimer Engine::Timer::Get()

	//----------------------------------------------------------------------------//
	// Timer
	//----------------------------------------------------------------------------//

	class ENGINE_API Timer : public Subsystem, public Singleton<Timer>
	{
	public:

	protected:

	};

	//----------------------------------------------------------------------------//
	// 
	//----------------------------------------------------------------------------//
}
