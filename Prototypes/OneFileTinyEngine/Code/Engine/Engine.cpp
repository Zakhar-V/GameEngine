#include "Engine.hpp"
#include "Frame.hpp"
#include "Time.hpp"
#include "File.hpp"
#include "Resource.hpp"
#include "Window.hpp"
#include "Graphics.hpp"
#include "Scene.hpp"

#include "GLFW/glfw3.h"

namespace Tiny
{
	//---------------------------------------------------------------------------//
	// Engine
	//---------------------------------------------------------------------------//

	//---------------------------------------------------------------------------//
	Engine::Engine(void)
	{
		glfwInit();

		AddSystem(new Timer);
		AddSystem(new FileSystem);
		AddSystem(new ResourceManager);
		AddSystem(new Window);
		AddSystem(new Graphics);
		AddSystem(new SceneManager);
	}
	//---------------------------------------------------------------------------//
	Engine::~Engine(void)
	{
		m_systems.clear();
	}
	//---------------------------------------------------------------------------//
	bool Engine::Startup(void)
	{
		LOG("Startup: Engine ...");

		for (auto i : m_systems)
		{
			if (!i->Startup())
			{
				LOG("Startup: Engine ... FAILED");
				return false;
			}
		}

		LOG("Startup: Engine ... OK");

		return true;
	}
	//---------------------------------------------------------------------------//
	void Engine::Shutdown(void)
	{
		LOG("Shutdown: Engine ...");

		for (auto i = m_systems.rbegin(); i != m_systems.rend(); ++i)
			(*i)->Shutdown();

		LOG("Shutdown: Engine ... OK");
	}
	//---------------------------------------------------------------------------//
	void Engine::AddSystem(SystemRef _system)
	{
		ASSERT(_system != nullptr);

		m_systems.push_back(_system);
		LOG("Add System : %s", _system->ClassName().c_str());
	}
	//---------------------------------------------------------------------------//
	bool Engine::SendEvent(int _id, void* _arg, bool _reverse)
	{
		if (_reverse)
		{
			for (auto i = m_systems.rbegin(); i != m_systems.rend(); ++i)
				if ((*i)->OnEvent(_id, _arg))
					return true;
		}
		else
		{
			for (auto i : m_systems)
				if (i->OnEvent(_id, _arg))
					return true;
		}

		return false;
	}
	//---------------------------------------------------------------------------//
	void Engine::Frame(void)
	{
		SendEvent(FrameEvent::Begin);
		SendEvent(FrameEvent::PreUpdate);
		SendEvent(FrameEvent::Update);
		SendEvent(FrameEvent::PostUpdate);
		SendEvent(FrameEvent::Draw);
		SendEvent(FrameEvent::End, nullptr, true);
	}
	//---------------------------------------------------------------------------//

	//---------------------------------------------------------------------------//
	// 
	//---------------------------------------------------------------------------//

}
