#include "Time.hpp"
#include "Internal.hpp"

//---------------------------------------------------------------------------//
// Timer
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
float Timer::CurrentTime(void)
{
	return (float)glfwGetTime();
}
//---------------------------------------------------------------------------//
EventResult Timer::OnEvent(int _id, const Variant& _args)
{

	switch (_id)
	{
	case FrameEvent::Begin:
	{
		if (m_frame == 0)
			m_prevTime = glfwGetTime();

		m_frame++;
		double _time = glfwGetTime();
		m_deltaTime = (float)(_time - m_prevTime);
		m_prevTime = _time;

		m_fpsCounter++;
		if (m_fpsTimer.Tick(m_deltaTime))
		{
			m_fps = m_fpsCounter / m_fpsTimer.time;
			m_fpsCounter = 0;
			m_fpsTimer.Reset();
		}
	} break;

	case FrameEvent::End:
	{

	} break;

	};
	return EventResult::Pass;
}
//---------------------------------------------------------------------------//
bool Timer::_Startup(void)
{
	LOG("%s", __FUNCTION__);

	return true;
}
//---------------------------------------------------------------------------//
void Timer::_Shutdown(void)
{
	LOG("%s", __FUNCTION__);
}
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// 
//---------------------------------------------------------------------------//
