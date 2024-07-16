#pragma once

//---------------------------------------------------------------------------//
// Includes
//---------------------------------------------------------------------------//

#include "System.hpp"

//---------------------------------------------------------------------------//
// Engine
//---------------------------------------------------------------------------//

//!
#define gEngine Engine::Get()

//!
class Engine : public System, public Singleton<Engine>
{
public:
	RTTI(Engine);

	//!
	Engine(void);
	//!
	~Engine(void);

	//!
	void AddSystem(System* _system);
	//!
	void RemoveSystem(System* _system);

	//!
	EventResult OnEvent(int _id, const Variant& _args = Variant::Null) override;

	//!
	EventResult SendEvent(int _id, const Variant& _args = Variant::Null);
	//!
	EventResult SendEventReverse(int _id, const Variant& _args = Variant::Null);

	//!
	void Frame(void);

protected:

	//!
	bool _Startup(void)	override;
	//!
	void _Shutdown(void) override;

	//!
	std::vector<SharedPtr<System>> m_systems;
};

//---------------------------------------------------------------------------//
// 
//---------------------------------------------------------------------------//
