#include "System.hpp"
#include "File.hpp"

namespace Engine
{
	//----------------------------------------------------------------------------//
	// Subsystem
	//----------------------------------------------------------------------------//

	//----------------------------------------------------------------------------//
	bool Subsystem::Startup(void)
	{
		if (m_initstate == InitState::Initial)
		{
			m_initstate = _Startup() ? InitState::Started : InitState::Failed;
		}
		return m_initstate == InitState::Started;
	}
	//----------------------------------------------------------------------------//
	void Subsystem::Shutdown(void)
	{
		if (m_initstate != InitState::Initial)
		{
			_Shutdown();
			m_initstate = InitState::Initial;
		}
	}
	//----------------------------------------------------------------------------//

	//----------------------------------------------------------------------------//
	// System
	//----------------------------------------------------------------------------//

	//---------------------------------------------------------------------------//
	System::System(const String& _overrideConfigFileName)
	{
		m_subsystems.Push(new FileSystem);
		
		
		// load config
		{
			String _nname = PathUtils::Full(_overrideConfigFileName);
			if (_nname.IsEmpty())
				_nname = gFileSystem->AppShortName() + ".json";

			String _testPath = _nname;
			m_configName = _testPath;
			struct stat _st;
			if (!PathUtils::IsFull(_testPath))
			{
				for (uint i = 0; i < 3; ++i)
				{
					if (!stat(m_configName, &_st) && (_st.st_mode & S_IFREG))
						break;
					_testPath = "../" + _testPath;
					m_configName = PathUtils::Full(_testPath, gFileSystem->AppDir());
				}
			}

			if (stat(m_configName, &_st) || !(_st.st_mode & S_IFREG))
			{
				m_configName = PathUtils::Full(_nname, gFileSystem->AppDir());
				LOG_WARNING("Configuration not found, used default settings");
			}
			else
			{
				FILE* _f = fopen(m_configName, "rb");
				if (_f)
				{

					fseek(_f, 0, SEEK_END);
					uint _size = (uint)ftell(_f);
					fseek(_f, 0, SEEK_SET);
					Array<char> _text(_size + 1);
					_size = (uint)fread(_text.Data(), 1, _size, _f);
					_text[_size] = 0;
					fclose(_f);

					String _err;
					if (m_config.Parse(_text.Data(), &_err))
					{
						m_configLoaded = true;
					}
					else
					{
						LOG_ERROR("Unable to load configuration : %s%s", m_configName.CStr(), _err.CStr());
					}

				}
				else
				{
					LOG_WARNING("Unable to load configuration : Couldn't open file '%s'", m_configName.CStr());
				}
			}

			m_configPath = PathUtils::FilePath(m_configName);
		}

		



		//m_subsystems.Push(new Device);
		//m_subsystems.Push(new Graphics);
	}
	//---------------------------------------------------------------------------//
	System::~System(void)
	{
		while (m_subsystems.NonEmpty())
		{
			m_subsystems.Pop();
		}

		// save config
		{
			FILE* _f = fopen(m_configName, "wb");
			if (_f)
			{
				String _str = m_config.Print();
				fwrite(_str.Data(), 1, _str.Length(), _f);
				fflush(_f);
				fclose(_f);
			}
			else
			{
				LOG_ERROR("Couldn't save configuration : Couldn't create file '%s'", m_configName.CStr());
			}
		}
	}
	//---------------------------------------------------------------------------//
	bool System::ConfigFileNotLoaded(void)
	{
		return !m_configLoaded;
	}
	//---------------------------------------------------------------------------//
	Variant& System::GetConfig(const String& _section)
	{
		return _section.IsEmpty() ? m_config : m_config[_section];
	}
	//---------------------------------------------------------------------------//
	bool System::OnEvent(int _id, const Variant& _args)
	{
		for (auto i : m_subsystems)
		{
			if (i->OnEvent(_id, _args))
				return true;
		}
		return false;
	}
	//---------------------------------------------------------------------------//
	bool System::SendEventReverse(int _id, const Variant& _args)
	{
		for (auto i = m_subsystems.End(); i-- != m_subsystems.Begin();)
		{
			if ((*i)->OnEvent(_id, _args))
				return true;
		}
		return false;
	}
	//---------------------------------------------------------------------------//
	bool System::Frame(void)
	{
		return
			OnEvent(FrameEvent::Begin) &&
			OnEvent(FrameEvent::Update) &&
			OnEvent(FrameEvent::Draw) &&
			SendEventReverse(FrameEvent::End);
	}
	//---------------------------------------------------------------------------//
	bool System::_Startup(void)
	{
		LOG_EVENT("Startup Engine");

		Variant _settings;
		//TODO:
		for (auto i : m_subsystems)
			i->LoadSettings(_settings);


		for (auto i : m_subsystems)
		{
			if (!i->Startup())
			{
				LOG_ERROR("Engine startup error");
				return false;
			}
		}

		LOG_INFO("Startup Engine: successful");

		return true;
	}
	//---------------------------------------------------------------------------//
	void System::_Shutdown(void)
	{
		LOG_EVENT("Shutdown Engine");

		for (auto i = m_subsystems.End(); i-- != m_subsystems.Begin();)
		{
			(*i)->Shutdown();
		}

		LOG_INFO("Shutdown Engine: successful");

	}
	//---------------------------------------------------------------------------//

	//----------------------------------------------------------------------------//
	// 
	//----------------------------------------------------------------------------//
}
