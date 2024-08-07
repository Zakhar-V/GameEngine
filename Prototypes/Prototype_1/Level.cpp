#include "Level.hpp"
#include "Factory.hpp"

//---------------------------------------------------------------------------//
// Level
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
EventResult Level::OnEvent(int _id, const Variant& _args)
{
	if (m_currentScene)
		return m_currentScene->OnEvent(_id, _args);

	return EventResult::Pass;
}
//---------------------------------------------------------------------------//
bool Level::_Startup(void)
{
	LOG("%s", __FUNCTION__);

	gFactory->Register<Component>();
	gFactory->Register<Entity>();
	gFactory->Register<Scene>();

	return true;
}
//---------------------------------------------------------------------------//
void Level::_Shutdown(void)
{
	LOG("%s", __FUNCTION__);
}
//---------------------------------------------------------------------------//
 
//---------------------------------------------------------------------------//
//
//---------------------------------------------------------------------------//
