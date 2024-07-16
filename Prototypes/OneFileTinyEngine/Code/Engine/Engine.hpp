#pragma once

#include "System.hpp"

namespace Tiny
{
	//---------------------------------------------------------------------------//
	// Engine
	//---------------------------------------------------------------------------//
	
	//!
#	define gEngine Tiny::Engine::Get()

	//!
	class Engine : public System, public Singleton<Engine>
	{

	public:

		//!
		Engine(void);
		//!
		~Engine(void);

		//!
		bool Startup(void) override;
		//!
		void Shutdown(void) override;

		//!
		void AddSystem(SystemRef _system);

		//!
		bool SendEvent(int _id, void* _arg = nullptr, bool _reverse = false);

		//!
		void Frame(void);

	protected:

		//!
		std::list<SystemRef> m_systems;
	};

	//---------------------------------------------------------------------------//
	// 
	//---------------------------------------------------------------------------//
}