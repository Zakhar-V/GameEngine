#pragma once

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

#include "Object.hpp"
#include "Serialization.hpp"

namespace Engine
{
	//----------------------------------------------------------------------------//
	// Definitions
	//----------------------------------------------------------------------------//
	
	//!
	struct FrameEvent
	{
		enum ID	: uint
		{
			Begin = String::ConstHash("FrameEvent::Begin"),
			Update = String::ConstHash("FrameEvent::Update"),
			Draw = String::ConstHash("FrameEvent::Draw"),
			End = String::ConstHash("FrameEvent::End"),
		};
	};

	//!
#	define gSystem Engine::System::Get()

	//----------------------------------------------------------------------------//
	// Subsystem
	//----------------------------------------------------------------------------//

	//!
	class ENGINE_API Subsystem : public Object
	{
	public:
		RTTI(Subsystem);

		//!
		enum class InitState
		{
			Initial,
			Started,
			Failed,
		};

		//!
		virtual void LoadSettings(const Variant& _settings) { }
		//!
		virtual void SaveSettings(Variant& _settings) { }
		//!
		bool Startup(void);
		//!
		void Shutdown(void);
		//!
		virtual bool OnEvent(int _id, const Variant& _args = Variant::Null) { return false; }
		//!
		//template <class T> bool OnEvent(T _id, const Variant& _args = Variant::Null) { return OnEvent(static_cast<int>(_id), _args); }

	protected:

		//!
		virtual bool _Startup(void) { return true; }
		//!
		virtual void _Shutdown(void) { }

		InitState m_initstate = InitState::Initial;
	};

	//----------------------------------------------------------------------------//
	// System
	//----------------------------------------------------------------------------//
		
	//!
	class ENGINE_API System : public Subsystem, public Singleton<System>
	{
	public:
		RTTI(System);

		//!
		System(const String& _overrideConfigFileName = String::Empty);
		//!
		~System(void);

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
		List<SharedPtr<Subsystem>> m_subsystems;
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
