#pragma once

#include "Object.hpp"

namespace Tiny
{
	//---------------------------------------------------------------------------//
	// System
	//---------------------------------------------------------------------------//

	//!
	typedef Reference<class System> SystemRef;

	//!
	class System : public Object
	{
	public:
		//!
		RTTI(System);

		//!
		virtual bool Startup(void) { return true; }
		//!
		virtual void Shutdown(void) { }

	protected:
	};

	//---------------------------------------------------------------------------//
	// 
	//---------------------------------------------------------------------------//
}
