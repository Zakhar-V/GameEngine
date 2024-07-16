#include "EngineAPI.hpp"
#include "Internal.hpp"

//---------------------------------------------------------------------------//
// Engine
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
Engine::Engine(void)
{
	AddSystem(new Factory);
	AddSystem(new Timer);
	AddSystem(new FileSystem);
	AddSystem(new ResourceManager);
	AddSystem(new Window);
	AddSystem(new Input);
	AddSystem(new Graphics);
	AddSystem(new Level);
	AddSystem(new Renderer);
}
//---------------------------------------------------------------------------//
Engine::~Engine(void)
{

}
//---------------------------------------------------------------------------//
void Engine::AddSystem(System* _system)
{
	if (_system)
	{
		for (auto i : m_systems)
		{
			if (i == _system)
				return;
		}

		LOG("Add system '%s'", _system->Class().c_str());

		m_systems.push_back(_system);
	}
}
//---------------------------------------------------------------------------//
void Engine::RemoveSystem(System* _system)
{
	for (auto i = m_systems.begin(); i != m_systems.end(); ++i)
	{
		if (*i == _system)
		{
			LOG("Remove system '%s'", _system->Class().c_str());

			m_systems.erase(i);
			break;
		}
	}
}
//---------------------------------------------------------------------------//
EventResult Engine::OnEvent(int _id, const Variant& _args)
{
	return EventResult::Pass;
}
//---------------------------------------------------------------------------//
EventResult Engine::SendEvent(int _id, const Variant& _args)
{
	for (auto i : m_systems)
	{
		if (i->OnEvent(_id, _args) == EventResult::Break)
			return EventResult::Break;
	}
	return EventResult::Pass;
}
//---------------------------------------------------------------------------//
EventResult Engine::SendEventReverse(int _id, const Variant& _args)
{
	for (auto i = m_systems.rbegin(); i != m_systems.rend(); ++i)
	{
		if ((*i)->OnEvent(_id, _args) == EventResult::Break)
			return EventResult::Break;
	}
	return EventResult::Pass;
}
//---------------------------------------------------------------------------//
void Engine::Frame(void)
{
	SendEvent(FrameEvent::Begin);
	SendEventReverse(FrameEvent::End);
}
//---------------------------------------------------------------------------//
bool Engine::_Startup(void)
{
	LOG("%s\n{", __FUNCTION__);

	glfwInit();


	for (auto i : m_systems)
	{
		if (!i->Startup())
			return false;
	}

	LOG("}");

	return true;
}
//---------------------------------------------------------------------------//
void Engine::_Shutdown(void)
{
	LOG("%s\n{", __FUNCTION__);

	for (auto i = m_systems.rbegin(); i != m_systems.rend(); ++i)
	{
		(*i)->Shutdown();
	}

	m_systems.clear();

	LOG("}");
}
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// 
//---------------------------------------------------------------------------//

