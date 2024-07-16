#pragma once

#include "System.hpp"

namespace Tiny
{
	//---------------------------------------------------------------------------//
	// Timer
	//---------------------------------------------------------------------------//

	//!
#	define gTimer Tiny::Timer::Get()

	//!
	class Timer : public System, public Singleton<Timer>
	{
	public:
		//!
		RTTI(Timer);

		//!
		bool Startup(void) override;
		//!
		void Shutdown(void) override;
		//!
		bool OnEvent(int _id, void* _arg = nullptr) override;

		//!
		float Delta(void) { return m_deltaTime; }

	protected:
		//!
		int m_frame = 0;
		//!
		double m_prevTime = 0;
		//!
		float m_deltaTime = 0; // seconds
	};

	//---------------------------------------------------------------------------//
	// 
	//---------------------------------------------------------------------------//
}
