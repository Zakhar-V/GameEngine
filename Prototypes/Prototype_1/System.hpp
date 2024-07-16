#pragma once

//----------------------------------------------------------------------------//
// Includes 
//----------------------------------------------------------------------------//

#include "Object.hpp"
#include "Variant.hpp"

//---------------------------------------------------------------------------//
// EventResult
//---------------------------------------------------------------------------//

enum class EventResult
{
	//Fail,
	Pass,
	Break
};

//---------------------------------------------------------------------------//
// FrameEvent
//---------------------------------------------------------------------------//

struct FrameEvent
{
	enum Enum
	{
		Begin = ConstHash("FrameEvent::Begin"),
		End = ConstHash("FrameEvent::End")
	};
};

//---------------------------------------------------------------------------//
// System
//---------------------------------------------------------------------------//

//!
class System : public Object
{
public:
	RTTI(System);

	//!
	bool Startup(void);
	//!
	void Shutdown(void);
	//!
	virtual EventResult OnEvent(int _id, const Variant& _args = Variant::Null) { return EventResult::Pass; }

protected:
	//!
	virtual bool _Startup(void) { return true; }
	//!
	virtual void _Shutdown(void) { }

	//!
	bool m_initialized = false;
};

//---------------------------------------------------------------------------//
// 
//---------------------------------------------------------------------------//
