#pragma once

//----------------------------------------------------------------------------//
// Includes 
//----------------------------------------------------------------------------//

#include "System.hpp"

//----------------------------------------------------------------------------//
// PeriodTimer
//----------------------------------------------------------------------------//

//!
struct PeriodTimer
{
	//!
	PeriodTimer(float _period) : period(_period) { }
	//!
	bool Tick(float _dt) { return (time += _dt) > period; }
	//!
	void Reset(void) { time = 0; }

	//!
	PeriodTimer operator += (float _dt) { time += _dt; return *this; }
	//!
	operator bool(void) const { return time > period; }

	//!
	float period = 0;
	//!
	float time = 0;
};

//----------------------------------------------------------------------------//
// Timer
//----------------------------------------------------------------------------//

//!
#define gTimer Timer::Get()

//!
class Timer : public System, public Singleton<Timer>
{
public:
	RTTI(Timer);

	//!
	int Frame(void) { return m_frame; }
	//!
	float DeltaTime(void) { return m_deltaTime; }
	//!
	float FramesPerSecond(void) { return m_fps; }
	//!
	float CurrentTime(void);

	//!
	EventResult OnEvent(int _id, const Variant& _args = Variant::Null) override;


protected:
	//!
	bool _Startup(void)	override;
	//!
	void _Shutdown(void) override;

	//!
	int m_frame = 0;
	//!
	double m_prevTime = 0;
	//!	in seconds
	float m_deltaTime = 0;

	//!
	float m_fps = 0;
	//!
	int m_fpsCounter = 0;
	//!
	PeriodTimer m_fpsTimer = PeriodTimer(1.0f);
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
