#pragma once

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

#include "Object.hpp"
#include "Serialization.hpp"

namespace Engine
{
	//----------------------------------------------------------------------------//
	// Subsystem
	//----------------------------------------------------------------------------//

	//!
	class ENGINE_API System : public Object
	{
	public:
		RTTI(System);

		//!
		enum class InitState
		{
			Initial,
			Started,
			Failed,
		};

		//!
		virtual void LoadSettings(const Variant& _settings) { }
		//!
		virtual void SaveSettings(Variant& _settings) { }
		//!
		bool Startup(void);
		//!
		void Shutdown(void);
		//!
		virtual bool OnEvent(int _id, const Variant& _args = Variant::Null) { return false; }
		//!
		//template <class T> bool OnEvent(T _id, const Variant& _args = Variant::Null) { return OnEvent(static_cast<int>(_id), _args); }

	protected:

		//!
		virtual bool _Startup(void) { return true; }
		//!
		virtual void _Shutdown(void) { }

		InitState m_initstate = InitState::Initial;
	};

	//----------------------------------------------------------------------------//
	// 
	//----------------------------------------------------------------------------//
}
