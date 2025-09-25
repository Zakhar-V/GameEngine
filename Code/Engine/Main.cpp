#include "Main.hpp"
#include "File.hpp"

namespace Engine
{
	//----------------------------------------------------------------------------//
	// 
	//----------------------------------------------------------------------------//

	//---------------------------------------------------------------------------//
	Main::Main(const String& _overrideConfigFileName)
	{
		m_systems.Push(new FileSystem);

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
	Main::~Main(void)
	{
		while (m_systems.NonEmpty())
		{
			m_systems.Pop();
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
	bool Main::ConfigFileNotLoaded(void)
	{
		return !m_configLoaded;
	}
	//---------------------------------------------------------------------------//
	Variant& Main::GetConfig(const String& _section)
	{
		return _section.IsEmpty() ? m_config : m_config[_section];
	}
	//---------------------------------------------------------------------------//
	bool Main::OnEvent(int _id, const Variant& _args)
	{
		for (auto i : m_systems)
		{
			if (i->OnEvent(_id, _args))
				return true;
		}
		return false;
	}
	//---------------------------------------------------------------------------//
	bool Main::SendEventReverse(int _id, const Variant& _args)
	{
		for (auto i = m_systems.End(); i-- != m_systems.Begin();)
		{
			if ((*i)->OnEvent(_id, _args))
				return true;
		}
		return false;
	}
	//---------------------------------------------------------------------------//
	bool Main::Frame(void)
	{
		return false;
			/*OnEvent(FrameEvent::Begin) &&
			OnEvent(FrameEvent::Update) &&
			OnEvent(FrameEvent::Draw) &&
			SendEventReverse(FrameEvent::End);*/
	}
	//---------------------------------------------------------------------------//
	bool Main::_Startup(void)
	{
		LOG_EVENT("Startup Engine");

		Variant _settings;
		//TODO:
		for (auto i : m_systems)
			i->LoadSettings(_settings);


		for (auto i : m_systems)
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
	void Main::_Shutdown(void)
	{
		LOG_EVENT("Shutdown Engine");

		for (auto i = m_systems.End(); i-- != m_systems.Begin();)
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
