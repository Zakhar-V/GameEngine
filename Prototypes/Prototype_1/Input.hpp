#pragma once

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

#include "System.hpp"

//---------------------------------------------------------------------------//
// Input 
//---------------------------------------------------------------------------//

//!
#define gInput Input::Get()

//!
class Input : public System, public Singleton<Input>
{
public:
	RTTI(Input);

	//!
	EventResult OnEvent(int _id, const Variant& _args = Variant::Null) override;

protected:
	//!
	bool _Startup(void)	override;
	//!
	void _Shutdown(void) override;
};

//---------------------------------------------------------------------------//
//  
//---------------------------------------------------------------------------//