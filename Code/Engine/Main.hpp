#pragma once

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

#include "System.hpp"

namespace Engine
{
	//----------------------------------------------------------------------------//
	// Definitions
	//----------------------------------------------------------------------------//

	//!
#	define gMain Engine::Main::Get()

	//!
	/*struct FrameEvent
	{
		enum ID : uint
		{
			Begin = String::ConstHash("FrameEvent::Begin"),
			Update = String::ConstHash("FrameEvent::Update"),
			Draw = String::ConstHash("FrameEvent::Draw"),
			End = String::ConstHash("FrameEvent::End"),
		};
	};

	//!
#	define gSystem Engine::System::Get()*/

	//----------------------------------------------------------------------------//
	// System
	//----------------------------------------------------------------------------//

	//!
	class ENGINE_API Main : public System, public Singleton<Main>
	{
	public:
		RTTI(Main);

		//!
		Main(const String& _overrideConfigFileName = String::Empty);
		//!
		~Main(void);

		//!
		bool ConfigFileNotLoaded(void);
		//!	return directory
		const String& GetConfigPath(void) { return m_configPath; }
		//!
		const String& GetConfigName(void) { return m_configName; }
		//!
		Variant& GetConfig(const String& _section = String::Empty);

		//!
		virtual bool OnEvent(int _id, const Variant& _args = Variant::Null);
		//!
		bool SendEventReverse(int _id, const Variant& _args = Variant::Null);

		//!
		bool Frame(void);

	protected:

		//!
		bool _Startup(void);
		//!
		void _Shutdown(void);

		//!
		List<SharedPtr<System>> m_systems;
		//!
		Variant m_config;
		//!
		String m_configPath;
		//!
		String m_configName;
		//!
		bool m_configLoaded = false;
	};

	//----------------------------------------------------------------------------//
	// 
	//----------------------------------------------------------------------------//
}
